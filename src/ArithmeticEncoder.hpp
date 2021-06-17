#pragma once
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

class ArithmeticEncoder {
public:
  ArithmeticEncoder(FILE *f) : archive(f){};
  uint32_t x1, x2; /**< Range, initially [0, 1), scaled by 2^32 */
  uint32_t x; /**< Decompress mode: last 4 input bytes of archive */
  FILE* archive;

  void prefetch();
  void flush();
  void encodeBit(int p, int bit,int bpos);
  int decodeBit(int p);
};