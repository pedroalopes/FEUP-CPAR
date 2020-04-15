#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

int main (int argc, char *argv[])
{
    long long n;
    
    cout << "Power of 10: ";
    cin >> n;
 
    n = pow(10,n);
    
    bool *primes = new bool[n];
    
    long long k = 3;

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
    
    for (int i=1; i<n; i+=2)
        if (!primes[i>>1])
            cout << i << " ";
}
