#!/bin/bash

arch=$(./checkDeviceInfo | grep "Device 0" | cut -d: -f2 | sed -e 's/[ "]//g')

dim0=4
dimN=32

# sweep through a range of matrix dimensions
for (( N = dim0; N <= dimN; N *= 2 ))
  do for (( B = 2; B <= 16; B *= 2 ))
    do
      if [ $N -gt $B ]
      then
        printf "\n$arch: $N x $N matrices; $B x $B blocks\n"
        ./MatMatMult_blocks_threads $N $B
      fi
    done;
  done;
