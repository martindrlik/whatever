#ifndef LIST_H
#define LIST_H

struct lstnode
{
    const char name[255];
    double age, weight, height;
    struct lstnode *next;
};

/* list must be freed by lstfree(). */
struct list
{
    struct lstnode *first;
    struct lstnode *active;
};

void lstfree(struct list);

/* lstlen returns number of nodes stored in ls. */
lstlen(struct list ls);

/* lstfirst inserts n at first.
 * Returns 1 to indicate out of memory. */
lstinsfirst(struct list *ls, struct lstnode n);
/* lstafter inserts n after active.
 * Returns 1 to indicate out of memory. */
lstinsafter(struct list ls, struct lstnode n);

/* lstrmfirst removes first. */
void lstrmfirst(struct list *ls);
/* lstrmafter removes after active. */
void lstrmafter(struct list *ls);

#endif // LIST_H
