#pragma once

#include "../map/ContextMap2.hpp"
#include <thread>
#include <time.h>
/**
 * Model for order 0-14 contexts
 * Contexts are hashes of previous 0..14 bytes.
 * Order 0..6, 8 and 14 are used for prediction.
 * Note: order 7+ contexts are modeled by matchModel as well.
 */
class NormalModel {
private:
    static constexpr int nCM = 9;
    static constexpr int nSM = 3;
    Shared * const shared;
    ContextMap2 cm;
    StateMap smOrder0Slow;
    StateMap smOrder1Slow;
    StateMap smOrder1Fast;
    uint64_t cxt[15] {}; // context hashes
public:
    static constexpr int MIXERINPUTS =
            nCM * (ContextMap2::MIXERINPUTS + ContextMap2::MIXERINPUTS_RUN_STATS + ContextMap2::MIXERINPUTS_BYTE_HISTORY) + nSM; //66
    static constexpr int MIXERCONTEXTS = 64 + 8 + 1024 + 256 + 256 + 256 + 256 + 1536; //3656
    static constexpr int MIXERCONTEXTSETS = 7;
    NormalModel(Shared* const sh, uint64_t cmSize);
    void reset();

    /**
     * update order 1..14 context hashes.
     * Note: order 0 context does not need an update so its hash never changes.
     */
    void updateHashes();
    void mix(Mixer &m);

    /**
     * setting more mixer contexts after skipping the special blockTypes
     * @param m
     */
    void mixPost(Mixer &m);

    //-----------------
    //并行处理
    //-----------------
    void pretreatment();
    

};

