#ifndef HT_H
#define HT_H

struct person
{
    const char key[255];
    const char name[255];
    double age;
    double weight;
    double height;
};

struct htable
{
    struct person *arr;
    int cap;
    int len;
};

void htfree(struct htable ht);
struct htable htinit(int cap);

/* htset inserts p for kay to ht and returns index (hash). */
htset(struct htable *ht, const char *key, struct person p);

void htwalk(struct htable ht, void (*fn)(struct person));

#endif /* HT_H */
