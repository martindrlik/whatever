#ifndef BT_H
#define BT_H

struct btdata
{
    char name[255];
    double age, weight, height;
};

struct btnode
{
    int key;
    struct btdata data;
    struct btnode *left;
    struct btnode *right;
};

void btfree(struct btnode *leaf);
void btcount(struct btnode *leaf, int *n);
void btinsert(int key, struct btnode **leaf, struct btdata data);
void btdelete(int key, struct btnode *leaf);

enum btmode { btpre, btin, btpost };
void btwalk(struct btnode *leaf, enum btmode mode, void (*fn)(struct btnode *leaf));
struct btnode *btsearch(int key, struct btnode *leaf);


#endif
