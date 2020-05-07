#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>

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

    int nr_threads=4;
    ll block_size = n / nr_threads + 1;
    
    struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

    #pragma omp parallel for schedule(static) num_threads(nr_threads)
    for (ll i = 0; i<n ; i+=block_size) {
        ll b_end = i + block_size;
        b_end = b_end > n ? n : b_end;
        sieve(i, b_end);
    }
    
    ll count = 0;
    #pragma omp parallel for schedule(static) num_threads(nr_threads) reduction(+: count)
    for (int i=1; i<n; i+=2)
        if (!primes[i>>1]) {
//            cout << i << " ";
            ++count;
        }

    clock_gettime(CLOCK_REALTIME, &end);

	double time_spent = (end.tv_sec - start.tv_sec) +
						(end.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Loop took %f seconds to execute. Found %llu primes\n", time_spent , count);

    free(primes);
    
    // PAPI 
	ret = PAPI_stop(EventSet, values);
  		if (ret != PAPI_OK) cout << "ERRO: Stop PAPI" << endl;

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
