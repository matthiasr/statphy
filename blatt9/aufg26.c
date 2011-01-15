#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define DIM 2
#define BINS 100
#define BURNIN 1000
#define SAMPLES 10000
#define CHAINS 1

#ifndef _THREAD_SAFE
#define __thread
#endif
/* maximale Schrittweite */
#ifndef METROPOLIS_DELTA
#define METROPOLIS_DELTA 1
#endif

#define PRNG_MAX 1
typedef double prng_t;
typedef uint64_t prng_state_t;
typedef double state_t;

__thread static prng_state_t s = 1;

static inline prng_t prng(void)
{
     s = 6364136223846793005LLU * s + 1442695040888963407LLU; /* implicit (mod) 2^64 */
    return (prng_t)(s >> 32)/UINT32_MAX;
}

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

static inline double p_accept(size_t N, const state_t* oldstate, const state_t* state)
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

static inline void copy_state(const size_t N, const state_t* from, state_t* to)
{
    size_t i;
    for(i=0;i<N;i++) to[i] = from[i];
}

static inline void metropolis_evolve_state(const size_t N, state_t* state, state_t* tempstate)
{
    size_t i;

    copy_state(N, state, tempstate);
    /* evolve state */
    for(i=0;i<N;i++)
    {
        tempstate[i] = state[i] + METROPOLIS_DELTA*((double)2*prng()/PRNG_MAX-1);
        if(prng() < PRNG_MAX*p_accept(N, state, tempstate))
        {
            state[i] = tempstate[i];
        }
        else
        {
            tempstate[i] = state[i];
        }
    }
}

static inline void bin_density(const size_t N, const state_t* state, unsigned int* rho)
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

static inline void bin_correlation(const size_t N, const state_t* state, unsigned int* g)
{
    size_t i,j,k;
    state_t r;
    for(i=0;i<N/DIM;i++)
        for(j=0;j<N/DIM;j++)
            {
                r = 0;
                for(k=0;k<DIM;k++)
                    r += dist(state[i*DIM+k],state[j*DIM+k])*dist(state[i*DIM+k],state[j*DIM+k]);
                r = sqrt(r);
                k = (size_t) (r * BINS / L);
                assert( 0 <= k && k < BINS);
                g[k]++;
            }

}

static inline state_t state_checksum(size_t N, const state_t* state)
{
    state_t r = 0;
    size_t i;
    for(i=0;i<N;i++)
        r += state[i];
    return r;
}

/* returns 0 if no valid state could be found */
static inline int init_state(const size_t N, state_t* state, state_t* tempstate)
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
        metropolis_evolve_state(N, state, tempstate);
        rewrap_state(N,state);
    }

    return 1;
}

int main(int argc, char** argv)
{
    size_t i,j,k,l;
    unsigned int* rho,*g;
    double* density,*correlation;
    state_t *state, *tempstate;

    density = malloc(sizeof(double)*BINS*sizeof(double)*BINS);
    rho = malloc(sizeof(unsigned int)*BINS*sizeof(unsigned int)*BINS);
    correlation = malloc(sizeof(double)*BINS);
    g = malloc(sizeof(unsigned int)*BINS);
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

    for(i=0;i<BINS;i++)
    {
        g[i] = 0;
        correlation[i] = 0;
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
            metropolis_evolve_state(N, state, tempstate);
            rewrap_state(N,state);
            bin_density(N, state, rho);
            bin_correlation(N,state,g);
        }
    }

    /* calculate real local density */
    for(i=1;i<BINS*BINS;i++)
        density[i] = (double)rho[i] / (CHAINS*SAMPLES);

    /* calculate correlation */
    for(i=1;i<BINS;i++)
        correlation[i] = (double)g[i] * pow(L,DIM) / ( 4*M_PI*(N/DIM)*(N/DIM) * (i*L/BINS)*(i*L/BINS) );

    for(i=1;i<BINS;i++)
        for(j=1;j<BINS;j++)
            printf("%lf %lf %lf %u\n", (double)i*L/(BINS), (double)j*L/(BINS), density[i*BINS+j], rho[i*BINS+j]);

    printf("\n\n");

    printf("# correlation\n");
    for(i=1;i<BINS;i++)
        printf("%lf %lf %d\n", (double)i*L/BINS, correlation[i], g[i]);
    fflush(stdout);

    free(state);
    free(rho);
    free(density);

    exit(0);
}
