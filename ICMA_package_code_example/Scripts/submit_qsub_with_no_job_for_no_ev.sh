#!/bin/bash

address=$(pwd)
Nbins=100

for (( a=1; a<=220; a++ ))
  do
    #mkdir p$a    #Based on my plan, MATLAB has already made these p$a folders.
    #cd $address/p$a
    for (( b=1; b<=1; b++ ))
      do
        cd $address/p$a
        echo p$a
        mkdir v$b
        cd v$b
        #if (( a<=1 ))       #This is for not submitting unnecessary jobs when there is no modes for that bin.
          #then
            cp $address/p$a/ev_real.txt .
            cp $address/in.SiGe .
            cp $address/lattice.xyz .
            cp $address/SiCGe.tersoff .
            cp $address/jobrun_data.pbs .
            cp $address/init_pos.txt .
            cp $address/lmp_mpi_tersoff_summodes_center .
            kk=159700
            (( kk=kk+b ))
            sed -i "s/"999999"/"$kk"/g" in.SiGe

            c=$(awk '{print NF ; exit}' < ev_real.txt)
            sed -i "s/"replacethiswithNmodes"/"$c"/g" init_pos.txt
            sed -i "s/"SiGejobname"/"cSaG-$a-$b"/g" jobrun_data.pbs
            sed -i "s/"requiredtime"/"05:00:00:00"/g" jobrun_data.pbs

            sed -i "s/"queuename"/"prometforce-6"/g" jobrun_data.pbs
            qsub jobrun_data.pbs

          #else
          #  cp $address/Qzeros.txt ./Q.txt
        #fi
      done
  done
cd $address/

