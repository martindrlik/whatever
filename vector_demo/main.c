#include "vector.h"
#include <stdio.h>
#include <assert.h>

vector vec;

/* is_newline indicates if ch is new line. */
is_newline(int ch)
{
    if(ch == '\n')
        return 1;
    return ch == '\r';
}

/* eat likes CR or LF and eats one from stdin. */
void
eat()
{
    int ch;
    ch = getchar();
    if(!is_newline(ch))
        ungetc(ch, stdin);
}

/* single returns number created from each digit stored in vec.
 * For vec storing [4, 1, 2, 3] it returns 4123. */
single(vector vec)
{
    int i, j, m, n;

    m = 0;
    for(i = 0; i < vec.len; i++) {
        n = vec.p[i];
        for(j = 0; j < vec.len-i-1; j++)
            n *= 10;
        m += n;
    }
    return m;
}

void
test_single()
{
    vector vec;
    vec = vecmake(4);
    vecadd(&vec, 4);
    vecadd(&vec, 1);
    vecadd(&vec, 2);
    vecadd(&vec, 3);
    assert(single(vec) == 4123);
}

enum yums { yumok, yumnan, yumeof, };
typedef struct s_yumv
{
    enum yums state;
    int v;
}
yumv;

/* yum reads integer from stdin; check state. */
yumv
yum()
{
    int ch;
    vector vec;
    vec = vecmake(0);
    yumv v;
    v.state = yumnan;
    v.v = 0;
    for(;;) {
        ch = getchar();
        switch(ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            vecadd(&vec, ch-48);
            break;
        case '\n':
        case '\r':
            if(vec.len > 0)
                v.state = yumok;
            v.v = single(vec);
            return v;
        default:
            return v;
        }
    }
}

void
test_vector()
{
    vector vec;
    vecval v;
    vec = vecmake(2);
    assert(vec.cap == 2);
    assert(vec.len == 0);
    vecadd(&vec, 5);
    vecadd(&vec, 16);
    vecadd(&vec, 3);
    assert(vec.len == 3);
    assert(vecindexof(&vec, 0, 16) == 1);
    assert(vecindexof(&vec, 0, 2) == -1);
    v = vecat(&vec, 2);
    assert(v.ok == 1);
    assert(v.v == 3);
    v = vecat(&vec, 4);
    assert(v.ok == 0);
    vecadd(&vec, 17);
    vecdel(&vec, 2);
    v = vecat(&vec, 2);
    assert(v.ok == 1);
    assert(v.v == 17);
    vecfree(&vec);
}

int
plen(void)
{
    printf("Delka vektoru: %d\n", vec.len);
    return 0;
}

int
pin(void)
{
    yumv a;
    puts("Vlozte hodnotu prvku:");
    a = yum();
    vecadd(&vec, a.v);
    return 0;
}

int
pdel(void)
{
    yumv a;
    puts("Zadejte pozici prvku ktery chcete odebrat:");
    a = yum();
    vecdel(&vec, a.v);
    return 0;
}

int
pprint(void)
{
    int i;
    for(i = 0; i < vec.len; i++)
        printf("vector[%d]: %d\n", i, vec.p[i]);
    return 0;
}

int
phas(void)
{
    yumv a;
    puts("Zadejte hodnotu hledaneho prvku:");
    a = yum();
    if(vecindexof(&vec, 0, a.v) == -1)
        puts("Vector neobsahuje prvek");
    return 0;
}

int
pfind(void)
{
    yumv a, b;
    puts("Zadejte hodnotu hledaneho prvku:");
    a = yum();
    puts("Zadejte pocatecni index hledani:");
    b = yum();
    printf("Pozice prvku: %d\n", vecindexof(&vec, b.v, a.v));
    return 0;
}

int
pclear(void)
{
    vec.len = 0;
    return 0;
}

int (*func[7])(void) =
{
    plen,
    pin,
    pdel,
    pprint,
    phas,
    pfind,
    pclear,
};

int
main(void)
{
    int ch;
    assert('0'-48 == 0);
    test_vector();
    test_single();
    vec = vecmake(20);
    vecadd(&vec, 0);
    vecadd(&vec, 5);
    vecadd(&vec, 10);
    vecadd(&vec, 15);
    vecadd(&vec, 20);
    vecadd(&vec, 25);
    vecadd(&vec, 30);
    vecadd(&vec, 35);
    vecadd(&vec, 40);
    vecadd(&vec, 45);
    vecadd(&vec, 50);
    vecadd(&vec, 55);
    vecadd(&vec, 60);
    vecadd(&vec, 65);
    vecadd(&vec, 70);
    vecadd(&vec, 75);
    vecadd(&vec, 80);
    vecadd(&vec, 85);
    vecadd(&vec, 90);
    vecadd(&vec, 95);
    puts("Vector test program");
menu:
    puts("Stisknete:");
    puts("1 pro vypis delky vektoru");
    puts("2 pro pridani prvku");
    puts("3 pro odebrani prvku");
    puts("4 pro vypis vsech prvku");
    puts("5 pro zjisteni jestli vektor obsahuje prvek");
    puts("6 pro nalezení pozice prvku");
    puts("7 pro vycisteni vektoru");
    puts("cokoli jineho pro konec.");
    ch = getchar();
    switch(ch) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
        eat();
        func[ch-48-1]();
        goto menu;
    default:
        break;
    }
    vecfree(&vec);
    puts("Konec souboru, koncime.");
    return 0;
}
