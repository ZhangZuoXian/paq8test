#include "Encoder.hpp"

Encoder::Encoder(Shared * const sh,Mode m,FILE* f) :predictor(sh), shared(sh), mode(m),archive(f),ari(f){ 
    
}

void Encoder::compressByte(uint8_t c) {

    for( int i = 7; i >= 0; --i ) {
      int p = predictor.p();
      int y = (c >> i) & 1;
      ari.encodeBit(p, y,7-i);
      predictor.update(y);
    }
  
}