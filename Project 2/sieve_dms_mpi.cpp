#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <mpi.h>
//#include <papi.h>

using namespace std;

typedef long long ll;

void sieve(unsigned int* primes, ll end) {

    ll k = 3;
    do
    {
        for (long long j = k*k ; j<end ; j+=k<<1)
        {  
            primes[j>>6]|=(1<<((j>>1)&31));
        }
        
        do
        {
            k+=2;
        }while (k*k <= end && primes[k>>6]&(1<<((k>>1)&31)));
        
    } while (k*k <= end);
}

void rmSmallPrimes(unsigned int* primes, ll start, ll end, ll n) {

    ll sml_primes_size = (((int)sqrt(n))>>6)+1; 
    unsigned int *sml_primes = new unsigned int[sml_primes_size]();
    sieve(sml_primes,((int)sqrt(n)));
    ll k = 3;
    do
    {
        ll x = (start - k*k) / (k<<1);
        ll ini = k*k + ((k*x)>>1);

        if(x < 0)
            x = 0;
        else if(ini < start) {
            x++; 
        }
        ini = k*k + 2*k*x;

        for (long long j = ini ; j<end ; j+=k<<1)
        {  
            int idx = j - start;
            primes[idx>>6]|=(1<<((idx>>1)&31));
        }
        
        do
        {
            k+=2;
        }while (k*k <= end && k < ((int)sqrt(n)) && sml_primes[k>>6]&(1<<((k>>1)&31)));
        
    } while (k*k <= end && k < ((int)sqrt(n)));
    delete[] sml_primes;
}

ll countPrimes(unsigned int *primes, ll primes_size, ll n) {
    ll count = 0;
    for (int i=0; i<(primes_size-1); i++) {
        count += 32 - __builtin_popcount(primes[i]);
    }
    int aux_end = (1<<((n>>1)&31))-1 ;
    count += __builtin_popcount(aux_end) - __builtin_popcount(primes[n>>6]&(aux_end));

    return count;
}

int main (int argc, char *argv[])
{
    ll n = 5;
    if(argc >= 2) 
        n = 1LLU<<(atoi(argv[1]));

    int rank, size;

   	MPI_Init(&argc, &argv);
   	MPI_Comm_size(MPI_COMM_WORLD, &size);
   	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // PAPI 
//	int EventSet = PAPI_NULL;
//  	long long values[2];
//  	int ret;
//
//	ret = PAPI_library_init( PAPI_VER_CURRENT );
//	if ( ret != PAPI_VER_CURRENT )
//		cout << "FAIL" << endl;
//
//	ret = PAPI_create_eventset(&EventSet);
//	if (ret != PAPI_OK)
//		cout << "ERRO: create eventset" << endl;
//
//	ret = PAPI_add_event(EventSet,PAPI_L1_DCM );
//	if (ret != PAPI_OK)
//		cout << "ERRO: PAPI_L1_DCM" << endl;
//
//	ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
//	if (ret != PAPI_OK)
//		cout << "ERRO: PAPI_L2_DCM" << endl;
//
//	ret = PAPI_start(EventSet);
//	if (ret != PAPI_OK)
//		cout << "ERRO: Start PAPI" << endl;
    // PAPI
    
    struct timespec start, end;
    ll total = 0;
    
    ll block_size = n / size + 1;

    if(rank == 0)
	    clock_gettime(CLOCK_REALTIME, &start);

    ll b_ini = rank * block_size;
    ll b_end = b_ini + block_size;
    b_end = b_end > n ? n : b_end;
    ll count = 0;

    if(b_ini&1) --b_ini; 

    ll primes_size = ((b_end-b_ini)>>6)+1; 
    unsigned int *primes = new unsigned int[primes_size]();
    if(b_ini < b_end) {
        rmSmallPrimes(primes, b_ini, b_end, n);
        count += countPrimes(primes, primes_size, b_end-b_ini);
    }
    MPI_Reduce(&count, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if(rank == 0) { 
        clock_gettime(CLOCK_REALTIME, &end);

        double time_spent = (end.tv_sec - start.tv_sec) +
                            (end.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Loop took %f seconds to execute, using size %d. Found %llu primes\n", time_spent, size, total);
    }

    delete[] primes;

    // PAPI 
//	ret = PAPI_stop(EventSet, values);
//  		if (ret != PAPI_OK) cout << "ERRO: Stop PAPI" << endl;
//
//    printf("\tL1 DCM: %lld \n", values[0]);
//    printf("\tL2 DCM: %lld \n", values[1]);
//
//	ret = PAPI_reset( EventSet );
//	if ( ret != PAPI_OK )
//		cout << "FAIL reset" << endl;
//
//	ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
//	if ( ret != PAPI_OK )
//		cout << "FAIL remove event" << endl;
//
//	ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
//	if ( ret != PAPI_OK )
//		cout << "FAIL remove event" << endl;
//
//	ret = PAPI_destroy_eventset( &EventSet );
//	if ( ret != PAPI_OK )
//		cout << "FAIL destroy" << endl;
    // PAPI 

    MPI_Finalize();

    return 0;
}
