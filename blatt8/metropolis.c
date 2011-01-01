#include "metropolis.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

static inline void copy_state(const size_t N, const state_t* from, state_t* to)
{
    size_t i;
    for(i=0;i<N;i++) to[i] = from[i];
}

void metropolis_evolve_state(const size_t N, state_t* state, state_t* tempstate,\
        double (*p_accept)(const size_t N, const state_t* oldstate, const state_t* newstate))
{
    size_t i;

    /* evolve state */
    for(i=0;i<N;i++)
        tempstate[i] = state[i] + METROPOLIS_DELTA*((double)2*prng()/PRNG_MAX-1);

    if(prng() < PRNG_MAX*p_accept(N, state, tempstate))
        copy_state(N, tempstate, state);
}
