#!/bin/bash

for i in $(seq 1 16);
do
    for _ in $(seq 1 20);
    do
        mpirun --hostfile hostfile -np "$i" parallel.out
    done
done
