#include <iostream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <papi.h>

using namespace std;

typedef long long ll;
unsigned int *primes;

void sieve(ll start, ll end)
{

    ll k = 3;

    do
    {
        ll x = (start - k * k) / (k << 1);
        if (x < 0)
            x = 0;
        for (long long j = k * k + 2 * k * x; j < end; j += k << 1)
        {
            primes[j >> 6] |= (1 << ((j >> 1) & 31));
        }

        do
        {
            k += 2;
        } while (k * k <= end && primes[k >> 6] & (1 << ((k >> 1) & 31)));

    } while (k * k <= end);
}

ll countPrimes(ll primes_size, ll n, ll ini, ll end)
{
    ll count = 0;
    for (int i = (ini >> 6) + 1; i < (end >> 6); i++)
    {
        count += 32 - __builtin_popcount(primes[i]);
    }
    int aux_ini = (1 << ((ini >> 1) & 31)) - 1;
    int aux_end = (1 << ((end >> 1) & 31)) - 1;

    if (!((ini >> 6) ^ (end >> 6)))
    {
        int aux = (~aux_ini) & aux_end;
        count += __builtin_popcount(aux) - __builtin_popcount(primes[ini >> 6] & (aux));
    }
    else
    {
        count += __builtin_popcount(~aux_ini) - __builtin_popcount(primes[ini >> 6] & (~aux_ini));
        count += __builtin_popcount(aux_end) - __builtin_popcount(primes[end >> 6] & (aux_end));
    }
    return count;
}

int main(int argc, char *argv[])
{
    ll n;
    int nr_threads = 1;

    if (argc >= 2)
    {
        n = 1LLU << (atoi(argv[1]));
        nr_threads = atoi(argv[2]);
    }
    else
    {
        cout << "Power of 10: ";
        cin >> n;

        n = pow(10, n);
    }

    // PAPI
    int EventSet = PAPI_NULL;
    ll values[2];
    int ret;

    ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT)
        cout << "FAIL" << endl;

    ret = PAPI_create_eventset(&EventSet);
    if (ret != PAPI_OK)
        cout << "ERRO: create eventset" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK)
        cout << "ERRO: PAPI_L1_DCM" << endl;

    ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK)
        cout << "ERRO: PAPI_L2_DCM" << endl;

    ret = PAPI_start(EventSet);
    if (ret != PAPI_OK)
        cout << "ERRO: Start PAPI" << endl;
    // PAPI

    ll total = 0;
    ll primes_size = (n >> 6) + 1;
    primes = new unsigned int[primes_size]();

    struct timespec start, end;

    ll block_size = n / nr_threads + 1;

    clock_gettime(CLOCK_REALTIME, &start);

#pragma omp parallel for schedule(static) num_threads(nr_threads) reduction(+ \
                                                                            : total)
    for (ll i = 0; i < n; i += block_size)
    {
        ll b_ini = i;
        ll b_end = i + block_size;
        b_end = b_end > n ? n : b_end;
        sieve(b_ini, b_end);
        total += countPrimes(primes_size, n, b_ini, b_end);
    }

    clock_gettime(CLOCK_REALTIME, &end);

    double time_spent = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Loop took %f seconds to execute. Found %llu primes\n", time_spent , total);

    free(primes);

    // PAPI
    ret = PAPI_stop(EventSet, values);
    if (ret != PAPI_OK)
        cout << "ERRO: Stop PAPI" << endl;

    //  printf("\tL1 DCM: %lld \n", values[0]);
    //  printf("\tL2 DCM: %lld \n", values[1]);


    ret = PAPI_reset(EventSet);
    if (ret != PAPI_OK)
        cout << "FAIL reset" << endl;

    ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
    if (ret != PAPI_OK)
        cout << "FAIL remove event" << endl;

    ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
    if (ret != PAPI_OK)
        cout << "FAIL remove event" << endl;

    ret = PAPI_destroy_eventset(&EventSet);
    if (ret != PAPI_OK)
        cout << "FAIL destroy" << endl;
    // PAPI
}
