#pragma once

#include "../map/ContextMap.hpp"
#include "../Mixer.hpp"
#include "../Shared.hpp"
#include <cassert>
#include <cstdint>

/**
 * Model order 1-2-3 contexts with gaps.
 */
class SparseModel {
private:
    static constexpr int nCM = 38; //17+3*7
    const Shared * const shared;
    ContextMap cm;

public:
    static constexpr int MIXERINPUTS = nCM * (ContextMap::MIXERINPUTS); // 190
    static constexpr int MIXERCONTEXTS = 0;
    static constexpr int MIXERCONTEXTSETS = 0;
    explicit SparseModel(const Shared* const sh, uint64_t size);
    void mix(Mixer &m);
    void reset();
};