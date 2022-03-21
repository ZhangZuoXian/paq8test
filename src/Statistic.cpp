#include "Statistic.hpp"

bool Stats::stat_flag{false};
bool Stats::record_flag{false};
std::vector<std::string> Stats::fileNames{"bit1","bit2","bit3","bit4","bit5","bit6","bit7","bit8"};
std::string Stats::dirPath{"../test/statistic/"};
uint32_t Stats::prediction[8] = {0};
std::vector<std::vector<uint32_t>> Stats::loss_stats = {};
std::vector<std::fstream> Stats::fWrite = {};
int Stats::average = 0;

Stats::Stats(){
    loss_stats.resize(8);
    fWrite.resize(8);
    for(int i = 0; i < 8; i++){
        // loss_stats[i].resize(BLOCKSIZE);
        std::string filePath = dirPath + fileNames[i] + ".txt";
        fWrite[i].open(filePath, std::ios::out);
    }
}

void Stats::lossStats(uint8_t y, uint8_t bitposition){
    assert(bitposition >= 0 && bitposition <8);
    assert(y == 0 || y == 1); 
    
    if(loss_stats[0].size() < BLOCKSIZE){
        // std::cout<<" loss stats: "<< (y << 12) - prediction[bitposition]<<"\n";
        loss_stats[bitposition].emplace_back( abs((y << 12) - prediction[bitposition]) );
    }
}

void Stats::flush(){
    std::cout<<"\n\n loss statistic: "<<Stats::loss_stats[0].size()<<"\n";

    for(int i = 0; i < 8; i++){
        std::string filePath = dirPath + fileNames[i] + ".txt";
        std::cout<<filePath;
        
        long sum = 0;
        for(int j = 0; j < BLOCKSIZE; j++){
            sum += loss_stats[i][j];
            fWrite[i]<<loss_stats[i][j]<<" ";
        }
        std::cout<< "average: "<<(sum / BLOCKSIZE)<<std::endl;
        fWrite[i].close();
    }
}

void Stats::record(uint32_t p, uint8_t bitposition) {
    assert(bitposition >= 0 && bitposition <8);
    assert( p >=0 && p <=4096);
    //这里成功接收到了prediction
    prediction[bitposition] = p;
}