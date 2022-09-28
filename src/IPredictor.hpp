#pragma once

class IPredictor {
public:
    virtual void update() = 0;
    // virtual int getType() = 0;
    virtual ~IPredictor() = default;
};
