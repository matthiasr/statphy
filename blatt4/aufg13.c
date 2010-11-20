#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "aufg13.h"

#ifndef AUFG13_N
#define AUFG13_N 20
#endif

float gauss(float x)
{
    return exp(-x*x/2)/sqrt(2*M_PI);
}

int main(int argc, char **argv)
{
    srandom(AUFG13_N);

    /* swap */
    float a = random() % (10*AUFG13_N), b = random() % (10*AUFG13_N);

    printf("Swap:\na = %.0f, b = %.0f", a, b);
    swap (&a, &b);
    printf("  ==>  a = %.0f, b = %.0f\n\n", a, b);

    /* sort */
    float feld[AUFG13_N];
    int i;
    for (i=0;i<AUFG13_N;i++) feld[i] = random() % (10*AUFG13_N);

    printf("Sort:\n{ ");
    for (i=0;i<AUFG13_N-1;i++) printf("%.0f, ",feld[i]);
    printf("%.0f }\n==>\n{ ", feld[AUFG13_N-1]);

    sort(feld, AUFG13_N);

    for (i=0;i<AUFG13_N-1;i++) printf("%.0f, ",feld[i]);
    printf("%.0f }\n\n", feld[AUFG13_N-1]);

    /* integrieren */
    printf("Gauß-Integral, ±1 Standardabweichung: %f\n", integrieren(gauss, -1, 1, AUFG13_N));

    return 0;
}
