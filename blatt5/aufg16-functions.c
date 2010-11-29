#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "aufg16.h"

#ifndef USE_STDLIB_RANDOM
#define MAX_RANDOM UINT32_MAX
#define RANDOM lcg_random
#else
#define MAX_RANDOM 2147483647
#define RANDOM random
#endif

uint32_t lcg_random(void)
{
    static uint64_t s = 294857623; /* completely random seed: chosen by fair dice roll */
    /* Parameters from http://en.wikipedia.org/wiki/Linear_congruential_generator#LCGs_in_common_use */
    s = 6364136223846793005LLU * s + 1442695040888963407LLU; /* implicit (mod) 2^64 */
    return (uint32_t)(s >> 32);
}


/* füllt Array der Größe size*size (int[size][size])
 * mit der Wahrscheinlichkeit p mit 1, (1-p) mit 0 */
void fill_array(int* f, const int size, const float p)
{
    int i;
    for(i=0; i<size*size; i++)
    {
        if(RANDOM() < p*MAX_RANDOM)
        {
            f[i] = 1;
        }
        else
        {
            f[i] = 0;
        }
    }
}

int find_path_from_pos(const int* f, int* visited, const int size, \
        const int xpos, const int ypos, const int xfrom, const int yfrom)
{
#ifdef DEBUG
    printf("%d %d\n", xpos, ypos);
#endif
    if( xpos == size-1 ) return 1; /* am Ziel */
    if( f[xpos*size+ypos] == 0 || visited[xpos*size+ypos] != 0 ) return 0; /* kein Pfad hier */

    visited[xpos*size+ypos] = 1; /* loop prevention */

    int xdelta, ydelta, ergebnis=0;
    for( xdelta=1; xdelta>=-1; xdelta--)
        for( ydelta=1; ydelta>=-1; ydelta--)
        {
            if ( xpos+xdelta<0 || ypos+ydelta >= size || ypos+ydelta<0 )
                return 0; /* out of bounds - kein Pfad hier */

            ergebnis = find_path_from_pos(f, visited, size, \
                    xpos+xdelta, ypos+ydelta, xfrom, yfrom);
            if(ergebnis)
                return ergebnis;
        }
    return 0;
}

/* Sucht size*size-Feld f nach einem Pfad von links nach rechts ab. */
int has_path(const int* f, const int size)
{
    int ystart;
    int ergebnis;
    int* visited;

    visited = malloc(size*size*sizeof(int));

    if(visited == NULL)
    {
        fprintf(stderr, "Memory Error");
        exit(1);
    }

    int i;
    for(i=0;i<size*size;i++)
        visited[i] = 0;

    for(ystart=0;ystart<size;ystart++)
    {
        ergebnis = find_path_from_pos(f, (int*)visited, size, 0, ystart, -1, -1);
        if(ergebnis)
        {
            free(visited);
            return 1; /* Pfad gefunden */
        }
    }
    free(visited);
    return 0; /* kein Pfad */
}
