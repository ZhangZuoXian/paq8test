#pragma once
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "Shared.hpp"

class ArithmeticEncoder {
public:
  ArithmeticEncoder(FILE *f,Shared * const sh) :x1(0),x2(0xffffffff),x(0), archive(f), shared(sh){
    
  };
  uint32_t x1, x2; /**< Range, initially [0, 1), scaled by 2^32 */
  uint32_t x; /**< Decompress mode: last 4 input bytes of archive */
  Shared * shared;
  FILE* archive;

  void prefetch();
  void flush();
  void encodeBit(int p, int bit,int bpos);
  int decodeBit(int p);
  void reset();
};