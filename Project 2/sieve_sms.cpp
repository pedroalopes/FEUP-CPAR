#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>

using namespace std;

typedef long long ll;
bool *primes;

void sieve(ll start, ll end) {
    
    for(ll k = 3; k*k <= end; k+=2) 
    {
        if(primes[k>>1])
            continue;

        ll j = k*k;
        if (start > j) {
            ll mult = (start-j)/k;
            if(mult < 2)
                mult = 2;
            else if(mult & 1)
                mult++;
            j+=k*mult;
        }

        for (; j<end ; j+=2*k)
        {   
            primes[j>>1]=true;
        }
    } 
}

int main (int argc, char *argv[])
{
    ll n;
    
    cout << "Power of 10: ";
    cin >> n;
 
    n = pow(10,n);
    primes = new bool[n/2];

    int nr_threads=4;
    ll block_size = n / nr_threads;
    
    struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

    #pragma omp parallel for schedule(dynamic) num_threads(nr_threads)
    for (ll i = 0; i<n ; i+=block_size) {
        ll b_end = i + block_size;
        b_end = b_end > n ? n : b_end;
        sieve(i, b_end);
    }
    
    clock_gettime(CLOCK_REALTIME, &end);

	double time_spent = (end.tv_sec - start.tv_sec) +
						(end.tv_nsec - start.tv_nsec) / 1000000000.0;

    ll count = 0;
    for (int i=1; i<n; i+=2)
        if (!primes[i>>1]) {
//            cout << i << " ";
            count+=1;
        }

    printf("Loop took %f seconds to execute. Found %llu primes\n", time_spent , count);

    free(primes);
    return 0;
}
