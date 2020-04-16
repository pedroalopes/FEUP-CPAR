#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>

using namespace std;

int main (int argc, char *argv[])
{
    long long n;
    
    cout << "Power of 10: ";
    cin >> n;
 
    n = pow(10,n);
    
    bool *primes = new bool[n];
    
    long long k = 3;

    clock_t start, finish;
    start = clock();

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

    finish = clock();
    double cpu_time_used = (double(finish-start)/CLOCKS_PER_SEC);
    
    unsigned long long count = 0;
    for (int i=1; i<n; i+=2)
        if (!primes[i>>1])
            //cout << i << " ";
            count+=1;

    printf("\nLoop took %f seconds to execute. Found %llu primes\n", cpu_time_used, count);
}
