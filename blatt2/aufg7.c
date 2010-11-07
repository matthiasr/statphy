/* Statistische Physik I WiSe 2010/11 
 * */

/* Usage:
 * aufg7 m T v_min v_max steps
 * */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>


#define K_B 1.3806504E-23

static inline long double maxwell(long double v)
{
    return v*v*exp(-v*v);
}

static inline void usage(const char* progname)
{
        fprintf(stderr,"Usage:\n\t%s m T v_min v_max steps\n",progname);
        exit(1);
}

int main(int argc, const char* argv[])
{
    if (argc != 6)
    {
        usage(argv[0]);
    }

    errno = 0;
    long i;
    long double sum = 0.0;

    /* Verwende numerische Einheiten v' = sqrt(m/(2*K_B*T)) * v */
    const long double m = strtold(argv[1],(char**)NULL);
    const long double T = strtold(argv[2],(char**)NULL);

    const long double v_min = strtold(argv[3],(char**)NULL) * sqrt(m/(2*K_B*T));
    const long double v_max = strtold(argv[4],(char**)NULL) * sqrt(m/(2*K_B*T));
    const long steps = strtol(argv[5],(char**)NULL,10);

    const long double dv = (v_max - v_min) / steps;

    /* rudimentärer Fehlercheck */
    if (errno != 0)
    {
        perror(argv[0]);
        usage(argv[0]);
    }
    else if (v_max < v_min)
    {
        fprintf(stderr, "Error: v_max < v_min\n");
        usage(argv[0]);
    }

#ifdef DEBUG
    printf("m=%Lg T=%Lg v_min=%Lg v_max=%Lg steps=%d dv=%Lg\n", \
            m,T,v_min,v_max,(int)steps,dv);
    printf("%Lg\n", sqrtl(2*K_B*T/m));
#endif

#pragma omp parallel for default(shared) private(i) reduction(+:sum)
    for(i=0;i<steps;i++)
    {
#ifndef TRAPEZ
        sum = sum + (dv * maxwell(v_min + i*dv));
#else
        sum = sum + (0.5 * dv * (maxwell(v_min + i*dv) + maxwell(v_min + (i-1)*dv)));
#endif
    }

    sum = sum * 4 / sqrt(M_PI);

    printf("%Lg\n",sum);

    return 0;
}
