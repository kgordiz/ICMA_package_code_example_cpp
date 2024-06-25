#!/bin/bash

address2=$(pwd)

for (( m=1;m<=220;m++  ))
  do
    cd $address2/p$m/
    echo $m
    cp $address2/mode_averaging_AC_CCs.cpp .
    cp $address2/job_aout.pbs .
    g++ mode_averaging_AC_CCs.cpp
    sed -i "s/"cor1"/"ave-$m"/g" job_aout.pbs
    qsub job_aout.pbs
  done

cd $address2
