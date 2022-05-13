#!/bin/bash

#
# collect_data.sh
#

# get host name
hname=$(hostname)
# get processor name
proc=$(lscpu | grep "Model name" | cut -d: -f2 | sed -e 's/[ "()RTM]//g')
# variants are: straigthforward (6) and swapped inner loops (7)
variants="6 7"
# square root of matrix sizes (initial and final)
#dim0=8
dim0=2
dimN=32

# clear out any previous versions of files we will build
for v in ${variants}
  do
    rm -f MMM$v-${hname}-${proc}.out
  done

# sweep through a range of matrix dimensions
for (( s = dim0; s <= dimN; s *= 2 ))
  do
    for v in ${variants}
      do
        ./MatMatMult$v $s | tee tmp.$$
        echo "$s x $s Ki, `cat tmp.$$ | awk '{print $10}'`" >> MMM$v-${hname}-${proc}.out
      done
  done

# clean up
rm -f tmp.$$
