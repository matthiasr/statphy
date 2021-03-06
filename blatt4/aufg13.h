#ifndef __HAVE_AUFG13_H
#define __HAVE_AUFG13_H

static void swap(float* f1, float* f2)
{
    const float temp = *f2;
    *f2 = *f1;
    *f1 = temp;
}

/* in-place quicksort */
static void sort(float* feld, int N)
{
    /* Abbruchbedingung: Einelementige Listen sind sortiert */
    if ( N <= 1 ) return;

    int pivot = N/2;
    int i = 0;
    float temp;

    /* Elemente, die kleiner als feld[pivot] sind dahinter verschieben */
    while ( i < pivot )
    {
        if ( feld[i] < feld[pivot] )
        {
            temp = feld[i];
            feld[i] = feld[pivot-1];
            feld[pivot-1] = feld[pivot];
            feld[pivot] = temp;
            pivot--;
        }
        else i++;
    }

    /* Elemente, die größer als feld[pivot] sind davor verschieben */
    while ( i < N )
    {
        if ( feld[i] > feld[pivot] )
        {
            temp = feld[i];
            feld[i] = feld[pivot+1];
            feld[pivot+1] = feld[pivot];
            feld[pivot] = temp;
            pivot++;
        }
        else i++;
    }

    /* Rekursion */
    sort(feld, pivot);
    sort(feld+pivot+1, N-pivot-1);

    return;
}

float integrieren(float (*integrand)(float), float von, float bis, int N)
{
    int i;
    float sum = 0.;

    const float delta = (bis - von) / N;

    /* Integrationsschleife, mit OpenMP parallelisierbar */
#pragma omp parallel for default(shared) private(i) reduction(+:sum)
    for(i=0;i<N;i++)
    {
#ifndef TRAPEZ
        /* Standard-Berechnung über Rechtecke */
        sum = sum + (delta * integrand(von + i*delta));
#else
        /* Trapez-Methode. Genauer, aber langsamer */
        sum = sum + (0.5 * delta * (integrand(von + i*delta) + integrand(von + (i-1)*delta)));
#endif
    }

    return sum;
}

#endif
