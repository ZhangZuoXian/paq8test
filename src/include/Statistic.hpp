#pragma once

#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cassert>
#include <functional>
#include <cmath>

#define BLOCKSIZE 4096

class Stats {
public:
    Stats() ;
    ~Stats() = default;

    static void setStatsFunc(void(*func));
    static void lossStats(uint8_t y, uint8_t bitposition);
    static void flush();
    static void record(uint32_t p, uint8_t bitposition) ;
    static bool stat_flag;
    static bool record_flag;

private:
    static uint32_t size;
    static uint32_t prediction[8];
    static std::vector<std::vector<uint32_t>> loss_stats;
    static std::vector<std::fstream> fWrite;
    static std::vector<std::string> fileNames;
    static std::string dirPath;

    static int average;
    
};