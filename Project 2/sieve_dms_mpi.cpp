#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <mpi.h>

using namespace std;

typedef long long ll;

void sieve(bool* primes, ll start, ll end) {
    ll ini = 3;
    if(ini < start)
        ini = start;

    for(ll i = ini; i<end; i+=2) {
        int idx = (i>>1) - (start>>1);
        if (!primes[idx]) {
            for(int j = i*i; j<end; j+=i<<1) {
                idx = (j>>1) - (start>>1);
                primes[idx] = true;
            }
        }
    }
}

void rmSmallPrimes(bool* primes, ll start, ll end, int n) {

    bool *sml_primes = new bool[((int)sqrt(n))>>1]();
    ll k = 3;
    do
    {
        ll x = (start - k*k) / (2*k);
        ll ini = k*k + 2*k*x;

        if(x < 0)
            x = 0;
        else if(ini < start) {
            x++; 
        }
        ini = k*k + 2*k*x;

        for (long long j = ini ; j<end ; j+=k<<1)
        {   primes[(j>>1)-(start>>1)]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= end && sml_primes[k>>1]);
        
    } while (k*k <= end);
    delete[] sml_primes;
}

ll countPrimes(bool *primes, int s, ll ini, ll end) {
    ll count = 0;
    if(ini%2 == 0)
        ++ini;
    for (int i=ini; i<end; i+=2) {
        int idx = (i>>1)-(ini>>1);
        if (!primes[idx]) {
            count+=1;
        }
    }
    return count;
}

int main (int argc, char *argv[])
{
    ll n = 5;
    if(argc >= 2) 
        n = 2<<(atoi(argv[1])-1);

    int rank, size;

   	MPI_Init(&argc, &argv);
   	MPI_Comm_size(MPI_COMM_WORLD, &size);
   	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    struct timespec start, end;
    ll total = 0;
    
    ll block_size = n / size + 1;

    if(rank == 0)
	    clock_gettime(CLOCK_REALTIME, &start);

    ll b_ini = rank * block_size;
    ll b_end = b_ini + block_size;
    b_end = b_end > n ? n : b_end;
    ll count = 0;
    int s = (b_end>>1) - (b_ini>>1); 
    bool *primes = new bool[s]();
    if(b_ini < b_end) {
        rmSmallPrimes(primes, b_ini, b_end, n);
        sieve(primes, b_ini, b_end);
        count = countPrimes(primes, s, b_ini, b_end);
//        cout << "From " << b_ini << " to " << b_end << " got " << count << " primes on rank=" << rank << endl;
    }
    MPI_Reduce(&count, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0) { 
        clock_gettime(CLOCK_REALTIME, &end);

        double time_spent = (end.tv_sec - start.tv_sec) +
                            (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Loop took %f seconds to execute, using size %d. Found %llu primes\n", time_spent, size, total);
    }

    delete[] primes;

    MPI_Finalize();

    return 0;
}
