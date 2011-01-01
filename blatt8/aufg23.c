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

double p_accept_runin(size_t N, const state_t* oldstate, const state_t* newstate)
{
    /* always accept if newstate is valid */
    if(p_accept(N,oldstate,newstate)) return 1;

    /* drive towards a valid state */
    double H_old = 0, H_new = 0;
    double r_old, r_new;
    size_t i,j,k;
    for(i=0;i<N/DIM;i++)
    {
        r_old = r_new = 0;
        for(k=0;k<DIM;k++)
        {
                r_new += newstate[DIM*i+k]*newstate[DIM*i+k];
                r_old += oldstate[DIM*i+k]*oldstate[DIM*i+k];
        }

        H_new += r_new*r_new*r_new*r_new;
        H_old += r_old*r_old*r_old*r_old;

        for(j=0;j<i;j++)
        {
            r_old = r_new = 0;
            for(k=0;k<DIM;k++)
            {
                r_new += ((newstate[DIM*i+k]-newstate[DIM*j+k])*(newstate[DIM*i+k]-newstate[DIM*j+k]));
                r_old += ((oldstate[DIM*i+k]-oldstate[DIM*j+k])*(oldstate[DIM*i+k]-oldstate[DIM*j+k]));
            }
            H_new += 1/(r_new*r_new*r_new*r_new*r_new*r_new);
            H_old += 1/(r_old*r_old*r_old*r_old*r_old*r_old);
        }
    }
    return exp(-1E9*(H_new - H_old ));
}

void bin_density(const size_t N, const state_t* state, unsigned int rho[BINS])
{
    size_t i,j,k;
    double rr;
    for(i=0;i<N/DIM;i++)
    {
        rr = 0;
        for(j=0;j<DIM;j++)
        {
            rr += state[DIM*i+j]*state[DIM*i+j];
        }
        rr = sqrt(rr);
        k = lrint(floor(2*BINS*rr/lambda));
        assert(k>=0 && k<BINS);
        rho[k]++;
    }

}

/* returns 0 if no valid state could be found */
int init_state(const size_t N, state_t* state, state_t* tempstate)
{
    int i;
    /* initial state */
    for(i=0;i<N;i++)
        state[i] = 0.5*(lambda-1)*((double)2*prng()/PRNG_MAX-1);
    do {
        metropolis_evolve_state(N,state, tempstate, p_accept_runin);
    } while(p_accept(N,NULL,state) == 0);

    /* burn in */
    for(i=0;i<BURNIN;i++)
        metropolis_evolve_state(N, state, tempstate, p_accept);

    return 1;
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

            printf("# N = %d, lambda = %f\n", (int)NS[j], lambda);
            fflush(stdout);

            for(i=0;i<BINS;i++)
            {
                rho[i]=0;
                density[i] = 0;
            }

            for(l=0;l<CHAINS;l++)
            {
                if(!init_state(N, state, tempstate)) continue;

                /* sampling */
                for(i=0;i<SAMPLES;i++)
                {
                    metropolis_evolve_state(N, state, tempstate, p_accept);
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
