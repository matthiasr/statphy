/* Statistische Physik I WiSe 2010/11 
 * */

/* Usage:
 * 
 * */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

typedef uint64_t lcgstate;

uint32_t lcg_random(lcgstate* s)
{
    /* Parameters from http://en.wikipedia.org/wiki/Linear_congruential_generator#LCGs_in_common_use */
    *s = 6364136223846793005LLU * (*s) + 1442695040888963407LLU; /* implicit (mod) 2^64 */
    return (uint32_t)(*s >> 32);
}

/* Hilfetext anzeigen und Programm beenden. Kehrt nie zurück. */
static inline void usage(const char* progname)
{
        fprintf(stderr,"Usage:\n\n");
        fprintf(stderr,"Display <count> random numbers seeded with <seed>:\n\t%s [count [seed]]\n\n", \
                progname);
        fprintf(stderr, "Show this message:\n\t%s -h|--help\n", progname);
        exit(0);
}

int main(int argc, const char* argv[])
{
    if (argc == 2 && (strcmp("-h",argv[1]) == 0 || strcmp("--help",argv[1]) == 0 ))
    {
        usage(argv[0]);
    }
    else
    {
        lcgstate s;
        unsigned long i;
        unsigned long count = 0;

        switch(argc) {
            case 3:
                s = strtoll(argv[2],NULL,10);
                count = strtol(argv[1],NULL,10);
                break;
            case 2:
                count = strtol(argv[1],NULL,10);
                s = time(NULL);
                break;
            default:
                s = time(NULL);
                count = 1;
        }

        if (count < 1) usage(argv[0]);

        for (i=0;i<count;i++)
#ifdef DEBUG
            printf("%08x\n",lcg_random(&s));
#else
            printf("%.8f\n",lcg_random(&s) / exp2(32));
#endif
        exit(0);
    }
}
