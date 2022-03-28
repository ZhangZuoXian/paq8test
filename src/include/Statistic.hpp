#pragma once

#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cassert>
#include <functional>
#include <cmath>
#include "util/redisHandler.hpp"

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

    // offline function :
    static std::string prePath;
    static std::vector<int> avgVec;
    static int avg(){
        long sum = 0;
        int n = avgVec.size();
        for(int i = 0; i <n; i++){
            sum += avgVec[i];
        }
        avgVec.resize(0);
        return sum / n;
    }
    static void addPath(const std::string& name){
        Stats::prePath += "+"+name;
    }

private:
    static uint32_t size;
    static uint32_t prediction[8];
    static std::vector<std::vector<uint32_t>> loss_stats;
    static std::vector<std::fstream> fWrite;
    static std::vector<std::string> fileNames;
    static std::string dirPath;

    static int average;
    
};