#include "model/NormalModel.hpp"

NormalModel::NormalModel(Shared* const sh, const uint64_t cmSize) : 
  shared(sh), cm(sh, cmSize, nCM, 64, CM_USE_RUN_STATS | CM_USE_BYTE_HISTORY),
  smOrder0Slow(sh, 1, 255, 1023, StateMap::Generic), 
  smOrder1Slow(sh, 1, 255 * 256, 1023, StateMap::Generic),
  smOrder1Fast(sh, 1, 255 * 256, 64, StateMap::Generic) // 64->16 is also ok
{
  assert(isPowerOf2(cmSize));
}

void NormalModel::reset() {
  memset(&cxt[0], 0, sizeof(cxt));
}

void NormalModel::updateHashes() {
  INJECT_SHARED_c1
  for( int i = 14; i > 0; --i ) {
    cxt[i] = combine64(cxt[i - 1], c1 + (i << 10U));
  }
}

void NormalModel::mix(Mixer &m) {
  INJECT_SHARED_bpos

  Stats::prePath.clear();
  Stats::addPath(name);  

  if( bpos == 0 ) {
    updateHashes();
    for( int i = 1; i <= 7; ++i ) {
      cm.set(cxt[i]);
    }
    cm.set(cxt[9]);
    cm.set(cxt[14]);
    cxt_record = cxt[6];
  }
  cm.mix(m);
  INJECT_SHARED_c0 
  INJECT_SHARED_c1

  Stats::addPath(std::to_string(cxt_record));
  int prediction = Stats::avg();
  RedisHandler& rds = RedisHandler::get_instance();
  int rel = rds.setValue(Stats::prePath,std::to_string(prediction));
  Stats::stat_flag = false;

  m.add((stretch(smOrder0Slow.p1(c0 - 1))) >> 2U); //order 0
  m.add((stretch(smOrder1Fast.p1((c0 - 1) << 8U | c1))) >> 2U); //order 1
  m.add((stretch(smOrder1Slow.p1((c0 - 1) << 8U | c1))) >> 2U); //order 1

  const int order = max(0, cm.order - (nCM - 7)); //0-7
  assert(0 <= order && order <= 7);
  m.set(order << 3U | bpos, 64);
  shared->State.order = order;
}

void NormalModel::mixPost(Mixer &m) {
  INJECT_SHARED_c4
  uint32_t c2 = (c4 >> 8U) & 0xffU;
  uint32_t c3 = (c4 >> 16U) & 0xffU;
  uint32_t c;

  INJECT_SHARED_c0
  INJECT_SHARED_c1
  INJECT_SHARED_bpos
  INJECT_SHARED_blockType
  m.set(8 + (c1 | static_cast<int>(bpos > 5) << 8U | static_cast<int>(((c0 & ((1U << bpos) - 1)) == 0) || (c0 == ((2 << bpos) - 1))) << 9U), 8 + 1024);
  m.set(c0, 256);
  m.set(shared->State.order | ((c4 >> 6U) & 3U) << 3U | static_cast<int>(bpos == 0) << 5U | static_cast<int>(c1 == c2) << 6U | static_cast<int>(blockType == EXE) << 7U, 256);
  m.set(c2, 256);
  m.set(c3, 256);
  if( bpos != 0 ) {
    c = c0 << (8 - bpos);
    if( bpos == 1 ) {
      c |= c3 >> 1U;
    }
    c = min(bpos, 5) << 8U | c1 >> 5U | (c2 >> 5U) << 3U | (c & 192U);
  } else {
    c = c3 >> 7U | (c4 >> 31U) << 1U | (c2 >> 6U) << 2U | (c1 & 240U);
  }
  m.set(c, 1536);
}

