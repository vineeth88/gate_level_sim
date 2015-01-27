#!/bin/bash
echo $1 $2

cd fsim
cp $1.lev $1_$2.lev
cp $1.eqf $1_$2.eqf
cp $1.init $1_$2.init
#../exec/fsimNew -io $1_$2 > $1_$2.out
../exec/fsimNew -io $1_$2 > $1_$2.out
rm $1_$2.lev $1_$2.eqf $1_$2.init $1_$2.frs
cd ..
echo "Faultsim complete"

