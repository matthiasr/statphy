#include "prng.h"
#include "metropolis.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define DIM 2
#define BINS 100
#define BURNIN 1000
#define SAMPLES 10000
#define CHAINS 1

static const size_t N = 400*DIM;
static const double eta = 0.1;
static const double L = 56.04991216;

/* wrap the coordinate X according to the minimum image convention
 * with regard to ref
 * assumes a box of side length L
 */
static inline state_t wrap(state_t x, const state_t ref)
{
    while( 1 )
    {
        if( x - ref > 0.5*L )
        {
            x -= L;
        }
        else if( ref - x >= 0.5*L )
        {
            x += L;
        }
        else
        {
            break;
        }
    }
    return x;
}

static inline void rewrap_state(size_t N, state_t* state)
{
    size_t i;
    for(i=0;i<N;i++)
        state[i] = wrap(state[i], 0.5*L);
}

/* calculate the coordinate difference according to the minimum image convention */
static inline state_t dist(const state_t x0, const state_t x1)
{
    return x0 - wrap(x1,x0);
}

double p_accept(size_t N, const state_t* oldstate, const state_t* state)
{
    size_t i,j,k;
    double temp;

    /* check whether any two discs overlap */
    for(i=0;i<N/DIM;i++)
    {
        for(j=0;j<i;j++)
        {
            temp = 0;
            for(k=0;k<DIM;k++)
                temp += dist(state[DIM*i+k], state[DIM*j+k])*dist(state[DIM*i+k], state[DIM*j+k]);
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
        for(j=0;j<i;j++)
        {
            r_old = r_new = 0;
            for(k=0;k<DIM;k++)
            {
                r_new += dist(newstate[DIM*i+k], newstate[DIM*j+k])*dist(newstate[DIM*i+k], newstate[DIM*j+k]);
                r_old += dist(oldstate[DIM*i+k], oldstate[DIM*j+k])*dist(oldstate[DIM*i+k], oldstate[DIM*j+k]);
            }
            H_new += 1/(r_new*r_new*r_new*r_new*r_new*r_new);
            H_old += 1/(r_old*r_old*r_old*r_old*r_old*r_old);
        }
    }
    return exp(-1E9*(H_new - H_old ));
}

void bin_density(const size_t N, const state_t* state, unsigned int* rho)
{
/* FIXME: not usable for DIM!=2 */
#if DIM!=2
#error "bin_density not implemented for DIM!=2
#endif
    size_t i,j,k;
    size_t xbin,ybin;
    for(i=0;i<N/DIM;i++)
    {
        xbin = (int)(BINS*wrap(state[DIM*i],0.5*L)/L);
        ybin = (int)(BINS*wrap(state[DIM*i+1],0.5*L)/L);
        assert(xbin>=0 && xbin<BINS);
        assert(ybin>=0 && ybin<BINS);
        rho[xbin*BINS+ybin]++;
    }
}

state_t state_checksum(size_t N, const state_t* state)
{
    state_t r = 0;
    size_t i;
    for(i=0;i<N;i++)
        r += state[i];
    return r;
}

/* returns 0 if no valid state could be found */
int init_state(const size_t N, state_t* state, state_t* tempstate)
{
    int i=0,j;
    while(i<N)
    {
        for(j=0;j<DIM;j++)
            state[i*DIM+j] = L*prng();
        if(p_accept((i+1)*DIM,NULL,state)) i++;
    }

    /* burn in */
    for(i=0;i<BURNIN;i++)
    {
        metropolis_evolve_state(N, state, tempstate, p_accept);
        rewrap_state(N,state);
    }

    return 1;
}

int main(int argc, char** argv)
{
    size_t i,j,k,l;
    unsigned int* rho;  
    double* density;
    state_t *state, *tempstate;

    density = malloc(sizeof(double)*BINS*sizeof(double)*BINS);
    rho = malloc(sizeof(unsigned int)*BINS*sizeof(unsigned int)*BINS);
    state = malloc(sizeof(state_t)*N);
    tempstate = malloc(sizeof(state_t)*N);
    assert(density!=NULL && rho!=NULL && state!=NULL && tempstate!=0);

    printf("# N = %d, L = %f\n", (int)N/DIM, L);
    fflush(stdout);

    for(i=0;i<BINS*BINS;i++)
    {
        rho[i]=0;
        density[i] = 0;
    }

    for(l=0;l<CHAINS;l++)
    {
        if(!(init_state(N, state, tempstate)))
        {
            continue;
        }

        /* sampling */
        for(i=0;i<SAMPLES;i++)
        {
            metropolis_evolve_state(N, state, tempstate, p_accept);
            rewrap_state(N,state);
            bin_density(N, state, rho);
        }
    }

    /* calculate real local density */
    for(i=1;i<BINS*BINS;i++)
        density[i] = (double)rho[i] / (CHAINS*SAMPLES);

    for(i=1;i<BINS;i++)
        for(j=1;j<BINS;j++)
            printf("%lf %lf %lf %u\n", (double)i*L/(BINS), (double)j*L/(BINS), density[i*BINS+j], rho[i*BINS+j]);

    printf("\n\n");
    fflush(stdout);

    free(state);
    free(rho);
    free(density);

    exit(0);
}
