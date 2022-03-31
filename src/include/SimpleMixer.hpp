#pragma once

#include "UpdateBroadcaster.hpp"
#include "./util/BitCount.hpp"
#include "./util/Ilog.hpp"
#include "Mixer.hpp"
#include "./util/Squash.hpp"

template<SIMD simd>
class SimpleMixer : public Mixer{
public:
    SimpleMixer(const Shared* const sh, const int n, const int m, const int s) 
        : Mixer(sh, ((n + (simdWidth() - 1)) & -(simdWidth())), m, s){}


    inline int dot_product(const short *const t, const short *const w, int n){
      int sum = 0;
      while((n -= 2) >= 0 ) {
        sum += (t[n] * w[n] + t[n + 1] * w[n + 1]) >> 8U;
      }
      return sum;
    }

    inline void train(const short *const t, short *const w, int n, const int err){
      while((n -= 1) >= 0 ) {
        int wt = w[n] + ((((t[n] * err * 2) >> 16U) + 1) >> 1U);
        if( wt < -32768 ) {
          wt = -32768;
        } else if( wt > 32767 ) {
          wt = 32767;
        }
        *reinterpret_cast<short*>(&w[n]) = wt;
      }
    }
    auto p() -> int override {
        shared->GetUpdateBroadcaster()->subscribe(this);
        int sum=0;    
        for (int i=0; i<n; ++i)
          sum+=t[i]*w[i];
        return sum>>8;
    }

private:
    /**
     * Define padding requirements.
     */
    [[nodiscard]] constexpr inline auto simdWidth() const -> int {
      if( simd == SIMD_AVX2 ) {
        return 32 / sizeof(short); // 256 bit (32 byte) data size
      }
      else if( simd == SIMD_SSE2 || simd == SIMD_SSSE3 || simd == SIMD_NEON ) {
        return 16 / sizeof(short); // 128 bit (16 byte) data size
      }
      else if( simd == SIMD_NONE ) {
        return 4 / sizeof(short); // Processes 2 shorts at once -> width is 4 bytes
      }
      assert(false);
    }



};