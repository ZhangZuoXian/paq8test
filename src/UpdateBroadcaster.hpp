#pragma once

#include "IPredictor.hpp"
#include <cassert>

class UpdateBroadcaster {
public:
    void subscribe(IPredictor *subscriber);
    void broadcastUpdate();
    UpdateBroadcaster() {}
private:
    int n {0}; /**< number of subscribed predictors, (number of items in "subscribers" array) */
    IPredictor *subscribers[1024] {};

    /**
     * Copy constructor is private so that it cannot be called
     */
    UpdateBroadcaster(UpdateBroadcaster const & /*unused*/) {}

    /**
     * Assignment operator is private so that it cannot be called
     */
    auto operator=(UpdateBroadcaster const & /*unused*/) -> UpdateBroadcaster & { return *this; }
};