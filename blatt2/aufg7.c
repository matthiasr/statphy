/* Statistische Physik I WiSe 2010/11 
 * */

/* Usage:
 * aufg7 m T v_min v_max steps
 * */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define K_B 1.3806504E-23

double maxwell(double v)
{
    return v*v*exp(-v*v);
}

int main(int argc, const char* argv[])
{
    if (argc != 6)
    {
        printf("Usage:\n\t%s m T v_min v_max steps\n",argv[0]);
        return 0;
    }

    long i;
    double sum = 0.0;

    /* Verwende numerische Einheiten v' = sqrt(m/(2*K_B*T)) * v */
    const double m = strtod(argv[1],(char**)NULL);
    const double T = strtod(argv[2],(char**)NULL);

    const double v_min = strtod(argv[3],(char**)NULL) * sqrt(m/(2*K_B*T));
    const double v_max = strtod(argv[4],(char**)NULL) * sqrt(m/(2*K_B*T));
    const long steps = strtol(argv[5],(char**)NULL,10);

    const double dv = (v_max - v_min) / steps;

#ifdef DEBUG
    printf("m=%g T=%g v_min=%g v_max=%g steps=%d dv=%g\n", \
            m,T,v_min,v_max,(int)steps,dv);
    printf("%g\n", sqrt(2*K_B*T/m));
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

    printf("%g\n",sum);

    return 0;
}
