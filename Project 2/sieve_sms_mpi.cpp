#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <papi.h>

using namespace std;

typedef long long ll;
bool *primes;

void sieve(ll start, ll end) {

    ll k = 3;

    do
    {
        ll x = (start - k*k) / (k<<1);
        if(x < 0)
            x = 0;
        for (long long j = k*k + 2*k*x ; j<end ; j+=k<<1)
        {   primes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= end && primes[k>>1]);
        
    } while (k*k <= end);
}

ll countPrimes(int ini, int end) {
    ll count = 0;
    if(ini%2 == 0)
        ++ini;
    for (int i=ini; i<end; i+=2)
        if (!primes[i>>1]) {
            count+=1;
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
    
    // PAPI 
	int EventSet = PAPI_NULL;
  	ll values[2];
  	int ret;

	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		cout << "FAIL" << endl;

	ret = PAPI_create_eventset(&EventSet);
	if (ret != PAPI_OK)
		cout << "ERRO: create eventset" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
	if (ret != PAPI_OK)
		cout << "ERRO: PAPI_L1_DCM" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
	if (ret != PAPI_OK)
		cout << "ERRO: PAPI_L2_DCM" << endl;

	ret = PAPI_start(EventSet);
	if (ret != PAPI_OK)
		cout << "ERRO: Start PAPI" << endl;
    // PAPI

    primes = new bool[n>>1]();

    struct timespec start, end;
    ll total = 0;
    
    ll block_size = n / size + 1;

    if(rank == 0)
	    clock_gettime(CLOCK_REALTIME, &start);

    ll b_ini = rank * block_size;
    ll b_end = b_ini + block_size;
    b_end = b_end > n ? n : b_end;
    sieve(b_ini, b_end);
    ll count = countPrimes(b_ini, b_end);
//    cout << "From " << b_ini << " to " << b_end << " got " << count << " primes on rank=" << rank << endl;
    MPI_Reduce(&count, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0) { 
        clock_gettime(CLOCK_REALTIME, &end);

        double time_spent = (end.tv_sec - start.tv_sec) +
                            (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Loop took %f seconds to execute, using size %d. Found %llu primes\n", time_spent, size, total);
    }

    free(primes);

    MPI_Finalize();

    // PAPI 
	ret = PAPI_stop(EventSet, values);
  		if (ret != PAPI_OK) cout << "ERRO: Stop PAPI" << endl;

    printf("\tL1 DCM: %lld \n", values[0]);
    printf("\tL2 DCM: %lld \n", values[1]);

	ret = PAPI_reset( EventSet );
	if ( ret != PAPI_OK )
		cout << "FAIL reset" << endl;

	ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
	if ( ret != PAPI_OK )
		cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
	if ( ret != PAPI_OK )
		cout << "FAIL remove event" << endl;

	ret = PAPI_destroy_eventset( &EventSet );
	if ( ret != PAPI_OK )
		cout << "FAIL destroy" << endl;
    // PAPI 
}
