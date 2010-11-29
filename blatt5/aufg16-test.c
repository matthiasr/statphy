#include <stdio.h>
#include <stdlib.h>
#include "aufg16.h"

void print_array(int* f, int size)
{
    int i,j;
#ifdef DEBUG
    int sum=0;
#endif

    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            printf("%d ", f[i*size+j]);
#ifdef DEBUG
            sum += f[i*size+j];
#endif
        }
        printf("\n");
    }

#ifdef DEBUG
    printf("%d\n",sum);
#endif
}

int main(int argc, char** argv)
{
    int size = atoi(argv[1]);
    int* f;
    f = malloc(size*size*sizeof(int));
    float p = strtof(argv[2],NULL);

    fill_array((int*)f, size, p);

    print_array((int*)f, size);

    if(has_path((int *)f,size)) printf("Pfad gefunden!\n");

    return 0;
}
