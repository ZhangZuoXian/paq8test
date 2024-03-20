#!/bin/bash

# 测试8KB“分块”在不同训练量（2%、1%、0.5%、0.1%）和不同k（2%、1%、0.5%、0.1%）、不同θ（0.8、0.9、1.0、1.1）时的压缩比和吞吐量
# 压缩等级取12，吞吐量测试两到三遍取平均值

# rm -rf ./output/${FILE}/

LOG=./paq8test.txt
COMP_LV=12
BLOCKSIZE=8

FILENAME=calgary
THETA=0.8

echo -e $(date)'\n' >> ${LOG} 2>&1 

./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.1  ${THETA} >> ${LOG} 2>&1

./paq8test ${COMP_LV} ${FILENAME} 0 1 1 1 >> ${LOG} 2>&1
echo -e $(date)'\n' >> ${LOG} 2>&1 

THETA=0.9

./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.1  ${THETA} >> ${LOG} 2>&1

./paq8test ${COMP_LV} ${FILENAME} 0 1 1 1 >> ${LOG} 2>&1
echo -e $(date)'\n' >> ${LOG} 2>&1 

THETA=1.0

./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.1  ${THETA} >> ${LOG} 2>&1

./paq8test ${COMP_LV} ${FILENAME} 0 1 1 1 >> ${LOG} 2>&1
echo -e $(date)'\n' >> ${LOG} 2>&1 

THETA=1.1

./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  2    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  1    0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.5  0.1  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  2    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  1    ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.5  ${THETA} >> ${LOG} 2>&1
./paq8test ${COMP_LV} ${FILENAME} ${BLOCKSIZE}  0.1  0.1  ${THETA} >> ${LOG} 2>&1

./paq8test ${COMP_LV} ${FILENAME} 0 1 1 1 >> ${LOG} 2>&1
echo -e $(date)'\n' >> ${LOG} 2>&1 