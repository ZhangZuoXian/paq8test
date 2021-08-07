#pragma once
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

class ArithmeticEncoder {
public:
  ArithmeticEncoder(FILE *f,FILE *d) :x1(0),x2(0xffffffff),x(0), archive(f),decom(d){
    // printf("%x\n",x2);
  };
  uint32_t x1, x2; /**< Range, initially [0, 1), scaled by 2^32 */
  uint32_t x; /**< Decompress mode: last 4 input bytes of archive */
  FILE* archive;
  FILE* decom;

  void prefetch();
  void flush();
  void encodeBit(int p, int bit,int bpos);
  int decodeBit(int p);
};