#include "array.h"
#include <stdlib.h>

/* array_new initializes new array with cap; must be freed by array_free(); returns 0 if not enough memory. */
array *
array_new(int cap)
{
    array *ar;

    ar = malloc(sizeof(array));
    if(ar == 0)
        return 0;
    ar->p = malloc(cap * sizeof(int));
    if(ar->p == 0) {
        free(ar);
        return 0;
    }
    ar->cap = cap;
    ar->len = 0;
    return ar;
}

void
array_free(array *ar)
{
    free(ar->p);
    free(ar);
}

/* array_append adds n to ar and returns 0; returns 1 if not enough memory. */
int
array_append(array *ar, int n)
{
    if(ar->len == ar->cap) {
        int *nar;
        nar = (int*)realloc(ar->p, ar->cap*2 * sizeof(int));
        if(nar == 0)
            return 1;
        ar->p = nar;
        ar->cap = ar->cap*2;
    }
    ar->p[ar->len] = n;
    ar->len++;
    return 0;
}
