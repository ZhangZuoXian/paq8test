#pragma once

#include <cstdint>
#include "./model/ContextModel.hpp"
#include "Shared.hpp"
#include "UpdateBroadcaster.hpp"
#include "./util/utils.hpp"
#include "Models.hpp"

class Predictor{
private:
    Shared* shared;
    Models models;
    
    ContextModel contextModel;
    int pr;

    
public:
    Predictor(Shared* const sh);
    Predictor(Shared* const sh, const Predictor *train_p);
    void update(uint8_t y);
    [[nodiscard]] auto p() -> int;
    
    [[nodiscard]] auto pWPar(uint8_t c) -> int*;
};