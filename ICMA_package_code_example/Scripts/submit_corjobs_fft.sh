#!/bin/bash

address=$(pwd)

for (( m=1;m<=220;m++  ))
  do
    for (( k=1;k<=1;k++ ))
      do
        cd $address/p$m/v$k
        echo $m-$k
        cp $address/cor_fft_c.cpp .
        cp $address/job_aout.pbs .
        Nlines=$(cat Q.txt | wc -l)
        (( Nlines=Nlines-1 ))
        sed -i "s/"Replacethis"/"$Nlines"/g" cor_fft_c.cpp
        g++ cor_fft_c.cpp -lfftw3
        sed -i "s/"cor1"/"cor$m-$k"/g" job_aout.pbs
        qsub job_aout.pbs
      done
  done  

cd $address
