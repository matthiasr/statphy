#include "prng.h"
#include "metropolis.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define DIM 2
#define BINS 100
#define BURNIN 100000
#define SAMPLES 10000000
#define CHAINS 10

#define N_LAMBDAS 4
static const double LAMBDAS[N_LAMBDAS] = {1.5, 2, 5, 10};
#define N_NS 4
static const double NS[N_NS] = {1, 3, 10, 100};
#define MAX_NS 100

static double lambda;

double p_accept(size_t N, const state_t* oldstate, const state_t* state)
{
    size_t i,j,k;
    double temp;
    /* check whether any disc exceeds the bounds */
    for(i=0;i<N/DIM;i++)
    {
        temp = 0;
        for(j=0;j<DIM;j++)
            temp += state[DIM*i+j]*state[DIM*i+j];
        if(temp >= 0.25*(lambda-1)*(lambda-1)) return 0;
    }

    /* check whether any two discs overlap */
    for(i=0;i<N/DIM;i++)
    {
        for(j=0;j<i;j++)
        {
            temp = 0;
            for(k=0;k<DIM;k++)
                temp += (state[DIM*i+k]-state[DIM*j+k])*(state[DIM*i+k]-state[DIM*j+k]);
            if(temp < 1) return 0;
        }
    }

    return 1;
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
    size_t i,j,k,l;
    size_t N;
    unsigned int* rho;  
    double* density;
    state_t *state, *tempstate;

    density = malloc(sizeof(double)*BINS);
    rho = malloc(sizeof(unsigned int)*BINS);
    state = malloc(sizeof(state_t)*MAX_NS*DIM);
    tempstate = malloc(sizeof(state_t)*MAX_NS*DIM);
    assert(density!=NULL && rho!=NULL && state!=NULL && tempstate!=0);
    for(j=0;j<N_NS;j++)
    {
        N = NS[j]*DIM;
        for(k=0;k<N_LAMBDAS;k++)
        {
            lambda = LAMBDAS[k] * pow(N/DIM,1./DIM);

            printf("# N = %d, lambda = %f\n", (int)NS[j], LAMBDAS[k]);
            fflush(stdout);

            for(i=0;i<BINS;i++)
            {
                rho[i]=0;
                density[i] = 0;
            }

            for(l=0;l<CHAINS;l++)
            {
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
            }

            /* correct local density for size of (DIM-1)-sphere */
            for(i=1;i<BINS;i++)
                density[i] = (double)rho[i] * pow(((double)2*BINS/( CHAINS*SAMPLES )) / (i*lambda), DIM-1);

            for(i=1;i<BINS;i++)
                printf("%lf %lf\n", (double)i*lambda/(2*BINS), density[i]);

            printf("\n\n");
            fflush(stdout);
        }
    }

    free(state);
    free(rho);
    free(density);

    exit(0);
}
