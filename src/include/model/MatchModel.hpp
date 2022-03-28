#ifndef PAQ8PX_MATCHMODEL_HPP
#define PAQ8PX_MATCHMODEL_HPP


#include "../Shared.hpp"
#include "../map/ContextMap2.hpp"
#include "IndirectContext.hpp"
#include "../map/SmallStationaryContextMap.hpp"
#include "../map/StationaryMap.hpp"
#include "util/redisHandler.hpp"

/**
 * Predict the next bit based on a preceding long matching byte sequence
 *
 * This model monitors byte sequences and keeps their most recent positions in a hashtable.
 * When the current byte sequence matches an older sequence (having the same hash) the model predicts the forthcoming bits.
 */


class MatchModel {
private:
    static constexpr int numHashes = 3; //代表有多少阶的哈希
    static constexpr int nCM = 2;
    static constexpr int nST = 3;
    static constexpr int nSSM = 2;
    static constexpr int nSM = 2;
    Shared * const shared;
    enum Parameters : uint32_t {
        MaxExtend = 0, /**< longest allowed match expansion // warning: larger value -> slowdown */
        MinLen = 5, /**< minimum required match length */
        StepSize = 2, /**< 每个高阶哈希之间的间隔，additional minimum length increase per higher order hash */
    };
    Array<uint32_t> table;
    StateMap stateMaps[nST];
    ContextMap2 cm;
    SmallStationaryContextMap SCM;
    StationaryMap maps[nSM];
    IndirectContext<uint8_t> iCtx;
    uint32_t hashes[numHashes] {0}; //存储哈希之后的结果
    uint32_t ctx[nST] {0};
    uint32_t length = 0; /**< rebased length of match (length=1 represents the smallest accepted match length), or 0 if no match */
    uint32_t index = 0; /**< points to next byte of match in buf, 0 when there is no match */
    uint32_t lengthBak = 0; /**< allows match recovery after a 1-byte mismatch */
    uint32_t indexBak = 0;  //
    uint8_t expectedByte = 0; /**< prediction is based on this byte (buf[index]), valid only when length>0 */
    bool delta = false; /**< indicates that a match has just failed (delta mode) */
    const uint32_t mask;
    const int hashBits;   /*表示table的下标,比如table只有8个元素，那这个就是3位*/
    Ilog *ilog = Ilog::getInstance();

public:
    std::string name = "MactchModel";
    static constexpr int MIXERINPUTS = 2 + nCM * (ContextMap2::MIXERINPUTS + ContextMap2::MIXERINPUTS_RUN_STATS) + nST +
                                       nSSM * SmallStationaryContextMap::MIXERINPUTS + nSM * StationaryMap::MIXERINPUTS; // 23
    static constexpr int MIXERCONTEXTS = 8;
    static constexpr int MIXERCONTEXTSETS = 1;
    MatchModel(Shared* const sh, const uint64_t buffermemorysize, const uint64_t mapmemorysize);
    void update();
    void mix(Mixer &m);

    uint64_t cxt_record;
    void save(Mixer &m);
    void load(Mixer &m);
};


#endif //PAQ8PX_MATCHMODEL_HPP
