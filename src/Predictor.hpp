#pragma once

#include <cstdint>
#include "ContextModel.hpp"
#include "Shared.hpp"
#include "UpdateBroadcaster.hpp"
#include "utils.hpp"
#include "Models.hpp"

class Predictor{
private:
    Shared* shared;
    Models models;
    
    ContextModel contextModel;
    int pr;

    
public:
    Predictor(Shared* const sh);
    void update(uint8_t y);
    [[nodiscard]] auto p() -> int;
    
    
};