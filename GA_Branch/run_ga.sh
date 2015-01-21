echo $1 
for i in `seq 1 $2`;
do
	exec/$1Sim lib/$1.param > $1.out$i
	sleep 5
	echo $i done
done

j=1;
for i in $(ls $1_*.vec);
do 
	echo $i
	echo $j
	mv $i $1_$j.vec
	j=$((j+1))
done
