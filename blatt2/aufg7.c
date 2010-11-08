/* Statistische Physik I WiSe 2010/11 
 * */

/* Usage:
 * aufg7 m T p_min p_max steps
 * */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>


#define K_B 1.3806504E-23

/* Zu integrierende Verteilungsfunktion */
static inline long double maxwell(long double p)
{
    return p*p*exp(-p*p);
}

/* Hilfetext anzeigen und Programm beenden. Kehrt nie zurück. */
static inline void usage(const char* progname)
{
        fprintf(stderr,"Usage:\n\t%s m T p_min p_max steps\n\nAll values are in SI units.\n",progname);
        exit(1);
}

/* long-double-Wert vom Standardinput lesen */
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

/* long-Wert vom Standardinput lesen */
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

    /* Initialisierung */
    long i;
    long double sum = 0.0;

    long double m;
    long double T;

    long double p_min;
    long double p_max;
    long steps;

    if (argc == 6) /* Eingabe über Kommandozeile */
    {
        errno = 0;
        m = strtold(argv[1],(char**)NULL);
        T = strtold(argv[2],(char**)NULL);

        /* Verwende numerische Einheiten p' = p / sqrtl(2*m*K_B*T) */
        p_min = strtold(argv[3],(char**)NULL) / sqrtl(2*m*K_B*T);
        p_max = strtold(argv[4],(char**)NULL) / sqrtl(2*m*K_B*T);
        steps = strtol(argv[5],(char**)NULL,10);

        /* Fehlerchecks */
        if (errno != 0)
        {
            perror(argv[0]);
            usage(argv[0]);
        }
        else if (p_max < p_min)
        {
            fprintf(stderr, "Error: p_max < p_min\n");
            usage(argv[0]);
        }
        else if ( m <= 0 || T <= 0 || p_min < 0 || p_max < 0 || steps < 1)
        {
            fprintf(stderr, "Error: values too small\n");
            usage(argv[0]);
        }
    }
    else if (argc == 1) /* Eingabe über Standard-Input */
    {
        m = readvalue_ld("Mass [kg]");
        T = readvalue_ld("Temperature [K]");
        /* Verwende numerische Einheiten p' = p / sqrt(2*m*K_B*T)  */
        p_min = readvalue_ld("Minimum impulse [kg*m/s]") / sqrtl(2*m*K_B*T);
        p_max = readvalue_ld("Maximum impulse [kg*m/s]") / sqrtl(2*m*K_B*T);
        steps = readvalue_l("Number of steps");

        /* Fehlerchecks */
        if (p_max < p_min)
        {
            fprintf(stderr, "Error: p_max < p_min\n");
            usage(argv[0]);
        }
        else if ( m <= 0 || T <= 0 || p_min < 0 || p_max < 0 || steps < 1)
        {
            fprintf(stderr, "Error: values too small\n");
            usage(argv[0]);
        }
        printf("\n");
    }
    else /* Hilfetext sonst */
    {
        usage(argv[0]);
    }

    const long double dp = (p_max - p_min) / steps;

#ifdef DEBUG
    printf("m=%Lg T=%Lg p_min=%Lg p_max=%Lg steps=%d dp=%Lg\n", \
            m,T,p_min,p_max,(int)steps,dp);
    printf("Most probable impulse = %Lg\n", sqrtl(2*m*K_B*T));
#endif

    /* Integrationsschleife, mit OpenMP parallelisierbar */
#pragma omp parallel for default(shared) private(i) reduction(+:sum)
    for(i=0;i<steps;i++)
    {
#ifndef TRAPEZ
        /* Standard-Berechnung über Rechtecke */
        sum = sum + (dp * maxwell(p_min + i*dp));
#else
        /* Trapez-Methode. Genauer, aber langsamer */
        sum = sum + (0.5 * dp * (maxwell(p_min + i*dp) + maxwell(p_min + (i-1)*dp)));
#endif
    }

    /* Normierung des Ergebnisses */
    sum = sum * 4 / sqrtl(M_PI);

    printf("p = %.12Lg\n",sum);

    return 0;
}
