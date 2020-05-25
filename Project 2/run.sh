#!/bin/bash
#sudo sh -c 'printf -1 >/proc/sys/kernel/perf_event_paranoid'

g++ -O2 -Wall sieve_seq.cpp -o sieve_seq.out -lpapi
g++ -O2 -Wall -fopenmp sieve_sms.cpp -o sieve_sms.out -lpapi
mpic++ -o sieve_sms_mpi.out sieve_sms_mpi.cpp -lpapi
mpic++ -o sieve_dms_mpi.out sieve_dms_mpi.cpp -lpapi
mpic++ -o sieve_sms_mpi_mp.out sieve_sms_mpi_mp.cpp -fopenmp -lpapi

date | tee -a out.log
printf "STARTING\n" | tee -a out.log
#############################################################################

# ./sieve_seq.out <max_2pot> 
printf "\n*****Sequential Sieve*****\n" | tee -a out.log
./sieve_seq.out 25 | tee -a out.log
./sieve_seq.out 26 | tee -a out.log
./sieve_seq.out 27 | tee -a out.log
./sieve_seq.out 28 | tee -a out.log
./sieve_seq.out 29 | tee -a out.log
./sieve_seq.out 30 | tee -a out.log
./sieve_seq.out 31 | tee -a out.log
./sieve_seq.out 32 | tee -a out.log

# ./sieve_sms.out <max_2pot> <nr_threads>
printf "\n*****Shared Memory Sieve - nt=1*****\n" | tee -a out.log
./sieve_sms.out 25 1 | tee -a out.log
./sieve_sms.out 26 1 | tee -a out.log
./sieve_sms.out 27 1 | tee -a out.log
./sieve_sms.out 28 1 | tee -a out.log
./sieve_sms.out 29 1 | tee -a out.log
./sieve_sms.out 30 1 | tee -a out.log
./sieve_sms.out 31 1 | tee -a out.log
./sieve_sms.out 32 1 | tee -a out.log

printf "\n*****Shared Memory Sieve - nt=2*****\n" | tee -a out.log
./sieve_sms.out 25 2 | tee -a out.log
./sieve_sms.out 26 2 | tee -a out.log
./sieve_sms.out 27 2 | tee -a out.log
./sieve_sms.out 28 2 | tee -a out.log
./sieve_sms.out 29 2 | tee -a out.log
./sieve_sms.out 30 2 | tee -a out.log
./sieve_sms.out 31 2 | tee -a out.log
./sieve_sms.out 32 2 | tee -a out.log

printf "\n*****Shared Memory Sieve - nt=3*****\n" | tee -a out.log
./sieve_sms.out 25 3 | tee -a out.log
./sieve_sms.out 26 3 | tee -a out.log
./sieve_sms.out 27 3 | tee -a out.log
./sieve_sms.out 28 3 | tee -a out.log
./sieve_sms.out 29 3 | tee -a out.log
./sieve_sms.out 30 3 | tee -a out.log
./sieve_sms.out 31 3 | tee -a out.log
./sieve_sms.out 32 3 | tee -a out.log

printf "\n*****Shared Memory Sieve - nt=4*****\n" | tee -a out.log
./sieve_sms.out 25 4 | tee -a out.log
./sieve_sms.out 26 4 | tee -a out.log
./sieve_sms.out 27 4 | tee -a out.log
./sieve_sms.out 28 4 | tee -a out.log
./sieve_sms.out 29 4 | tee -a out.log
./sieve_sms.out 30 4 | tee -a out.log
./sieve_sms.out 31 4 | tee -a out.log
./sieve_sms.out 32 4 | tee -a out.log

# mpirun -np <nr_processors> ./sieve_sms_mpi.out <max_2pot> 
printf "\n*****Shared Memory MPI Sieve - np=1*****\n" | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi.out 32 2> err.log | tee -a out.log
printf "\n*****Shared Memory MPI Sieve - np=2*****\n" | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi.out 32 2> err.log | tee -a out.log
printf "\n*****Shared Memory MPI Sieve - np=3*****\n" | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi.out 32 2> err.log | tee -a out.log
printf "\n*****Shared Memory MPI Sieve - np=4*****\n" | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi.out 32 2> err.log | tee -a out.log

# # mpirun -np <nr_processors> ./sieve_dms_mpi.out <max_2pot> 
printf "\n*****Distributed Memory MPI Sieve - np=1*****\n" | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_dms_mpi.out 32 2> err.log | tee -a out.log
printf "\n*****Distributed Memory MPI Sieve - np=2*****\n" | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_dms_mpi.out 32 2> err.log | tee -a out.log
printf "\n*****Distributed Memory MPI Sieve - np=3*****\n" | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_dms_mpi.out 32 2> err.log | tee -a out.log
printf "\n*****Distributed Memory MPI Sieve - np=4*****\n" | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 25 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 26 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 27 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 28 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 29 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 30 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 31 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_dms_mpi.out 32 2> err.log | tee -a out.log

# # mpirun -np <nr_processors> ./sieve_sms_mpi_mp.out <max_2pot> <nr_threads>
printf "\n*****Shared Memory MPI & OpenMP Sieve - np=1*****\n" | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 25 4 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 26 4 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 27 4 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 28 4 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 29 4 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 30 4 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 31 4 2> err.log | tee -a out.log
mpirun -np 1 ./sieve_sms_mpi_mp.out 32 4 2> err.log | tee -a out.log
printf "\n*****Shared Memory MPI & OpenMP Sieve - np=2*****\n" | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 25 4 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 26 4 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 27 4 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 28 4 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 29 4 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 30 4 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 31 4 2> err.log | tee -a out.log
mpirun -np 2 ./sieve_sms_mpi_mp.out 32 4 2> err.log | tee -a out.log
printf "\n*****Shared Memory MPI & OpenMP Sieve - np=3*****\n" | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 25 4 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 26 4 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 27 4 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 28 4 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 29 4 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 30 4 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 31 4 2> err.log | tee -a out.log
mpirun -np 3 ./sieve_sms_mpi_mp.out 32 4 2> err.log | tee -a out.log
printf "\n*****Shared Memory MPI & OpenMP Sieve - np=4*****\n" | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 25 4 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 26 4 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 27 4 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 28 4 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 29 4 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 30 4 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 31 4 2> err.log | tee -a out.log
mpirun -np 4 ./sieve_sms_mpi_mp.out 32 4 2> err.log | tee -a out.log

############################################################################

wait
printf "\nENDING\n" | tee -a out.log
date | tee -a out.log
rm sieve*.out
