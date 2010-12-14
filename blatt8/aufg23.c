#include "prng.h"
#include "metropolis.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DIM 2
#define BINS 100
#define BURNIN 100000
#define SAMPLES 100000000

static double lambda;

double H(size_t N, const state_t* state)
{
    size_t i,j,k;
    double temp;
    /* check whether any disc exceeds the bounds */
    for(i=0;i<N/DIM;i++)
    {
        temp = 0;
        for(j=0;j<DIM;j++)
            temp += state[DIM*i+j]*state[DIM*i+j];
        if(temp > 0.25*(lambda-1)*(lambda-1)) return INFINITY;
    }

    /* check whether any two discs overlap */
    for(i=0;i<N/DIM;i++)
    {
        for(j=0;j<N/(2*DIM);j++)
        {
            temp = 0;
            for(k=0;k<DIM;k++)
                temp += (state[DIM*i+k]-state[DIM*j+k])*(state[DIM*i+k]-state[DIM*j+k]);
            if(temp < 1) return INFINITY;
        }
    }

    return 0;
}

void bin_density(const size_t N, const state_t* state, unsigned int rho[BINS])
{
    size_t i,j;
    double rr;
    for(i=0;i<N/DIM;i++)
    {
        rr = 0;
        for(j=0;j<DIM;j++)
        {
            rr += state[DIM*i+j]*state[DIM*i+j];
        }
        rr = sqrt(rr);
        rho[lrint(2*BINS*rr/lambda)]++;
    }

}

int main(int argc, char** argv)
{
    size_t i;
    size_t N = 1*DIM;
    unsigned int* rho;  
    double* density;
    state_t* state;

    density = malloc(sizeof(double)*BINS);
    rho = malloc(sizeof(unsigned int)*BINS);
    state = malloc(sizeof(state_t)*N);
    for(i=0;i<BINS;i++)
    {
        rho[i]=0;
        density[i] = 0;
    }

    lambda = 10;

    /* initial state */
    for(i=0;i<N;i++)
        state[i] = 0.5*(lambda-1)*((double)2*prng()/PRNG_MAX-1);

    /* burn in */
    for(i=0;i<BURNIN;i++)
        metropolis_evolve_state(N, state, H);

    /* sampling */
    for(i=0;i<SAMPLES;i++)
    {
        metropolis_evolve_state(N, state, H);
        bin_density(N, state, rho);
    }

    free(state);

    /* correct local density for size of (DIM-1)-sphere */
    for(i=1;i<BINS;i++)
        density[i] = (double)rho[i] * pow(((double)2*BINS/SAMPLES) / (i*lambda), DIM-1);

    free(rho);

    for(i=1;i<BINS;i++)
        printf("%lf %lg\n", (double)i/(2*lambda), density[i]);

    free(density);

    exit(0);
}
