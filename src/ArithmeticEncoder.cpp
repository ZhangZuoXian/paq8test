#include "ArithmeticEncoder.hpp"

void ArithmeticEncoder::prefetch() {
  for (int i = 0; i < 4; ++i) {
    x = (x << 8) + (getc(archive) & 255);
  }
}

void ArithmeticEncoder::encodeBit(int p, int bit,int bpos) {
  if (p == 0) p++;

  assert(p > 0 && p < 4096);
  uint32_t xMid = x1 + ((x2 - x1) >> 12) * p + (((x2 - x1) & 0xFFF) * p >> 12); //(x2-x1)的高20位*p，低12位*p
  assert(xMid >= x1 && xMid < x2);
  bit != 0 ? (x2 = xMid) : (x1 = xMid + 1);

  while (((x1 ^ x2) & 0xFF000000) == 0) { // pass equal leading bytes of range
    putc(x2 >> 24,archive);
    x1 <<= 8;
    x2 = (x2 << 8) + 255;
  }

}

int ArithmeticEncoder::decodeBit(int p){
  if (p == 0) p++;
  assert(p > 0 && p < 4096);
  uint32_t xMid = x1 + ((x2 - x1) >> 12) * p + (((x2 - x1) & 0xFFF) * p >> 12);
  assert(xMid >= x1 && xMid < x2);
  int bit = static_cast<int>(x <= xMid);
  bit != 0 ? (x2 = xMid) : (x1 = xMid + 1);
  while (((x1 ^ x2) & 0xFF000000) == 0) { // pass equal leading bytes of range
    x1 <<= 8;
    x2 = (x2 << 8) + 255;
    x = (x << 8) + (getc(archive) & 255); // EOF is OK
  }
  return bit;
}

void ArithmeticEncoder::flush(){
  putc(x1 >> 24,archive);
}

void ArithmeticEncoder::reset() {
  x1 = 0;
  x2 = 0xffffffff;
  x = 0;
}