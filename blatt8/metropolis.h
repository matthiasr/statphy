#ifndef METROPOLIS_H_INCLUDED
#define METROPOLIS_H_INCLUDED

#include "prng.h"
#include <stdlib.h>

/* maximale Schrittweite */
#ifndef METROPOLIS_DELTA
#define METROPOLIS_DELTA 1
#endif

typedef double state_t;

/* evolviert den N-dimensionalen Zustand state (Markov-Kette gemäß Hamiltonian */
void metropolis_evolve_state(const size_t N, state_t* state,\
        double (*Hamiltonian)(const size_t N, const state_t* state));

/* temporäre Speicher freigeben */
void metropolis_cleanup(void);

#endif
