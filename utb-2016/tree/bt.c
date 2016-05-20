#include "bt.h"
#include <stdlib.h>
#include <string.h>

void
btfree(struct btnode *leaf)
{
    if(!leaf)
        return;
    btfree(leaf->left);
    btfree(leaf->right);
    free(leaf);
}

void
btcount(struct btnode *leaf, int *n)
{
    if(!leaf)
        return;
    *n++;
    btcount(leaf->left, n);
    btcount(leaf->right, n);
}

void
btinsert(int key, struct btnode **leaf, struct btdata data)
{
    if(!*leaf) {
        *leaf = malloc(sizeof(struct btnode));
        strcpy((*leaf)->data.name,  data.name);
        (*leaf)->data.age = data.age;
        (*leaf)->data.weight = data.weight;
        (*leaf)->data.height = data.height;
        (*leaf)->left = 0;
        (*leaf)->right = 0;
    }
    else if(key < (*leaf)->key)
        btinsert(key, &(*leaf)->left, data);
    else if(key > (*leaf)->key)
        btinsert(key, &(*leaf)->right, data);
}

void
btdelete(int key, struct btnode *leaf)
{
}

void
btwalk(struct btnode *leaf, enum btmode mode, void (*fn)(struct btnode *leaf))
{
    if(!leaf)
        return;
    if(mode == btpre)
        fn(leaf);
    btwalk(leaf->left, mode, fn);
    if(mode == btin)
        fn(leaf);
    btwalk(leaf->right, mode, fn);
    if(mode == btpost)
        fn(leaf);
}

struct btnode *
btsearch(int key, struct btnode *leaf)
{
    if(!leaf)
        return 0;
    if(key == leaf->key)
        return leaf;
    else if(key < leaf->key)
        return btsearch(key, leaf->left);
    else
        return btsearch(key, leaf->right);
}
