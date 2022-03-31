#include "model/MatchModel.hpp"

MatchModel::MatchModel(Shared* const sh, const uint64_t buffermemorysize, const uint64_t mapmemorysize) : 
  shared(sh),
  table(buffermemorysize / sizeof(uint32_t)),
  stateMaps {{sh, 1, 56 * 256,          1023, StateMap::Generic},
             {sh, 1, 8 * 256 * 256 + 1, 1023, StateMap::Generic},
             {sh, 1, 256 * 256,         1023, StateMap::Generic}},
  cm(sh, mapmemorysize, nCM, 74, CM_USE_RUN_STATS),
  SCM {sh, 6, 1, 6, 64},
  maps {{sh, 23, 1, 64, 1023},
        {sh, 15, 1, 64, 1023}}, 
  iCtx {15, 1}, 
  mask(uint32_t(buffermemorysize / sizeof(uint32_t) - 1)), 
  hashBits(ilog2(mask + 1)) {
#ifdef VERBOSE
  printf("Created MatchModel with size = %" PRIu64 "\n", size);
#endif
  assert(isPowerOf2(mapmemorysize));


}

void MatchModel::update() {
  INJECT_SHARED_buf
  INJECT_SHARED_bpos
  
  if( length != 0 ) {
    const int expectedBit = (expectedByte >> ((8 - bpos) & 7U)) & 1U;
    INJECT_SHARED_y
    if( y != expectedBit ) {
      //之前有过误匹配了或者匹配到的相同字节序列长度比较小的时候，放弃匹配
      if( lengthBak != 0 && length - lengthBak < MinLen ) { // mismatch too soon in recovery mode -> give up
        lengthBak = 0;
        indexBak = 0;
      } else { //backup match information: maybe we can recover it just after this mismatch
        lengthBak = length;
        indexBak = index;
      }
      delta = true;
      length = 0;
    }
  }

  //bytewise contexts
  //预测一个字节的起始比特的时候执行
  if( bpos == 0 ) {
    // update hashes 三个哈希函数：从高阶到低阶，分别为9、7、5阶的哈希
    for( uint32_t i = 0, minLen = MinLen + (numHashes - 1) * StepSize; i < numHashes; i++, minLen -= StepSize ) {
      uint64_t hash = 0;
      for( uint32_t j = minLen; j > 0; j-- ) {
        
        hash = combine64(hash, buf(j)); //从buf(j)一直混合哈希到buf(1)：hash = hash(buf(j),buf(j-1),buf(j-2),...,buf(1) )
      }
      hashes[i] = finalize64(hash, hashBits);//限制在hashBits的范围内
    }

    // recover match after a 1-byte mismatch
    if( length == 0 && !delta && lengthBak != 0 ) { //match failed (2 bytes ago), no new match found, and we have a backup
      indexBak++;
      if( lengthBak < mask ) {
        lengthBak++;
      }
      INJECT_SHARED_c1
      if( buf[indexBak] == c1 ) { // match continues -> recover
        length = lengthBak;
        index = indexBak;
      } else { // still mismatch
        lengthBak = indexBak = 0; // purge backup
      }
    }
    
    // extend current match
    if( length != 0 ) {
      index++;
      if( length < mask ) {
        length++;
      }
    }
    delta = false;

    // find a new match, starting with the highest order hash and falling back to lower ones
    if( length == 0 ) {
      uint32_t minLen = MinLen + (numHashes - 1) * StepSize;
      uint32_t bestLen = 0;
      uint32_t bestIndex = 0;
      for( uint32_t i = 0; i < numHashes && length < minLen; i++, minLen -= StepSize ) {
        index = table[hashes[i]];
        if( index > 0 ) {//说明之前存在过哈希相同的高阶上下文
          length = 0;
          while( length < (minLen + MaxExtend) && buf(length + 1) == buf[index - length - 1] ) {
            length++;
          }
          if( length > bestLen ) {//从各个高阶中选择最长得匹配
            bestLen = length;
            bestIndex = index;
          }
        }
      }
      if( bestLen >= MinLen ) {
        length = bestLen - (MinLen - 1); // rebase, a length of 1 actually means a length of MinLen
        index = bestIndex;
        lengthBak = indexBak = 0; // purge any backup
      } else {  
        length = index = 0;
      }
    }
    // update position information in hashtable
    INJECT_SHARED_pos
    for( uint32_t i = 0; i < numHashes; i++ ) {
      table[hashes[i]] = pos;
    }
    shared->State.Match.expectedByte = expectedByte = (length != 0 ? buf[index] : 0);
  }
  
}

void MatchModel::mix(Mixer &m) {
  
  update();
  for( uint32_t i = 0; i < nST; i++ ) { // reset contexts
    ctx[i] = 0;
  }

  INJECT_SHARED_bpos
  INJECT_SHARED_c0
  INJECT_SHARED_c1
  const int expectedBit = length != 0 ? (expectedByte >> (7 - bpos)) & 1U : 0;
  if( length != 0 ) {
    if( length <= 16 ) {
      ctx[0] = (length - 1) * 2 + expectedBit; // 0..31
    } else {
      ctx[0] = 24 + (min(length - 1, 63) >> 2U) * 2 + expectedBit; // 32..55
    }
    ctx[0] = ((ctx[0] << 8U) | c0);
    ctx[1] = ((expectedByte << 11U) | (bpos << 8U) | c1) + 1;
    const int sign = 2 * expectedBit - 1;
    m.add(sign * (min(length, 32) << 5U)); // +/- 32..1024
    
    m.add(sign * (ilog->log(min(length, 65535)) << 2U)); // +/-  0..1024
  } else { // no match at all or delta mode
    m.add(0);
    m.add(0);
  }

  if( delta ) { // delta mode: helps predicting the remaining bits of a character when a mismatch occurs
    ctx[2] = (expectedByte << 8U) | c0;
  }

  for( uint32_t i = 0; i < nST; i++ ) {
    const uint32_t c = ctx[i];

    if( c != 0 ) {
      m.add(stretch(stateMaps[i].p1(c)) >> 1U);
    } else {
      m.add(0);
    }
  }

  const uint32_t lengthIlog2 = ilog2(length + 1);
  //no match:      lengthIlog2=0
  //length=1..2:   lengthIlog2=1
  //length=3..6:   lengthIlog2=2
  //length=7..14:  lengthIlog2=3
  //length=15..30: lengthIlog2=4

  const uint8_t length3 = min(lengthIlog2, 3); // 2 bits
  const auto rm = static_cast<const uint8_t>(lengthBak != 0 &&
                                             length - lengthBak == 1); // predicting the first byte in recovery mode is still uncertain
  const uint8_t length3Rm = length3 << 1U | rm; // 3 bits

  //bytewise contexts
  Stats::prePath.clear();
  Stats::addPath(name);
  INJECT_SHARED_c4
  if( bpos == 0 ) {
    if( length != 0 ) {
      cm.set(hash(0, expectedByte, length3Rm));
      cm.set(hash(1, expectedByte, length3Rm, c1)); 
      cxt_record = hash(1, expectedByte, length3Rm, c1);
    } else {
      // when there is no match it is still slightly beneficial not to skip(), but set some low-order contexts
      cm.set(hash(2, c4 & 0xffu)); // order 1
      cm.set(hash(3, c4 & 0xffffu)); // order 2
      cxt_record = hash(3, c4 & 0xffffu);
    }
  }
  
  cm.mix(m);
  Stats::addPath(std::to_string(cxt_record));
  int prediction = Stats::avg();
  RedisHandler& rds = RedisHandler::get_instance();
  int rel = rds.setValue(Stats::prePath,std::to_string(prediction));
  Stats::stat_flag = false;
  // if(rel != M_REDIS_OK){
  //   std::cout<<"fail to set kay value\n";
  // }
  
  // rds.disConnect();
  
  //bitwise contexts
  {
    
    // stationaryMap - 0
    Stats::prePath.clear();
    Stats::addPath(name);

    maps[0].set(hash(expectedByte, c0, c4 & 0xffffu, length3Rm));
    maps[0].mix(m);
    cxt_record = hash(expectedByte, c0, c4 & 0xffffu, length3Rm);

    Stats::addPath(std::to_string(cxt_record));
    int prediction = Stats::avg();
    RedisHandler& rds = RedisHandler::get_instance();
    int rel = rds.setValue(Stats::prePath,std::to_string(prediction));
    Stats::stat_flag = false;

    // stationaryMap - 1
    Stats::prePath.clear();
    Stats::addPath(name);

    INJECT_SHARED_y
    iCtx += y;
    const uint8_t c = length3Rm << 1U | expectedBit; // 4 bits
    iCtx = (bpos << 11U) | (c1 << 3U) | c;
    maps[1].setDirect(iCtx());
    maps[1].mix(m);

    SCM.set((bpos << 3U) | c);
  }
  
  SCM.mix(m);

  const uint32_t lengthC = lengthIlog2 != 0 ? lengthIlog2 + 1 : static_cast<uint32_t>(delta);
  //no match, no delta mode:   lengthC=0
  //failed match, delta mode:  lengthC=1
  //length=1..2:   lengthC=2
  //length=3..6:   lengthC=3
  //length=7..14:  lengthC=4
  //length=15..30: lengthC=5

  m.set(min(lengthC, 7), 8);
  shared->State.Match.length3 = min(lengthC, 3);
}

void MatchModel::save(Mixer &m){
  update();
  for( uint32_t i = 0; i < nST; i++ ) { // reset contexts
    ctx[i] = 0;
  }

  INJECT_SHARED_bpos
  INJECT_SHARED_c0
  INJECT_SHARED_c1
  const int expectedBit = length != 0 ? (expectedByte >> (7 - bpos)) & 1U : 0;
  if( length != 0 ) {
    if( length <= 16 ) {
      ctx[0] = (length - 1) * 2 + expectedBit; // 0..31
    } else {
      ctx[0] = 24 + (min(length - 1, 63) >> 2U) * 2 + expectedBit; // 32..55
    }
    ctx[0] = ((ctx[0] << 8U) | c0);
    ctx[1] = ((expectedByte << 11U) | (bpos << 8U) | c1) + 1;
    const int sign = 2 * expectedBit - 1;
    m.add(sign * (min(length, 32) << 5U)); // +/- 32..1024
    
    m.add(sign * (ilog->log(min(length, 65535)) << 2U)); // +/-  0..1024
  } else { // no match at all or delta mode
    m.add(0);
    m.add(0);
  }

  if( delta ) { // delta mode: helps predicting the remaining bits of a character when a mismatch occurs
    ctx[2] = (expectedByte << 8U) | c0;
  }

  for( uint32_t i = 0; i < nST; i++ ) {
    const uint32_t c = ctx[i];

    if( c != 0 ) {
      m.add(stretch(stateMaps[i].p1(c)) >> 1U);
    } else {
      m.add(0);
    }
  }

  const uint32_t lengthIlog2 = ilog2(length + 1);
  //no match:      lengthIlog2=0
  //length=1..2:   lengthIlog2=1
  //length=3..6:   lengthIlog2=2
  //length=7..14:  lengthIlog2=3
  //length=15..30: lengthIlog2=4

  const uint8_t length3 = min(lengthIlog2, 3); // 2 bits
  const auto rm = static_cast<const uint8_t>(lengthBak != 0 &&
                                             length - lengthBak == 1); // predicting the first byte in recovery mode is still uncertain
  const uint8_t length3Rm = length3 << 1U | rm; // 3 bits

  //bytewise contexts
  Stats::prePath.clear();
  Stats::addPath(name);
  INJECT_SHARED_c4
  if( bpos == 0 ) {
    if( length != 0 ) {
      cm.set(hash(0, expectedByte, length3Rm));
      cm.set(hash(1, expectedByte, length3Rm, c1)); 
      cxt_record = hash(1, expectedByte, length3Rm, c1);
    } else {
      // when there is no match it is still slightly beneficial not to skip(), but set some low-order contexts
      cm.set(hash(2, c4 & 0xffu)); // order 1
      cm.set(hash(3, c4 & 0xffffu)); // order 2
      cxt_record = hash(3, c4 & 0xffffu);
    }
  }
  
  cm.mix(m);
  Stats::addPath(std::to_string(cxt_record));
  int prediction = Stats::avg();
  RedisHandler& rds = RedisHandler::get_instance();
  int rel = rds.setValue(Stats::prePath,std::to_string(prediction));
  Stats::stat_flag = false;
  // if(rel != M_REDIS_OK){
  //   std::cout<<"fail to set kay value\n";
  // }
  
  // rds.disConnect();
  
  //bitwise contexts
  {
    
    // stationaryMap - 0
    Stats::prePath.clear();
    Stats::addPath(name);

    maps[0].set(hash(expectedByte, c0, c4 & 0xffffu, length3Rm));
    maps[0].mix(m);
    cxt_record = hash(expectedByte, c0, c4 & 0xffffu, length3Rm);

    Stats::addPath(std::to_string(cxt_record));
    int prediction = Stats::avg();
    RedisHandler& rds = RedisHandler::get_instance();
    int rel = rds.setValue(Stats::prePath,std::to_string(prediction));
    Stats::stat_flag = false;

    // stationaryMap - 1
    Stats::prePath.clear();
    Stats::addPath(name);

    INJECT_SHARED_y
    iCtx += y;
    const uint8_t c = length3Rm << 1U | expectedBit; // 4 bits
    iCtx = (bpos << 11U) | (c1 << 3U) | c;
    maps[1].setDirect(iCtx());
    maps[1].mix(m);

    SCM.set((bpos << 3U) | c);
  }
  
  SCM.mix(m);

  const uint32_t lengthC = lengthIlog2 != 0 ? lengthIlog2 + 1 : static_cast<uint32_t>(delta);
  //no match, no delta mode:   lengthC=0
  //failed match, delta mode:  lengthC=1
  //length=1..2:   lengthC=2
  //length=3..6:   lengthC=3
  //length=7..14:  lengthC=4
  //length=15..30: lengthC=5

  m.set(min(lengthC, 7), 8);
  shared->State.Match.length3 = min(lengthC, 3);
}

void MatchModel::load(Mixer &m){
  
  update();
  for( uint32_t i = 0; i < nST; i++ ) { // reset contexts
    ctx[i] = 0;
  }

  INJECT_SHARED_bpos
  INJECT_SHARED_c0
  INJECT_SHARED_c1
  const int expectedBit = length != 0 ? (expectedByte >> (7 - bpos)) & 1U : 0;
  if( length != 0 ) {
    if( length <= 16 ) {
      ctx[0] = (length - 1) * 2 + expectedBit; // 0..31
    } else {
      ctx[0] = 24 + (min(length - 1, 63) >> 2U) * 2 + expectedBit; // 32..55
    }
    ctx[0] = ((ctx[0] << 8U) | c0);
    ctx[1] = ((expectedByte << 11U) | (bpos << 8U) | c1) + 1;
    const int sign = 2 * expectedBit - 1;
    m.add(sign * (min(length, 32) << 5U)); // +/- 32..1024
    
    m.add(sign * (ilog->log(min(length, 65535)) << 2U)); // +/-  0..1024
  } else { // no match at all or delta mode
    m.add(0);
    m.add(0);
  }

  if( delta ) { // delta mode: helps predicting the remaining bits of a character when a mismatch occurs
    ctx[2] = (expectedByte << 8U) | c0;
  }

  for( uint32_t i = 0; i < nST; i++ ) {
    const uint32_t c = ctx[i];

    if( c != 0 ) {
      m.add(stretch(stateMaps[i].p1(c)) >> 1U);
    } else {
      m.add(0);
    }
  }

  const uint32_t lengthIlog2 = ilog2(length + 1);
  //no match:      lengthIlog2=0
  //length=1..2:   lengthIlog2=1
  //length=3..6:   lengthIlog2=2
  //length=7..14:  lengthIlog2=3
  //length=15..30: lengthIlog2=4

  const uint8_t length3 = min(lengthIlog2, 3); // 2 bits
  const auto rm = static_cast<const uint8_t>(lengthBak != 0 &&
                                             length - lengthBak == 1); // predicting the first byte in recovery mode is still uncertain
  const uint8_t length3Rm = length3 << 1U | rm; // 3 bits

  //bytewise contexts
  Stats::prePath.clear();
  Stats::addPath(name);
  INJECT_SHARED_c4
  if( bpos == 0 ) {
    if( length != 0 ) {
      // cm.set(hash(0, expectedByte, length3Rm));
      // cm.set(hash(1, expectedByte, length3Rm, c1)); 
      cxt_record = hash(1, expectedByte, length3Rm, c1);
    } else {
      // when there is no match it is still slightly beneficial not to skip(), but set some low-order contexts
      // cm.set(hash(2, c4 & 0xffu)); // order 1
      // cm.set(hash(3, c4 & 0xffffu)); // order 2
      cxt_record = hash(3, c4 & 0xffffu);
    }
  }
  
  cm.load(m);
  
  Stats::addPath(std::to_string(cxt_record));
  std::string prediction = "0";
  RedisHandler& rds = RedisHandler::get_instance();
  int rel = rds.getValue(Stats::prePath,prediction);
  if(rel != M_REDIS_OK){
    std::cout<<rds.getErrorMsg()<<std::endl;
  }
  int pr = atoi(prediction.c_str());
  m.add(pr);

  //bitwise contexts
  {
    
    // stationaryMap - 0
    // Stats::prePath.clear();
    // Stats::addPath(name);

    maps[0].set(hash(expectedByte, c0, c4 & 0xffffu, length3Rm));
    maps[0].mix(m);
    // cxt_record = hash(expectedByte, c0, c4 & 0xffffu, length3Rm);

    // Stats::addPath(std::to_string(cxt_record));
    // int prediction = Stats::avg();
    // RedisHandler& rds = RedisHandler::get_instance();
    // int rel = rds.setValue(Stats::prePath,std::to_string(prediction));
    // Stats::stat_flag = false;

    // stationaryMap - 1
    // Stats::prePath.clear();
    // Stats::addPath(name);

    INJECT_SHARED_y
    iCtx += y;
    const uint8_t c = length3Rm << 1U | expectedBit; // 4 bits
    iCtx = (bpos << 11U) | (c1 << 3U) | c;
    maps[1].setDirect(iCtx());
    maps[1].mix(m);

    SCM.set((bpos << 3U) | c);
  }
  
  SCM.mix(m);

  const uint32_t lengthC = lengthIlog2 != 0 ? lengthIlog2 + 1 : static_cast<uint32_t>(delta);
  //no match, no delta mode:   lengthC=0
  //failed match, delta mode:  lengthC=1
  //length=1..2:   lengthC=2
  //length=3..6:   lengthC=3
  //length=7..14:  lengthC=4
  //length=15..30: lengthC=5

  m.set(min(lengthC, 7), 8);
  shared->State.Match.length3 = min(lengthC, 3);
}