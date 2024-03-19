#include "Shared.hpp"

void Shared::init(uint8_t level) {
  this->level = level;
  mem = 65536ULL << level;
  buf.setSize(min(mem * 8, 1ULL << 30)); /**< no reason to go over 1 GB */
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
  
  // Broadcast to all current subscribers: y (and c0, c1, c4, etc) is known
  updateBroadcaster.broadcastUpdate();
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
