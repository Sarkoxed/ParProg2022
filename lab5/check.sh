#!/bin/bash

for i in $(seq 1 16);
do
    mpirun --hostfile hostfile -np "$i" parallel.out
done
