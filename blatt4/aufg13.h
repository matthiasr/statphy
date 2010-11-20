#ifndef __HAVE_AUFG13_H
#define __HAVE_AUFG13_H

static void swap(float* f1, float* f2)
{
    const float temp = *f2;
    *f2 = *f1;
    *f1 = temp;
}

#endif
