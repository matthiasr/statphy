#include <stdio.h>
#include <stdlib.h>
#include "aufg16.h"

#ifndef N_STAT
#define N_STAT 1000
#endif

#ifndef MAX_SIZE
#define MAX_SIZE 1000
#endif

#ifndef DEFAULT_P_STEP
#define DEFAULT_P_STEP 0.1
#endif

int main(int argc, char** argv)
{
    int p;
    float p_step;
    int i;
    int size;
    int* f;
    int paths_found;

    if(argc == 1)
    {
        p_step = DEFAULT_P_STEP;
    }
    else
    {
        p_step = strtof(argv[1], NULL);
        if(p_step==0.0)
        {
            fprintf(stderr, "Usage:\n\t%s [p]\n\np: Probability (default: %g)\n", argv[0], DEFAULT_P_STEP);
            exit(1);
        }
    }

    for(size=10; size <= MAX_SIZE; size*=10)
    {
        f = malloc(size*size*sizeof(int));
        for(p=0; p<1.0/p_step; p++)
        {
            paths_found = 0;
            for(i=0; i<N_STAT; i++)
            {
                fill_array(f, size, p*p_step);
                paths_found += has_path(f, size);
            }

            printf("%f %g %d\n", p*p_step, (float)paths_found/N_STAT, paths_found);
        }
        printf("\n\n");
        free(f);
    }

    return 0;
}
