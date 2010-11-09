/* Statistische Physik I WiSe 2010/11 
 * */

/* Usage:
 * 
 * */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

typedef uint64_t lcgstate;

uint32_t lcg_random(lcgstate* s)
{
    /* Parameters from http://en.wikipedia.org/wiki/Linear_congruential_generator#LCGs_in_common_use */
    *s = 6364136223846793005LLU * (*s) + 1442695040888963407LLU; /* implicit (mod) 2^64 */
    return *s >> 32;
}

int main(int argc, const char* argv[])
{
    lcgstate s;
    unsigned long i;
    unsigned long count = 0;

    switch(argc) {
        case 3:
            s = strtoll(argv[2],NULL,10);
        case 2:
            count = strtol(argv[1],NULL,10);
            break;
        default:
            s = time(NULL);
            count = 1;
    }

    for (i=0;i<count;i++)
        printf("%u\n",lcg_random(&s));
    exit(0);
}
