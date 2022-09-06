#include "Encoder.hpp"

Encoder::Encoder(Shared * const sh,Mode m,FILE* f,FILE* d) :predictor(sh), shared(sh), mode(m),archive(f),ari(f,d){ 
    modelTimeCost = 0;
    mapUpdateCost = 0;
    if(mode == DECOMPRESS){      
      ari.prefetch();
    }
}

void Encoder::compressByte(uint8_t c) {

    for( int i = 7; i >= 0; --i ) {
      clock_t t = clock();
      int p = predictor.p();
      // printf("p:%d\t",p);
      modelTimeCost += clock() - t;
      int y = (c >> i) & 1;
      predictor.update(y);

      ari.encodeBit(p, y,7-i);
      t = clock();
      
      mapUpdateCost += clock() - t;
    }
  
}

void Encoder::compressByteParallel(uint8_t c){

  int* p = predictor.pWPar(c);

}

auto Encoder::decompressByte() -> uint8_t {
  
  uint8_t c = 0;
  for( int i = 0; i < 8; ++i ) {
    int p = predictor.p();
    int y = ari.decodeBit(p);
    // printf("p:%d\t",p);
    c = c << 1 | y;
    predictor.update(y);
  }
  // printf("decompressByte %x\n",c);
  return c;
}

void Encoder::flush(){
  if(mode==COMPRESS)
    ari.flush();
}