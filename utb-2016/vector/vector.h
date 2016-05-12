#ifndef VECTOR_H
#define VECTOR_H

/* note that vector must be always initialized,
 * by vecmake() for instance. */
typedef struct s_vector
{
    int cap, len;
    int *p;
}
vector;

typedef struct s_vecval
{
    int v;
    int ok;
}
vecval;

/* vecmake initializes vector of capacity cap.
 * Must be freed by vecfree(). */
vector vecmake(int cap);

/* vecfree sets cap and len to 0 and frees memory allocated by vec. */
void vecfree(vector *vec);

/* vecadd appends v to vec. */
void vecadd(vector *vec, int v);

/* vecdel removes value at index i in vec. */
void vecdel(vector *vec, int i);

/* vecat returns vecval at index i. ok and v is 0 if no value at i;
 * otherwise ok is 1 and v is stored value. */
vecval vecat(vector *vec, int i);

/* vecindexof returns index of first v since start in vec; -1 if no v in vec. */
vecindexof(vector *vec, int start, int v);

#endif // VECTOR_H
