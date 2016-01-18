#!/bin/bash
pot=1
for i in {0..20}
do
	echo $pot
	pot=$(($pot*2))
	mpirun -np 2 --hostfile hostfile --mca btl tcp,self round_trip_latency.elf -s $pot
done
