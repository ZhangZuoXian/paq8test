#!/bin/bash
rm -f /home/zzx/paq8test/output/*
BLOCKSIZE=4
TRAINRATIO=1
# RUNPATH="/home/zzx/paq8test/build/paq8test"
RUNPATH="/home/zzx/paq8test/test/paq8test_retrain"
INFOPATH="/home/zzx/paq8test/test/tmptest.txt"

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

# nohup ${RUNPATH} ${BLOCKSIZE} de    2    2    0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    2    1    0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    2    0.5  0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    1    2    0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    1    1    0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    1    0.5  0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    0.5  2    0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    0.5  1    0.8 >> ${INFOPATH} 2>&1 &
# nohup ${RUNPATH} ${BLOCKSIZE} de    0.5  0.5  0.8 >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} de    2    0.1  0.8 >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} de    0.5  0.1  0.8 >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} de    0.1  2    0.8 >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} de    0.1  1    0.8 >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} de    0.1  0.5  0.8 >> ${INFOPATH} 2>&1 &

