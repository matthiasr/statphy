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
static const double rho = 0.6;
/* area side length = sqrt((double)N/rho) */
static const vect L = {25.81988897,25.81988897};

/* wrap the coordinate X according to the minimum image convention
 * with regard to ref
 * assumes a box of side lengths L
 */
static inline vect wrap(vect r, const vect ref)
{
    while( 1 )
    {
        if( r.x - ref.x > 0.5*L.x )
        {
            r.x -= L.x;
        }
        else if( ref.x - r.x >= 0.5*L.x )
        {
            r.x += L.x;
        }
        else
        {
            break;
        }
    }
    while( 1 )
    {
        if( r.y - ref.y > 0.5*L.y )
        {
            r.y -= L.y;
        }
        else if( ref.y - r.y >= 0.5*L.y )
        {
            r.y += L.y;
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
    const vect CENTER = { 0.5*L.x, 0.5*L.y };
    for(i=0;i<N;i++)
        state[i] = wrap(state[i], CENTER);
}

/* wrapped distance */
static inline double dist(vect r0, const vect r1)
{
    r0 = wrap(r0,r1);
    double dx = r0.x - r1.x;
    double dy = r0.y - r1.y;
    return sqrt(dx*dx + dy*dy);
}

int main(int argc, char* argv[])
{

}
