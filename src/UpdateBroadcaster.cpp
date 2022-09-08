#include "UpdateBroadcaster.hpp"
#include <stdio.h>

void UpdateBroadcaster::subscribe(IPredictor *subscriber) {
  
  // if(subscriber->getType() == 1){printf("%d\n", subscriber->getType());}
  subscribers[n] = subscriber;
  n++;
  assert(n < 1024);
}
//getType: 0=Mixer, 1=AdaptiveMap&stateMap, 2=contextMap, 3=contextMap2, 4=SmallStationaryMap, 5=StationaryMap
void UpdateBroadcaster::broadcastUpdate(bool updateState) {
  for( int i = 0; i < n; i++ ) {
    IPredictor *subscriber = subscribers[i];
    // printf("%d\n", subscriber->getType());
    if(subscriber->getType() == 0 || subscriber->getType() == 1){ //stateMap必须启用更新，但用share的updateState控制是否实质更新
      subscriber->update();
    }
    else if(updateState && subscriber->getType() != 2){
      subscriber->update();
    }
  }
  n = 0;
}
