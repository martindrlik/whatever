#include "ht.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void
htfree(struct htable ht)
{
    free(ht.arr);
}

struct htable
htinit(int cap)
{
    struct htable ht;
    ht.cap = cap;
    ht.len = 0;
    ht.arr = calloc(cap, sizeof(struct person));
    if(!ht.arr) {
        printf(stderr, "ht: out of memory\n");
        exit(1);
    }
    return ht;
}

/* hash func */
index(const char *s)
{
    int i, j;
    j = 0;
    for(i = 0; i < 255; i++) {
        if(s[i] == '\0')
            break;
        j += s[i];
    }
    return j;
}

/* htset inserts p for kay to ht and returns index (hash). */
htset(struct htable *ht, const char *key, struct person p)
{
    int i;
    i = index(key) % ht->cap;
    if(!strlen(ht->arr[i].name))
        ht->len++;
    strcpy(ht->arr[i].key, key);
    strcpy(ht->arr[i].name, p.name);
    ht->arr[i].age = p.age;
    ht->arr[i].height = p.height;
    ht->arr[i].weight = p.weight;
    return i;
}

void
htwalk(struct htable ht, void (*fn)(struct person))
{
    int i;
    for(i = 0; i < ht.cap; i++) {
        if(strlen(ht.arr[i].name))
            fn(ht.arr[i]);
    }
}
