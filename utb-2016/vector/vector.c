#include "vector.h"
#include <stdlib.h>

/* vecmake initializes vector of capacity cap. */
vector
vecmake(int cap)
{
    vector vec;
    vec.len = 0;
    if(cap > 0) {
        vec.p = malloc(cap * sizeof(int));
        vec.cap = vec.p == 0 ? 0 : cap;
    }
    else {
        vec.p = 0;
        vec.cap = 0;
    }
    return vec;
}

/* vecfree frees memory allocated by vec. */
void
vecfree(vector *vec)
{
    vec->cap = 0;
    vec->len = 0;
    free(vec->p);
}

/* vecadd appends v to vec. */
void
vecadd(vector *vec, int v)
{
    int *p;
    if(vec->cap < 1) {
        vec->p = malloc(sizeof(int));
        vec->cap = vec->p == 0 ? 0 : 1;
    }
    else if(vec->len == vec->cap) {
        p = realloc(vec->p, vec->cap * 2 * sizeof(int));
        if(p != 0) {
            vec->p = p;
            vec->cap = vec->cap * 2;
        }
    }
    if(vec->cap > 0) {
        vec->p[vec->len] = v;
        vec->len++;
    }
}

/* vecdel removes value at index i in vec. */
void
vecdel(vector *vec, int i)
{
    int j;
    if(i < 0)
        return;
    if(i >= vec->len)
        return;
    for(j = i; j < vec->len-1; j++)
        vec->p[j] = vec->p[j+1];
    vec->len--;
}

/* vecat returns vecval at index i. ok and v is 0 if no value at i;
 * otherwise ok is 1 and v is stored value. */
vecval
vecat(vector *vec, int i)
{
    vecval v;
    v.ok = 0;
    v.v = 0;
    if(i < 0)
        return v;
    if(i >= vec->len)
        return v;
    v.ok = 1;
    v.v = vec->p[i];
    return v;
}

/* vecindexof returns index of first v since start in vec; -1 if no v in vec. */
vecindexof(vector *vec, int start, int v)
{
    int i;
    if(start < 0)
        start = 0;
    for(i = start; i < vec->len; i++) {
        if(vec->p[i] == v)
            return i;
    }
    return -1;
}
