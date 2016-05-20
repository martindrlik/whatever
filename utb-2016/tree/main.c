#include "bt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char t[255];

void
printleaf(struct btnode *leaf)
{
    printf("Jmeno=%s, vek=%.1f, vaha=%.1f, vyska=%.1f", leaf->data.name, leaf->data.age, leaf->data.weight, leaf->data.height);
}

void
printree(struct btnode *leaf)
{
    printleaf(leaf);
    puts("");
    if(leaf->left) {
        printf("| L -> ");
        printleaf(leaf->left);
        puts("");
    }
    else
        printf("| L -> NULL ");
    if(leaf->right) {
        printf("| R -> ");
        printleaf(leaf->right);
        puts("");
    }
    else
        printf("| R -> NULL\n");
}

void
read(struct btdata *d)
{
    puts("Data uzlu stromu:");
    printf("Zadejte jmeno osoby: ");
    if(!gets(t))
        return;
    strcpy(d->name, t);
    puts("Zadejte vek, vahu a vysku (oddelene Enter):");
    if(!gets(t))
        return;
    d->age = atof(t);
    if(!gets(t))
        return;
    d->weight = atof(t);
    if(!gets(t))
        return;
    d->height = atof(t);
}

main(void)
{
    int initilized, n, key;
    struct btnode *r;
    struct btdata d;
    initilized = 0;
    r = 0;
    puts("Strom v1.0");
    puts("----------");
    puts("");
printmenu:
    puts("1 - tree_init()");
    puts("2 - tree_clear()");
    puts("3 - tree_insert()");
    puts("4 - tree_delete()");
    puts("5 - tree_find_node()");
    puts("6 - tree_get_node_count()");
    puts("7 - tree_proces()");
    puts("M - zobraz toto menu");
    puts("K - konec");
    puts("Pro ukonceni stiskni CTRL+D (Linux) nebo CTRL+Z (Windows).");
menu:
    if(!gets(t))
        goto out;
    puts("Vase volba:");
    printf("%s\n", t);
    switch(t[0]) {
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
        if(initilized)
            break;
        puts("Strom neni inicializovan!");
        goto menu;
    }
    switch(t[0]) {
    case '1':
        initilized = 1;
        puts("Strom byl inicializovan.");
        break;
    case '2':
        initilized = 0;
        btfree(r);
        r = 0;
        puts("Obsah stromu byl odstranen.");
        break;
    case '3':
        read(&d);
        btinsert(key, &r, d);
        break;
    case '6':
        n = 0;
        btcount(r, &n);
        printf("Pocet uzlu stromu: %d\n", n);
        break;
    case '7':
        puts("Pruchod:");
        puts("1 - preorder");
        puts("2 - inorder");
        puts("3 - postorder");
        if(!gets(t))
            goto out;
        switch(t[0]) {
        case '2':
            btwalk(r, btpre, printree);
            break;
        }
        break;
    case 'M':
    case 'm':
        goto printmenu;
    default:
        goto out;
    }
    goto menu;
out:
    btfree(r);
    return 0;
}
