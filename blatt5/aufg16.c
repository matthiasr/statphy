#include <stdio.h>
#include <stdlib.h>
#include "aufg16.h"

#ifndef N_STAT
#define N_STAT 1000
#endif

#ifndef MAX_SIZE
#define MAX_SIZE 1000
#endif

int main(int argc, char** argv)
{
    float p;
    int i;
    int size;
    int* f;
    int paths_found;

    for(size=10; size <= MAX_SIZE; size*=10)
    {
        f = malloc(size*size*sizeof(int));
        for(p=0.0; p<1.0; p+=0.1)
        {
            paths_found = 0;
            for(i=0; i<N_STAT; i++)
            {
                fill_array(f, size, p);
                paths_found += has_path(f, size);
            }

            printf("%f %g %d\n", p, (float)paths_found/N_STAT, paths_found);
        }
        printf("\n");
        free(f);
    }

    return 0;
}
