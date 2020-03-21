#!/bin/bash
# Usage: ./script_c.sh run_type
# ./matrixprod_c args: prod_type no_lines_start no_cols_start no_lines_end no_cols_end increment [no_blocks] [no_threads]
sudo sh -c 'echo -1 >/proc/sys/kernel/perf_event_paranoid'
g++ -O2 matrixprod.cpp -o matrixprod_c -lpapi -fopenmp

if [ $# == 0 ] ; then
    ./matrixprod_c 0
    exit 1;
fi

# 1
echo " *****************************"
echo " *** Simple Multiplication ***"
echo " *****************************"
./matrixprod_c 1 1 600 600 3000 3000 400


# 2
echo " *****************************"
echo " **** Line Multiplication ****"
echo " *****************************"
./matrixprod_c 1 2 600 600 3000 3000 400

./matrixprod_c 1 2 4096 4096 10240 10240 2048

# 3
echo " *****************************"
echo " *** Block Multiplication ****"
echo " *****************************"

./matrixprod_c 1 3 4096 4096 10240 10240 2048 128 2
./matrixprod_c 1 3 4096 4096 10240 10240 2048 256 2
./matrixprod_c 1 3 4096 4096 10240 10240 2048 512 2

./matrixprod_c 1 3 4096 4096 10240 10240 2048 128 4
./matrixprod_c 1 3 4096 4096 10240 10240 2048 256 4
./matrixprod_c 1 3 4096 4096 10240 10240 2048 512 4

./matrixprod_c 1 3 4096 4096 10240 10240 2048 128 6
./matrixprod_c 1 3 4096 4096 10240 10240 2048 256 6
./matrixprod_c 1 3 4096 4096 10240 10240 2048 512 6

./matrixprod_c 1 3 4096 4096 10240 10240 2048 128 8
./matrixprod_c 1 3 4096 4096 10240 10240 2048 256 8
./matrixprod_c 1 3 4096 4096 10240 10240 2048 512 8

./matrixprod_c 1 3 4096 4096 10240 10240 2048 128 10
./matrixprod_c 1 3 4096 4096 10240 10240 2048 256 10
./matrixprod_c 1 3 4096 4096 10240 10240 2048 512 10