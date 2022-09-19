#!/bin/bash
# nohup ./paq8test 0 /home/zzx/paq8test/data/calgarycorpus/book1 >> ./info.txt 2>&1 &
# nohup ./paq8test 0 /home/zzx/paq8test/data/calgarycorpus/book2 >> ./info.txt 2>&1 &
# nohup ./paq8test 0 /home/zzx/paq8test/data/enwik8/enwik8 >> ./info.txt 2>&1 &
nohup ./paq8test 0 dickens >> ./info.txt 2>&1 &
nohup ./paq8test 0 osdb >> ./info.txt 2>&1 &
nohup ./paq8test 0 nci >> ./info.txt 2>&1 &
nohup ./paq8test 0 reymont >> ./info.txt 2>&1 &
nohup ./paq8test 0 webster >> ./info.txt 2>&1 &
