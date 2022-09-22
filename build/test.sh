#!/bin/bash
BLOCKSIZE=64
nohup ./paq8test ${BLOCKSIZE} book1 >> ./info.txt 2>&1 &
nohup ./paq8test ${BLOCKSIZE} book2 >> ./info.txt 2>&1 &
nohup ./paq8test ${BLOCKSIZE} enwik8 >> ./info.txt 2>&1 &
nohup ./paq8test ${BLOCKSIZE} dickens >> ./info.txt 2>&1 &
nohup ./paq8test ${BLOCKSIZE} osdb >> ./info.txt 2>&1 &
nohup ./paq8test ${BLOCKSIZE} nci >> ./info.txt 2>&1 &
nohup ./paq8test ${BLOCKSIZE} reymont >> ./info.txt 2>&1 &
nohup ./paq8test ${BLOCKSIZE} webster >> ./info.txt 2>&1 &
