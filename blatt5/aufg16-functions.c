#include <stdlib.h>
#include <stdio.h>
#include "aufg16.h"

#define MAX_RANDOM 2147483647

/* füllt Array der Größe size*size (int[size][size])
 * mit der Wahrscheinlichkeit p mit 1, (1-p) mit 0 */
void fill_array(int* f, const int size, const float p)
{
    int i;
    for(i=0; i<size*size; i++)
    {
        if(random() < p*MAX_RANDOM)
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
