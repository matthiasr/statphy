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
        fprintf(stderr,"Usage:\n\t%s m T v_min v_max steps\n\nAll values are in SI units.\n",progname);
        exit(1);
}

static inline long double readvalue_ld(const char* varname)
{
    long double temp;
    int res = 0;
    while(1)
    {
        errno = 0;
        printf("%s? ",varname);
        res = scanf("%Lg",&temp);
        if (res == EOF)
        {
            printf("Error: premature end of input");
            exit(1);
        }
        else if (res < 1) {
            printf("Error: could not read number");
            continue;
        }
        else
        {
            printf("%Lg\n",temp);
            return temp;
        }
    }
}

static inline long readvalue_l(const char* varname)
{
    long temp;
    int res = 0;
    while(1)
    {
        errno = 0;
        printf("%s? ",varname);
        res = scanf("%li",&temp);
        if (res == EOF)
        {
            printf("Error: premature end of input");
            exit(1);
        }
        else if (res < 1) {
            printf("Error: could not read number");
            continue;
        }
        else
        {
            printf("%li\n",temp);
            return temp;
        }
    }
}

int main(int argc, const char* argv[])
{

    long i;
    long double sum = 0.0;

    long double m;
    long double T;

    long double v_min;
    long double v_max;
    long steps;

    if (argc == 6)
    {
        errno = 0;
        /* Verwende numerische Einheiten v' = sqrt(m/(2*K_B*T)) * v */
         m = strtold(argv[1],(char**)NULL);
         T = strtold(argv[2],(char**)NULL);

         v_min = strtold(argv[3],(char**)NULL) * sqrt(m/(2*K_B*T));
         v_max = strtold(argv[4],(char**)NULL) * sqrt(m/(2*K_B*T));
         steps = strtol(argv[5],(char**)NULL,10);

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
        else if ( m <= 0 || T <= 0 || v_min < 0 || v_max < 0 || steps < 1)
        {
            fprintf(stderr, "Error: values too small\n");
            usage(argv[0]);
        }
    }
    else if (argc == 1)
    {
        m = readvalue_ld("Mass [kg]");
        T = readvalue_ld("Temperature [K]");
        v_min = readvalue_ld("Minimum velocity [m/s]") * sqrt(m/(2*K_B*T));
        v_max = readvalue_ld("Maximum velocity [m/s]") * sqrt(m/(2*K_B*T));
        steps = readvalue_l("Number of steps");
        if (v_max < v_min)
        {
            fprintf(stderr, "Error: v_max < v_min\n");
            usage(argv[0]);
        }
        else if ( m <= 0 || T <= 0 || v_min < 0 || v_max < 0 || steps < 1)
        {
            fprintf(stderr, "Error: values too small\n");
            usage(argv[0]);
        }
        printf("\n");
    }
    else
    {
        usage(argv[0]);
    }

    const long double dv = (v_max - v_min) / steps;

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

    printf("p = %Lg\n",sum);

    return 0;
}
