#pragma once


#include "ArithmeticEncoder.hpp"
#include "Shared.hpp"
#include "Predictor.hpp"

typedef enum {
    COMPRESS, DECOMPRESS
} Mode;

class Encoder{
private:
    Predictor predictor;
    ArithmeticEncoder ari;
    const Mode mode; /**< Compress or decompress? */
    FILE* archive;
    const Shared * const shared;
    
public:
    explicit Encoder(Shared * const sh,Mode m,FILE* f);
    void compressByte(uint8_t c);
    auto decompressByte() ->uint8_t;
};