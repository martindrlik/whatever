#include "list.h"
#include <stdlib.h>
#include <string.h>

void
lstfree(struct list ls)
{
    struct lstnode *p, *q;
    for(p = ls.first; p != 0; p = q) {
        q = p->next;
        free(p);
    }
}

/* lstlen returns number of nodes stored in ls. */
lstlen(struct list ls)
{
    struct lstnode *n;
    int i;
    if(!ls.first)
        return 0;
    i = 0;
    for(n = ls.first; n; n = n->next, i++);
    return i;
}

/* lstfirst insert n at first to ls.
 * Returns 1 to indicate out of memory. */
lstinsfirst(struct list *ls, struct lstnode n)
{
    struct lstnode *p;
    p = malloc(sizeof(struct lstnode));
    if(!p)
        return 1;
    p->age = n.age;
    p->height = n.height;
    strcpy(p->name, n.name);
    p->weight =  n.weight;
    p->next = ls->first;
    ls->first = p;
    return 0;
}

/* lstafter inserts n after active.
 * Returns 1 to indicate out of memory. */
lstinsafter(struct list ls, struct lstnode n)
{
    struct lstnode *p;
    if(!ls.active)
        return 0;
    p = malloc(sizeof(struct lstnode));
    if(!p)
        return 1;
    p->age = n.age;
    p->height = n.height;
    strcpy(p->name, n.name);
    p->weight =  n.weight;
    p->next = ls.active->next;
    ls.active->next = p;
    return 0;
}

/* lstrmfirst removes first. */
void
lstrmfirst(struct list *ls)
{
    struct lstnode *p;
    if(!ls->first)
        return;
    if(ls->active == ls->first)
        ls->active = 0;
    p = ls->first->next;
    free(ls->first);
    ls->first = p;
}

/* lstrmafter removes after active. */
void
lstrmafter(struct list *ls)
{
    struct lstnode *p;
    if(!ls->active)
        return;
    if(!ls->active->next)
        return;
    p = ls->active->next->next;
    free(ls->active->next);
    ls->active->next = p;
}
