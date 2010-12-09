#include "prng.h"
#include <stdio.h>
#include <stdlib.h>

prng_t pdf(prng_t x)
{
    return 2*(PRNG_MAX-x);
}

int main(int argc, char** argv)
{
    int i;
    for(i=0;i<1000000;i++)
        printf("%lf\n", prng_pdf(pdf,2));
    exit(0);
}
