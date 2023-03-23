#pragma once


#include "ArithmeticEncoder.hpp"
#include "Shared.hpp"
#include "Predictor.hpp"
#include <time.h>

typedef enum {
    COMPRESS, DECOMPRESS
} Mode;

class Encoder{
private:
    Predictor predictor;
    ArithmeticEncoder ari;
    const Mode mode; /**< Compress or decompress? */
    // FILE* archive;
    Shared * shared;

    
public:
    explicit Encoder(Shared * const sh,Mode m,FILE* f);
    // explicit Encoder(Shared * const sh,Mode m,FILE* f, const Encoder &train_en);
    void compressByte(uint8_t c);
    auto decompressByte() ->uint8_t;
    void flush();
    void reset();

};