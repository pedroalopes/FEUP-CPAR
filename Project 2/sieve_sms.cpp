#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>

using namespace std;

typedef long long ll;
bool *primes;

void sieve(ll k, ll n) {
    
    do
    {
        for (long long j = k*k ; j<n ; j+=2*k)
        {   primes[j>>1]=true;
        }
        
        do
        {
            k+=2;
        }while (k*k <= n && primes[k>>1]);
        
    } while (k*k <= n);

}

int main (int argc, char *argv[])
{
    ll n;
    
    cout << "Power of 10: ";
    cin >> n;
 
    n = pow(10,n);
    primes = new bool[n];

    int nr_threads=1;
    ll block_size = n / nr_threads + 1;
    
    clock_t start, finish;
    start = clock();

    #pragma omp parallel for schedule(dynamic) num_threads(nr_threads)
    for(int s = 3; s < n; s += block_size) {
        int end_point = s + block_size;
        int start_point = s;
        start_point = ((start_point>>1)<<1) + 1;
        sieve(start_point, end_point);
    }
    
    finish = clock();
    double cpu_time_used = (double(finish-start)/CLOCKS_PER_SEC);
    
    ll count = 0;
    for (int i=1; i<n; i+=2)
        if (!primes[i>>1])
            //cout << i << " ";
            count+=1;

    printf("\nLoop took %f seconds to execute. Found %llu primes\n", cpu_time_used, count);

    free(primes);
    return 0;
}
