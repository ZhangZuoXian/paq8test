#!/bin/bash
rm -f /home/zzx/paq8test/output/*
BLOCKSIZE=1024
TRAINRATIO=100
RUNPATH="/home/zzx/paq8test/build/paq8test"
INFOPATH="/home/zzx/paq8test/test/info.txt"

nohup ${RUNPATH} ${BLOCKSIZE} book1   ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} book2   ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} enwik8  ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} dickens ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} osdb    ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} nci     ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} reymont ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
nohup ${RUNPATH} ${BLOCKSIZE} webster ${TRAINRATIO} >> ${INFOPATH} 2>&1 &
