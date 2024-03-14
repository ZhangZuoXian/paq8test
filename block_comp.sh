#!/bin/bash

# FILE=book1
# rm -rf ./output/${FILE}/

echo $(date) >> ./block_comp.txt 2>&1 
for i in $(seq 1 10)
do
    let SIZE=2**$i
    ./paq_block 12 ${SIZE} silesia_all >> ./block_comp.txt 2>&1 
done
./paq_block 12 0 silesia >> ./block_comp.txt 2>&1 


echo $(date) >> ./block_comp.txt 2>&1 
for i in $(seq 1 10)
do
    let SIZE=2**$i
    ./paq_block 12 ${SIZE} enwik8 >> ./block_comp.txt 2>&1 
done
./paq_block 12 0 enwik8 >> ./block_comp.txt 2>&1 
echo $(date) >> ./block_comp.txt 2>&1 

# Warning：Do not use 'nohup'!