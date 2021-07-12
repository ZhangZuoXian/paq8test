#include "Encoder.hpp"

Encoder::Encoder(Shared * const sh,Mode m,FILE* f) :predictor(sh), shared(sh), mode(m),archive(f),ari(f){ 
    modelTimeCost = 0;
    mapUpdateCost = 0;
}

void Encoder::compressByte(uint8_t c) {

    for( int i = 7; i >= 0; --i ) {
      clock_t t = clock();
      int p = predictor.p();
      modelTimeCost += clock() - t;
      
      int y = (c >> i) & 1;
      ari.encodeBit(p, y,7-i);
      t = clock();
      predictor.update(y);
      mapUpdateCost += clock() - t;
    }
  
}

void Encoder::compressByteParallel(uint8_t c){

  int p[] = predictor.p();

}