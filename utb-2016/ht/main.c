#include "ht.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct htable ht;
struct person p;
const char t[255];
const char k[255];

readperson()
{
    printf("Zadejte klic: ");
    if(!gets(k))
        return 0;
    printf("Jmeno: ");
    if(!gets(t))
        return 0;
    strcpy(p.name, t);
    printf("Vek: ");
    if(!gets(t))
        return 0;
    p.age = atof(t);
    printf("Vaha: ");
    if(!gets(t))
        return 0;
    p.weight = atof(t);
    printf("Vyska: ");
    if(!gets(t))
        return 0;
    p.height = atof(t);
    return 1;
}

int
readint()
{
    if(!gets(t))
        exit(1);
    return atoi(t);
}

void
printperson(struct person p)
{
    printf("Klic: %s\n", p.key);
    printf("-> Hodnota: %s, %.1f, %.1f, %.1f\n", p.name, p.age, p.weight, p.height);
}

main(void)
{
    ht.arr = 0;
    puts("Hash tabulka");
    puts("----------");
    puts("");
menu:
    puts("1 - HashTable_Init()");
    puts("2 - HashTable_Destruct()");
    puts("3 - HashTable_Insert()");
    puts("4 - HashTable_Delete()");
    puts("5 - HashTable_Find()");
    puts("6 - HashTable_Get_Count()");
    puts("7 - HashTable_Clear()");
    puts("8 - HashTable_Process()");
    puts("M - zobraz toto menu");
    puts("K - konec");
    puts("Pro ukonceni stiskni CTRL+D (Linux) nebo CTRL+Z (Windows).");
in:
    if(!gets(t))
        goto out;
    switch(t[0]) {
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
        if(ht.arr)
            break;
        puts("Tabulka neni inicializovana.");
        goto in;
    }
    switch(t[0]) {
    case '1':
        puts("Zadejte velikost tabulky (napr. 1000):");
        htfree(ht);
        ht = htinit(readint());
        goto in;
    case '3':
        if(!readperson())
            break;
        printf("hash: Key: %s\n", k);
        printf("-> Hash index: %d\n", htset(&ht, k, p));
        goto in;
    case '6':
        puts("Velikost tabulky:");
        printf("%d\n", ht.len);
        goto in;
    case '8':
        htwalk(ht, printperson);
        goto in;
    case 'M':
    case 'm':
        goto menu;
    }
out:
    htfree(ht);
    return 0;
}
