#ifndef ARRAY_H
#define ARRAY_H

typedef struct {
    int cap, len;
    int *p;
} array;

/* array_new initializes new array with cap; must be freed by array_free(); returns 0 if not enough memory. */
array *array_new(int cap);
void array_free(array *ar);

/* array_append adds n to ar and returns 0; returns 1 if not not enough memory. */
int array_append(array *ar, int n);

#endif // ARRAY_H
