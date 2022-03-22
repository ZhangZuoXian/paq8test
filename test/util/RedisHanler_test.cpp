#include <cstdio>
#include <thread>  // NOLINT
#include <vector>

#include "gtest/gtest.h"
#include "util/redisHandler.hpp"
#include "Encoder.hpp"
#include "Shared.hpp"

TEST(RedisHandler,SampleTest){
    using std::hash;
    RedisHandler rds;
    Shared shared;
    shared.init(2);

    Mode mode(COMPRESS);
    FILE* out = fopen("../result/paper1.paq8test","w");
    FILE* deCom = fopen("../result/paper1.txt","w");

    Encoder en(&shared, COMPRESS,out,deCom);
    uint8_t c=0x22;
    en.compressByte(c);
    rds.connect("127.0.0.1",6379);
    rds.setValue("gehujun","2");
    std::string value;
    rds.getValue("gehujun",value);
}

TEST(SimpleTest,SimpleTest){
    printf("nihaoya\n");
}