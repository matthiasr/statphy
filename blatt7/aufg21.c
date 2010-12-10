#include "prng.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define N_TRIES 100000

prng_t rho1(prng_t x)
{
    return 2*(1 - x);
}

prng_t rho2(prng_t x)
{
    return 3*(1 - x*x)/2;
}

int main(int argc, char** argv)
{
    unsigned int i;
    unsigned int hits = 0;
    long double weighted_hits = 0;
    prng_t x,y;

    struct timeval t;
    gettimeofday(&t,NULL);
    prng_setstate(t.tv_usec+t.tv_sec*1000000);

    for(i=0;i<N_TRIES;i++)
    {
        x = prng();
        y = prng();
        if(x*x + y*y < 1)
            hits++;
    }
    printf("Uniform sampling: %lf\n", (double)4.*hits/N_TRIES);

    for(i=0;i<N_TRIES;i++)
    {
        x = prng_pdf(rho1, 2);
        y = prng_pdf(rho2, 4);
        if(x*x+y*y<1)
            weighted_hits += 1/(rho1(x)*rho2(y));
    }
    printf("Importance sampling: %Lf\n", 4.*weighted_hits/N_TRIES);

    exit(0);
}
