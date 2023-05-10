#!/bin/bash
rm -f /home/zzx/paq8test/output/*
BLOCKSIZE=4
TRAINRATIO=1
FILENAME="ne"
# RUNPATH="/home/zzx/paq8test/build/paq8test"
RETRAIN="/home/zzx/paq8test/test/paq8test_retrain"
TRAINONCE="/home/zzx/paq8test/test/paq8test_train_once"
INFOPATH="/home/zzx/paq8test/test/tmptest.txt"
THETA=0.8

# 单次训练的脚本
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 1 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 2 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 5 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 10 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 20 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 25 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 30 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 40 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 50 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 75 >> ${INFOPATH} 2>&1 &
# nohup ${TRAINONCE} ${BLOCKSIZE} ${FILENAME} 100 >> ${INFOPATH} 2>&1 &

# 重训练脚本
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    2    2    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    2    1    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    2    0.5  ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    2    0.1  ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    1    2    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    1    1    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    1    0.5  ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    1    0.1  ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.5  2    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.5  1    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.5  0.5  ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.5  0.1  ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.1  2    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.1  1    ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.1  0.5  ${THETA} >> ${INFOPATH} 2>&1 &
nohup ${RETRAIN} ${BLOCKSIZE} ${FILENAME}    0.1  0.1  ${THETA} >> ${INFOPATH} 2>&1 &


# nohup ${RUNPATH} ${BLOCKSIZE} book1   ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} book2   ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} enwik8  ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} dickens ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} osdb    ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} nci     ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} reymont ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} webster ${TRAINRATIO} >> ${INFOPATH} 2>&1 &

# nohup ${RUNPATH} ${BLOCKSIZE} ne   1 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} ne   10 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} ne   100 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} bbe   1 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} bbe   10 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} bbe   100 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de   1 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de   10 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de   100 >> ${INFOPATH} 2>&1 &

# nohup ${RUNPATH} ${BLOCKSIZE} de 1 1 0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de 1 0.1 0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de 0.1 0.1 0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de 1 1 0.7 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de 1 0.1 0.7 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de 0.1 0.1 0.7 >> ${INFOPATH} 2>&1 &