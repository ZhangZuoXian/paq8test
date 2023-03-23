#!/bin/bash
rm -f /home/zzx/paq8test/output/*
BLOCKSIZE=60
INFOPATH="/home/zzx/paq8test/test/info.txt"
nohup ./paq8test ${BLOCKSIZE} book1   >> ${INFOPATH} 2>&1 &
nohup ./paq8test ${BLOCKSIZE} book2   >> ${INFOPATH} 2>&1 &
nohup ./paq8test ${BLOCKSIZE} enwik8  >> ${INFOPATH} 2>&1 &
nohup ./paq8test ${BLOCKSIZE} dickens >> ${INFOPATH} 2>&1 &
nohup ./paq8test ${BLOCKSIZE} osdb    >> ${INFOPATH} 2>&1 &
nohup ./paq8test ${BLOCKSIZE} nci     >> ${INFOPATH} 2>&1 &
nohup ./paq8test ${BLOCKSIZE} reymont >> ${INFOPATH} 2>&1 &
nohup ./paq8test ${BLOCKSIZE} webster >> ${INFOPATH} 2>&1 &
