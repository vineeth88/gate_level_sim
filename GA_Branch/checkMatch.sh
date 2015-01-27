echo $1 $2
cp $1_$2.vec fsim/
./run_fsim.sh $1 $2
exec/$1TestSim $1_$2 > $1Sim_$2
awk "(NR%2==1)" $1Sim_$2 > $1rsim_$2
awk "(NR%3==2)" fsim/$1_$2.out > $1fsim_$2
vimdiff $1fsim_$2 $1rsim_$2
