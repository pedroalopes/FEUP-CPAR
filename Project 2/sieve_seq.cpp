#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <papi.h>

using namespace std;

typedef long long ll;

int main (int argc, char *argv[])
{
    ll n;
    
    if(argc >= 2) {
        n = 2<<(atoi(argv[1])-1);
    } else {
        cout << "Power of 10: ";
        cin >> n;
     
        n = pow(10,n);
    }

    ll primes_size = (n>>6)+1; 
    unsigned int *primes = new unsigned int[primes_size]();
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

    bool *primes = new bool[n>>1]();
    
    long long k = 3;

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    do
    {
        for (long long j = k*k ; j<n ; j+=k<<1) {   
            primes[j>>6]|=(1<<((j>>1)&31));
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && primes[k>>6]&(1<<((k>>1)&31)));
        
    } while (k*k <= n);
    
    unsigned long long count = 0;
    for (int i=0; i<primes_size; i++) {
        count += 32 - __builtin_popcount(primes[i]);
//        for(int j=1; j<=32;j++) {
//            if(((i*32+j)*2+1) >= n) break;
//            if(!(primes[i]&(1<<j))) cout << i << '\t' << j << '\t' << ((i*32+j)*2+1) << endl;
//        }
    }
    count -= ((primes_size<<6) - n)>>1;
    
    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Loop took %f seconds to execute. Found %llu primes\n", time_spent, count);

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
