#include "metropolis.h"
#include <math.h>
#include <stdlib.h>

static state_t* tempstate = NULL;
static size_t tempstate_N = 0;

void metropolis_cleanup(void)
{
    if(tempstate != NULL)
    {
        free(tempstate);
        tempstate = NULL;
        tempstate_N = 0;
    }
}

static inline void copy_state(const size_t N, const state_t* from, state_t* to)
{
    size_t i;
    for(i=0;i<N;i++) to[i] = from[i];
}

static inline double p_accept(double deltaH)
{
    if(deltaH < 0)
    {
        return PRNG_MAX;
    }
    else
    {
        return PRNG_MAX*exp(-deltaH);
    }
}

void metropolis_evolve_state(const size_t N, state_t* state,\
        double (*Hamiltonian)(const size_t N, const state_t* state))
{
    const double H_old = Hamiltonian(N, state);
    size_t i;

    /* allocate tempstate if necessary */
    if(tempstate_N != N)
    {
        if(tempstate!=NULL) free(tempstate);
        tempstate = malloc(sizeof(state_t)*N);
        tempstate_N = N;
    }

    /* evolve state */
    for(i=0;i<N;i++)
        tempstate[i] = state[i] + METROPOLIS_DELTA*((double)2*prng()/PRNG_MAX-1);

    if(prng() <= p_accept(Hamiltonian(N, state) - H_old))
        copy_state(N, tempstate, state);
}
