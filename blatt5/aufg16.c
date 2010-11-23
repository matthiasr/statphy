#include <stdlib.h>
#include <stdio.h>
#include "aufg16.h"

#define MAX_RANDOM 2147483647

/* füllt Array der Größe size*size (int[size][size])
 * mit der Wahrscheinlichkeit p mit 1, (1-p) mit 0 */
void fill_array(int* f, int size, float p)
{
    srandom(time(NULL));
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

int main(int argc, char** argv)
{
    return 0;
}
