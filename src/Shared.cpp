#include "Shared.hpp"

void Shared::init(uint8_t level) {
  this->level = level;
  mem = 65536ULL << level;
  buf.setSize(min(mem * 8, 1ULL << 30)); /**< no reason to go over 1 GB */
  toScreen = !isOutputDirected();

  // MatchModelCostTime = 0;
  // NormalModelCostTime = 0;

}

void Shared::update(int y) {
  State.y = y;
  State.c0 += State.c0 + y;
  State.bitPosition = (State.bitPosition + 1U) & 7U;
  if(State.bitPosition == 0 ) {
    State.c1 = State.c0;
    buf.add(State.c1);
    State.c8 = (State.c8 << 8U) | (State.c4 >> 24U);
    State.c4 = (State.c4 << 8U) | State.c0;
    State.c0 = 1;
  }
//   static constexpr uint8_t asciiGroup[254] = { 0, 10, 0, 1, 10, 10, 0, 4, 2, 3, 10, 10, 10, 10, 0, 0, 5, 4, 2, 2, 3, 3, 10, 10, 10, 10, 10, 10, 10, 10, 0, 0, 0, 0, 5, 5, 9, 4, 2, 2, 2, 2, 3, 3, 3, 3, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 5, 8, 8, 5, 9, 9, 6, 5, 2, 2, 2, 2, 2, 2, 2, 8, 3, 3, 3, 3, 3, 3, 3, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 8, 8, 8, 8, 5, 5, 9, 9, 9, 9, 9, 7, 8, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
//   State.Text.characterGroup = (State.bitPosition > 0) ? asciiGroup[(1U << State.bitPosition) - 2 + (State.c0 & ((1U << State.bitPosition) - 1))] : 0;
  
  // Broadcast to all current subscribers: y (and c0, c1, c4, etc) is known
  updateBroadcaster.broadcastUpdate(updateState);
}

void Shared::reset() {
  buf.reset();
  memset(&State, 0, sizeof(State));
  State.c0 = 1;
}


UpdateBroadcaster *Shared::GetUpdateBroadcaster() const {
  UpdateBroadcaster* updater = const_cast<UpdateBroadcaster*>(&updateBroadcaster);
  return updater;
}

void Shared::dynamicPara(){
  updateState = true;
}
void Shared::staticPara(){
  updateState = false;
}
bool Shared::getUpdateState(){
  return updateState;
}

auto Shared::isOutputDirected() -> bool {
#ifdef WINDOWS
  DWORD FileType = GetFileType(GetStdHandle(STD_OUTPUT_HANDLE));
  return (FileType == FILE_TYPE_PIPE) || (FileType == FILE_TYPE_DISK);
#endif
#ifdef UNIX
  return isatty(fileno(stdout)) == 0;
#endif
}
