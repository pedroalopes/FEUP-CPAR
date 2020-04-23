# Commands

## Commands to run OpenMP 
```
g++ -O2 -Wall -fopenmp sieve_sms.cpp -o sieve.sms.out && ./sieve_sms.out
```

## Commands to run MPI
```
mpic++ -o sieve_sms_mpi.out sieve_sms_mpi.cpp
mpirun -np 8 ./sieve_sms_mpi.out 2
```
