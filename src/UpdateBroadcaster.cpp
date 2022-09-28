#include "UpdateBroadcaster.hpp"
#include <stdio.h>

void UpdateBroadcaster::subscribe(IPredictor *subscriber) {
  
  subscribers[n] = subscriber;
  n++;
  assert(n < 1024);
}
//getType: 0=Mixer, 1=AdaptiveMap&stateMap, 2=contextMap, 3=contextMap2, 4=SmallStationaryMap, 5=StationaryMap
void UpdateBroadcaster::broadcastUpdate(bool updateState) {
  for( int i = 0; i < n; i++ ) {
    IPredictor *subscriber = subscribers[i];
    subscriber->update();
  }
  n = 0;
}
