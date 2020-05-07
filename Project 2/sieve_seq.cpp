#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <papi.h>

using namespace std;

int main (int argc, char *argv[])
{
    long long n;
    
    if(argc >= 2) {
        n = 2<<(atoi(argv[1])-1);
    } else {
        cout << "Power of 10: ";
        cin >> n;
     
        n = pow(10,n);
    }

    // PAPI 
	int EventSet = PAPI_NULL;
  	long long values[2];
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

	struct timespec start, finish;
	double elapsed;
	if (rank == root) {
		clock_gettime(CLOCK_MONOTONIC, &start);
	}
    
    bool *primes = new bool[n>>1]();
    
    long long k = 3;

    clock_t start, finish;
    start = clock();

    do
    {
        for (long long j = k*k ; j<n ; j+=k<<1)
        {   primes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && primes[k>>1]);
        
    } while (k*k <= n);
    
    unsigned long long count = 0;
    for (int i=1; i<n; i+=2)
        if (!primes[i>>1])
            //cout << i << " ";
            ++count;

    finish = clock();
    double cpu_time_used = (double(finish-start)/CLOCKS_PER_SEC);

    printf("Loop took %f seconds to execute. Found %llu primes\n", cpu_time_used, count);

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
