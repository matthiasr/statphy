#include "prng.h"

#ifndef _THREAD_SAFE
#define __thread
#endif

__thread static prng_state_t s = 1;

prng_t prng(void)
{
     s = 6364136223846793005LLU * s + 1442695040888963407LLU; /* implicit (mod) 2^64 */
    return (prng_t)(s >> 32)/UINT32_MAX;
}

prng_t prng_pdf(prng_t (*pdf)(prng_t), prng_t pdf_max)
{
    prng_t x,y;
    do
    {
        x = prng();
        y = prng()*pdf_max;
    } while (y > pdf(x));
    return x;
}

prng_state_t prng_getstate(void)
{
    return s;
}

void prng_setstate(prng_state_t state)
{
    s = state;
}
