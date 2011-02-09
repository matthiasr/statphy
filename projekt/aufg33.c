#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define BINS 1000
#define BURNIN 1000
#define SAMPLES 10000

#ifndef _THREAD_SAFE
#define __thread
#endif

#define PRNG_MAX 1
typedef double prng_t;
typedef uint64_t prng_state_t;

typedef struct { double x; double y; } vect;

__thread static prng_state_t s = 1;

static inline prng_t prng(void)
{
     s = 6364136223846793005LLU * s + 1442695040888963407LLU; /* implicit (mod) 2^64 */
    return (prng_t)(s >> 32)/UINT32_MAX;
}

/* number of particles */
static const size_t N = 400;
/* density */
static double RHO;
/* area side length = sqrt((double)N/RHO) */
static double L;

/* cutoff radius */
static const double RC = 2.5;

void set_rho(const double rho)
{
    RHO = rho;
    L = sqrt((double)N/rho);
}

/* wrap the coordinate X according to the minimum image convention
 * with regard to ref
 * assumes a box of side lengths L
 */
static inline vect wrap(vect r, const vect ref)
{
    while( 1 )
    {
        if( r.x - ref.x > 0.5*L )
        {
            r.x -= L;
        }
        else if( ref.x - r.x >= 0.5*L )
        {
            r.x += L;
        }
        else
        {
            break;
        }
    }
    while( 1 )
    {
        if( r.y - ref.y > 0.5*L )
        {
            r.y -= L;
        }
        else if( ref.y - r.y >= 0.5*L )
        {
            r.y += L;
        }
        else
        {
            break;
        }
    }
    return r;
}

static inline void rewrap_state(size_t N, vect* state)
{
    size_t i;
    const vect CENTER = { 0.5*L, 0.5*L };
    for(i=0;i<N;i++)
        state[i] = wrap(state[i], CENTER);
}

/* vector difference */
static inline vect direction(vect r0, vect r1)
{
    r1 = wrap(r1,r0);
    r0.x -= r1.x;
    r0.y -= r1.y;
    return r0;
}

/* wrapped distance */
static inline double dist(vect r0, const vect r1)
{
    r0 = direction(r0,r1);
    return sqrt(r0.x*r0.x + r0.y*r0.y);
}

int main(int argc, char* argv[])
{

}
