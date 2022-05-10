#!/bin/bash

mkdir -p out/teen
mkdir -p out/orca

for i in {1..32};
do
    export OMP_NUM_THREADS=$i
    srun -c $i -p teen time -p ./build/par.out 2>> out/teen/teen_$i.time
done

for i in {1..128};
do
    export OMP_NUM_THREADS=$i
    srun -c $i -p orca time -p ./build/par.out 2>> out/orca/orca_$i.time
done
