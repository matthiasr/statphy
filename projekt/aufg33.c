#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_thread_num() 0
#define omp_get_num_threads() 1
#endif

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

/* wrapped distance squared */
static inline double dist2(vect r0, const vect r1)
{
    r0 = direction(r0,r1);
    return (r0.x*r0.x + r0.y*r0.y);
}

/* wrapped distance */
static inline double dist(const vect r0, const vect r1)
{
    return sqrt(dist2(r0,r1));
}

/* calculate shifted Lennard-Jones force between particles
 * at r0 and r1 */
static inline vect lj_force(const vect r0, const vect r1)
{
    const double r = dist(r0,r1);
    vect d;
    if ( r > RC || r < 1E-6 )
    {
        d.x = d.y = 0;
    }
    else
    {
        const double r6 = r*r*r*r*r*r;
        const double r13 = r6*r6*r;
        const double RC6 = RC*RC*RC*RC*RC*RC;
        const double RC13 = RC6*RC6*RC;
        const double f = (24*r6-48)/r13 - (24*RC6-48)/RC13;
        d = direction(r0,r1);
        d.x *= f/r;
        d.y *= f/r;
    }
    return d;
}

/* calculate shifted Lennard-Jones potential between particles
 * at r0 and r1 */
static inline double lj_energy(const vect r0, const vect r1)
{
    double rr = dist2(r0,r1);
    if ( rr > RC*RC )
    {
        return 0;
    }
    else if ( rr < 1E-12 )
    {
        rr = 1E-12;
    }

    const double r6 = rr*rr*rr;
    const double r12 = r6*r6;
    const double RC6 = RC*RC*RC*RC*RC*RC;
    const double RC12 = RC6*RC6;

    return 4*(1./r12 - 1./r6) - 4*(1./RC12 - 1./RC6) - (sqrt(rr) - RC)*(24*RC6 - 48)/(RC12*RC);
}

/* Leapfrog integration */
static inline void lf_advance(const size_t N, vect* pos, vect* vel, const double dt)
{
    vect* accel = malloc(N*sizeof(vect));
    assert(accel!=NULL);
    long i,j;
    vect f;

    /* calculate accelerations */
#pragma omp parallel for private(i,j,f) shared(accel)
    for(i=0;i<N;i++)
    {
        accel[i].x = 0;
        accel[i].y = 0;
        for(j=0;j<N;j++)
            if(i!=j)
            {
                f = lj_force(pos[i],pos[j]);
                accel[i].x += f.x;
                accel[i].y -= f.y;
            }
    }

    /* advance velocities */
#pragma omp parallel for private(i) shared(accel,vel)
    for(i=0;i<N;i++)
    {
        vel[i].x += accel[i].x*dt;
        vel[i].y += accel[i].y*dt;
    }

    /* advance positions */
#pragma omp parallel for private(i) shared(pos,vel)
    for(i=0;i<N;i++)
    {
        pos[i].x += vel[i].x*dt;
        pos[i].y += vel[i].y*dt;
    }

    free(accel);
}

static inline double kinetic_energy(const size_t N, const vect* vel)
{
    long i;
    double e = 0;
#pragma omp parallel for private(i) reduction(+:e)
    for(i=0;i<N;i++)
        e += 0.5*(vel[i].x*vel[i].x + vel[i].y*vel[i].y);
    return e;
}

static inline double potential_energy(const size_t N, const vect* pos)
{
    long i,j;
    double e = 0, ee;
#pragma omp parallel for private(i,j,ee) reduction(+:e)
    for(i=0;i<N;i++)
    {
        ee = 0;
        for(j=0;j<N;j++)
            if(i!=j)
                ee += lj_energy(pos[i],pos[j]);
        e += ee;
    }

    return e;
}

static inline double total_energy(const size_t N, const vect* pos, const vect* vel)
{
    return potential_energy(N, pos) + kinetic_energy(N, vel);
}

int main(int argc, char* argv[])
{

}
