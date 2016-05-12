#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

int min, max;
vector t, h;

void
checkptr(void *ptr)
{
    if(ptr)
        return;
    fprintf(stderr, "utb-line-histogram: out of memory\n");
    vecfree(&t);
    vecfree(&h);
    exit(1);
}

void
read(FILE *file, int delim)
{
    int i;
    t.len = 0;
    for(;;) {
        i = getc(file);
        vecadd(&t, i);
        if(i == EOF)
            break;
        if(i == delim)
            break;
    }
}

void
readfile()
{
    int i;
    char *n;
    FILE *f;
    n = malloc(t.len * sizeof(char));
    checkptr(n);
    /* t is /home/mdr/text.txt\n */
    /* n is /home/mdr/text.txt\0 */
    for(i = 0; i < t.len-1; i++)
        n[i] = t.p[i];
    n[t.len-1] = '\0';
    printf("Opening.. %s\n", n);
    f = fopen(n, "r");
    if(!f) {
        fprintf(stderr, "utb-line-histogram: can't read: %s\n", n);
        free(n);
        vecfree(&t);
        vecfree(&h);
        exit(1);
    }
    free(n);
    for(;;) {
        read(f, '\n');
        if(t.len-1 > 0) {
            vecadd(&h, t.len-1);
            if(min == -1 || min > t.len-1)
                min = t.len-1;
            if(max < t.len-1)
                max = t.len-1;
        }
        if(t.p[t.len-1] == EOF)
            break;
    }
    fclose(f);
}

struct range
{
    int from, to;
    int n;
};

void
histogram()
{
    int i, j;
    int boxn, boxs;
    struct range *r;
    char *n;
    if(h.len < 1)
        return;
    n = malloc(t.len * sizeof(char));
    checkptr(n);
    for(i = 0; i < t.len-1; i++)
        n[i] = t.p[i];
    n[t.len-1] = '\0';
    boxn = atoi(n);
    free(n);
    boxs = ((max-min)/boxn)+1;
    r = malloc(boxn * sizeof(struct range));
    checkptr(r);
    r[0].n = 0;
    r[0].from = 1;
    r[0].to = boxs;
    for(i = 1; i < boxn; i++) {
        r[i].n = 0;
        r[i].from = i*boxs + 1;
        r[i].to = (i+1) * boxs;
    }
    for(i = 0; i < h.len; i++) {
        for(j = 0; j < boxn; j++) {
            if(r[j].from > h.p[i])
                continue;
            if(r[j].to < h.p[i])
                continue;
            r[j].n++;
        }
    }
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    printf("Histogram....\n");
    printf("%-20s %-20s %-20s\n", "[From]", "[To]", "[Count]");
    for(i = 0; i < boxn; i++) {
        printf("%-20d %-20d %-20d\n", r[i].from, r[i].to, r[i].n);
    }
    free(r);
}

int
main(void)
{
    puts("Histogram");
    t = vecmake(100);
    h = vecmake(100);
    min = -1;
    max = -1;
menu:
    puts("Stisknete:");
    puts("1 pro nacteni noveho souboru");
    puts("2 pro vypis histogramu");
    puts("3 pro vymazani obsahu vektoru");
    puts("cokoli jineho pro konec.");
    read(stdin, '\n');
    switch(t.p[0]) {
    case '1':
        puts("Vlozte absolutni nebo relativni cestu k souboru:");
        read(stdin, '\n');
        readfile();
        goto menu;
    case '2':
        puts("Zadejte pocet boxu histogramu:");
        read(stdin, '\n');
        histogram();
        goto menu;
    case '3':
        min = -1;
        max = -1;
        h.len = 0;
        goto menu;
    }
    vecfree(&t);
    vecfree(&h);
    return 0;
}
