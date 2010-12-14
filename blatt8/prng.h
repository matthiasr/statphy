#ifndef PRNG_H_INCLUDED
#define PRNG_H_INCLUDED
#include <stdint.h>

#define PRNG_MAX 1
typedef double prng_t;
typedef uint64_t prng_state_t;

/* return a uniformly distributed random number from [0,PRNG_MAX) */
prng_t prng(void);

/* return a random number from [0,PRNG_MAX)
 * distributed according to the PDF.
 * PDF must map [0,PRNG_MAX) to [0,x) where x<=pdf_max
 * excessively large choice of pdf_max will result in low performance
 */
prng_t prng_pdf(prng_t (*pdf)(prng_t), prng_t pdf_max);

/* return the current internal state
 * state is thread-local if _THREAD_SAFE
 */
prng_state_t prng_getstate(void);

/* get the current internal state
 * state is thread-local if _THREAD_SAFE
 */
void prng_setstate(prng_state_t state);

#endif
