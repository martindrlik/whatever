#include "array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

array *ar;

minmax(void);
minimum(void);
maximum(void);
factorial(void);
fibonacci(void);

int (*func[5])(void) =
{
    minmax,
    minimum,
    maximum,
    factorial,
    fibonacci,
};

/* getch is like getchar(), but eats CR and LF before. */
getch();

/* fib returns fibonacci number for n */
fib(int n);

void
test_fib()
{
    assert(fib(0) == 0);
    assert(fib(1) == 1);
    assert(fib(2) == 1);
    assert(fib(3) == 2);
    assert(fib(4) == 3);
    assert(fib(5) == 5);
    assert(fib(10) == 55);
    assert(fib(16) == 987);
}

/* single returns number created from each digit stored in ar.
 * For ar storing [4, 1, 2, 3] it returns 4123. */
single(array *ar);

void
test_single()
{
    array *ar = array_new(4);
    array_append(ar, 4);
    array_append(ar, 1);
    array_append(ar, 2);
    array_append(ar, 3);
    assert(single(ar) == 4123);
}

main(void)
{
    int i, ch;

    assert('0'-48 == 0);

    test_fib();
    test_single();

    ar = array_new(10);
    if(!ar) {
        fputs("not enough memory", stderr);
        exit(1);
    }
    // atexit(free_ar); nope - need more than this
    for(i = 0; i < 10; i++)
        array_append(ar, i + 1);

menu:
    puts("Stisknete:");
    puts("1 pro minmax");
    puts("2 pro minimum");
    puts("3 pro maximum");
    puts("4 pro faktorial");
    puts("5 pro Fibonacciho posloupnost");
    puts("cokoli jineho pro konec.");
    ch = getch();
    switch(ch) {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
        switch(func[ch-48-1]()) {
        case 1:
            puts("Neocekavany konec souboru");
            break;
        default:
            goto menu;
        }
    }
    array_free(ar);
    return 0;
}

/* single returns number created from each digit stored in ar.
 * For ar storing [4, 1, 2, 3] it returns 4123. */
int
single(array *ar)
{
    int i, j, m, n;
    m = 0;
    for(i = 0; i < ar->len; i++) {
        n = ar->p[i];
        for(j = 0; j < ar->len-i-1; j++)
            n *= 10;
        m += n;
    }
    return m;
}

/* alter_array prints current ar and asks for change of ar. */
int
alter_array()
{
    int i, ch;
    array *na, *nb;

    puts("Aktualni obsah pole:");
    for(i = 0; i < ar->len; i++) {
        if(i + 1 < ar->len)
            printf("%d, ", ar->p[i]);
        else
            printf("%d\n", ar->p[i]);
    }
    puts("Zadat jine hodnoty? (A/N)");
    if(getch() != 'A')
        return 0;
    puts("Zadavejte hodnoty oddelene novym radkem, konec=K:");
    na = array_new(10);
    if(!na) {
        fputs("not enough memory", stderr);
        array_free(ar);
        exit(1);
    }
    nb = array_new(10);
    if(!nb) {
        fputs("not enough memory", stderr);
        array_free(na);
        array_free(ar);
        exit(1);
    }
    for(;;) {
        ch = getchar();
        switch(ch) {
        case EOF:
            array_free(na);
            array_free(nb);
            return 1;
        case 'K':
            goto out;
        case '-':
            break;
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
            /* Only one digit is stored then single()
             * is used to get whole number.
             * Silly, but seems simple. */
            array_append(na, ch-48);
            break;
        case '\n':
        case '\r':
            if(na->len > 0)
                array_append(nb, single(na));
            na->len = 0;
        }
    }
out:
    if(nb->len > 0) {
        array_free(ar);
        ar = array_new(nb->len);
        for(i = 0; i < nb->len; i++)
            array_append(ar, nb->p[i]);
    }
    array_free(na);
    array_free(nb);
    return 0;
}

/* num asks for integer and returns it. */
int
num()
{
    int N;

    puts("Vlozte hodnotu N:");
    scanf_s("%d", &N);
    return N;
}

void
mima(int *min, int *max)
{
    int i;

    for(i = 0; i < ar->len; i++) {
        if(i == 0) {
            *min = ar->p[i];
            *max = ar->p[i];
            continue;
        }
        if(*min > ar->p[i])
            *min = ar->p[i];
        if(*max < ar->p[i])
            *max = ar->p[i];
    }
}

/* minmax prints min and max value of ar. */
minmax(void)
{
    int min, max;
    if(alter_array())
        return 1;
    mima(&min, &max);
    printf("Minimum=%d, maximum=%d\n", min, max);
    return 0;
}

/* minimum prints min value of ar. */
minimum(void)
{
    int min, max;
    if(alter_array())
        return 1;
    mima(&min, &max);
    printf("Minimum=%d\n", min);
    return 0;
}

/* maximum prints max value of ar. */
maximum(void)
{
    int min, max;
    if(alter_array())
        return 1;
    mima(&min, &max);
    printf("Maximum=%d\n", max);
    return 0;
}

factorial(void)
{
    int n, i;
    unsigned k = 1;

    n = num();
    for(i = n; i >= 1; i--)
        k *= i;
    printf("Faktorial(%d)=%d\n", n, k);
    return 0;
}

fib(int n)
{
    if(n <= 0)
        return 0;
    if(n <= 2)
        return 1;
    return fib(n-2) + fib(n-1);
}

fibonacci(void)
{
    int n = num();
    printf("Fibonacci(%d)=%d\n", n, fib(n));
    return 0;
}

getch()
{
    int ch;

    for(;;) {
        ch = getchar();
        if(ch == '\n')
            continue;
        if(ch == '\r')
            continue;
        return ch;
    }
}
