#ifndef PAQ8PX_CONTEXTMODEL_HPP
#define PAQ8PX_CONTEXTMODEL_HPP


#include "../Mixer.hpp"
#include "../SimdMixer.hpp"
#include "../Models.hpp"
#include <thread>
#include <vector>

/**
 * This combines all the context models with a Mixer.
 */
class ContextModel {
    Shared * const shared;
    Models models;
    Mixer *m;
    BlockType nextBlockType = DEFAULT;
    BlockType blockType = DEFAULT;
    int blockSize = 1;
    int blockInfo = 0;
    int bytesRead = 0;
    bool readSize = false;

public:
    ContextModel(Shared* const sh, Models &models);
    auto p() -> int;
    auto pWPar(uint8_t c) -> int*;
    ~ContextModel();

    void blockReset();
    void reset();

    // std::thread tds[8];

private:
    
    // std::vector<std::thread> tds;
};

#endif //PAQ8PX_CONTEXTMODEL_HPP
