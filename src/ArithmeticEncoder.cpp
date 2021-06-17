#include "ArithmeticEncoder.hpp"


void ArithmeticEncoder::encodeBit(int p, int bit,int bpos) {
  // printf("encodebit");
  if (p == 0) p++;
  if(bpos == 0 && p>2048)
    printf("%d \n",p);
  assert(p > 0 && p < 4096);
  uint32_t xMid = x1 + ((x2 - x1) >> 12) * p + (((x2 - x1) & 0xFFF) * p >> 12); //(x2-x1)的高20位*p，低12位*p
  assert(xMid >= x1 && xMid < x2);
  bit != 0 ? (x2 = xMid) : (x1 = xMid + 1);
  
  while (((x1 ^ x2) & 0xFF000000) == 0) { // pass equal leading bytes of range
    // archive->putChar(x2 >> 24);
    putc(x2 >> 24,archive);
    x1 <<= 8;
    x2 = (x2 << 8) + 255;
  }

}