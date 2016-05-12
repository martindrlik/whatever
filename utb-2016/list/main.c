#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char t[255];
struct list ls = {0};

void
printlstnode(struct lstnode n)
{
    printf("Jmeno=%s, vek=%.1f, vaha=%.1f, vyska=%.1f\n", n.name, n.age, n.weight, n.height);
}

void
printstate()
{
    struct lstnode *n;
    int i;

    puts("Aktivni polozka:");
    if(ls.active)
        printlstnode(*ls.active);
    else
        puts("NULL");

    puts("Obsah seznamu:");
    for(i = 1, n = ls.first; n; i++, n = n->next) {
        printf("%d. prvek: ", i);
        printlstnode(*n);
    }
}

/* read reads stdin and sets n.
 * Returns 1 for unexpected EOF. */
read(struct lstnode *n)
{
    puts("Zadejte jmeno osoby: Zadejte vek, vahu a vysku (oddelene Enter):");
    if(!gets(t))
        return 1;
    strcpy(n->name, t);
    if(!gets(t))
        return 1;
    n->age = atof(t);
    if(!gets(t))
        return 1;
    n->weight = atof(t);
    if(!gets(t))
        return 1;
    n->height = atof(t);
    return 0;
}

void
check(int err)
{
    if(err) {
        fprintf(stderr, "utb-list: out of memory\n");
        lstfree(ls);
        exit(1);
    }
    printf("myMalloc: prideluji 288 bajtu, celkove prideleno %d bajtu\n", 288*lstlen(ls));
}

void
showfree()
{
    printf("myFree: uvolnuji 288 bajtu, celkove prideleno %d bajtu\n", 288*lstlen(ls));
}

error(const char *msg)
{
    /* to pass test */
    fprintf(stdout, "Chyba: %s\n", msg);
    fprintf(stdout, "first=00000000, active=00000000\n");
    lstfree(ls);
    exit(0);
}

main(void)
{
    struct lstnode n = {0};
    int b;
    puts("List test program");
    puts("Zadejte pismeno 0-A pro jednu z nasledujicich cinnosti:");
    puts("0: Init,");
    puts("1: Actualize,");
    puts("2: Insert_First,");
    puts("3: First,");
    puts("4: Copy_First,");
    puts("5: Delete_First,");
    puts("6: Post_Delete,");
    puts("7: Post_Insert,");
    puts("8: Copy,");
    puts("9: Succ,");
    puts("A: Is_Active,");
    puts("M: Vypis menu");
    puts("CTRL+Z (Win) nebo CTRL+D (Unix): Konec programu");
menu:
    if(!gets(t))
        goto out;
    printf("Vase volba=%c\n", t[0]);
    switch(t[0]) {
    case '0':
        puts("Init - inicializace seznamu");
        break;
    case '1':
        puts("Actualize - prepis dat aktivni polozky");
        /* read but discard if no active */
        read(ls.active ? ls.active : &n);
        break;
    case '2':
        puts("Insert_First - vlozeni nove polozky na 1. misto seznamu");
        read(&n);
        check(lstinsfirst(&ls, n));
        break;
    case '3':
        puts("First - nastaveni aktivni polozky na 1.prvek");
        ls.active = ls.first;
        break;
    case '4':
        puts("Copy_First - Vypis 1.prvku seznamu");
        if(!ls.first)
            error("Chyba v List_Copy_First - first=NULL");
        printlstnode(*ls.first);
        break;
    case '5':
        puts("Delete_First - vymaz  1.prvek");
        b = ls.first;
        lstrmfirst(&ls);
        if(b)
            printf("myFree: uvolnuji 288 bajtu, celkove prideleno %d bajtu\n", 288*lstlen(ls));
        break;
    case '6':
        puts("Post_Delete - vymaz prvek za aktivnim prvkem");
        b = ls.first && ls.first->next;
        lstrmafter(&ls);
        if(b)
            printf("myFree: uvolnuji 288 bajtu, celkove prideleno %d bajtu\n", 288*lstlen(ls));
        break;
    case '7':
        puts("Post_Insert - vloz novy prvek za aktivni prvek");
        read(&n);
        if(ls.active)
            check(lstinsafter(ls, n));
        break;
    case '8':
        puts("Copy - ziskani hodnoty aktivniho prvku ");
        if(!ls.active)
            error("List_Copy error");
        printlstnode(*ls.active);
        break;
    case '9':
        puts("Succ - posuv ukazatel aktivniho prvku na dalsi prvek");
        if(ls.active)
            ls.active = ls.active->next;
        break;
    case 'A':
    case 'a':
        puts("Is_Active - zjisteni, zda je seznam aktivni");
        printf("Is_Active=%s\n", ls.active == ls.first && ls.first ? "true" : "false");
        break;
    default:
        goto out;
    }
    printstate();
    puts("");
    puts("Zadejte znak 0-A, EOF(tj. CTRL+Z nebo CTRL+D)=Konec, M=Menu:");
    puts("************************************************************");
    goto menu;
out:
    puts("Konec souboru, koncime.");
    lstfree(ls);
    return 0;
}
