#pragma once

class IPredictor {
public:
    virtual void update() = 0;
    virtual ~IPredictor() = default;
};
