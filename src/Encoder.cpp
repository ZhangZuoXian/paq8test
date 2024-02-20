#include "Encoder.hpp"

Encoder::Encoder(Shared * const sh,Mode m,FILE* f) : predictor(sh), shared(sh), 
  mode(m), ari(f, sh) { 
    if(mode == DECOMPRESS){      
      ari.prefetch();
    }
}

void Encoder::compressByte(uint8_t c) {

    for( int i = 7; i >= 0; --i ) {
      int p = predictor.p();
      int y = (c >> i) & 1;
      predictor.update(y);
      ari.encodeBit(p, y, 7-i);
    }

}

auto Encoder::decompressByte() -> uint8_t {
  
  uint8_t c = 0;
  for( int i = 0; i < 8; ++i ) {
    int p = predictor.p();
    int y = ari.decodeBit(p);
    c = c << 1 | y;
    predictor.update(y);
  }

  return c;
}

void Encoder::flush(){
  if(mode==COMPRESS)
    ari.flush();
}

void Encoder::blockReset() {
  predictor.blockReset();
  ari.reset();
  if(mode == DECOMPRESS) {
    ari.prefetch();
  }
}

void Encoder::reset() {
  predictor.reset();
}