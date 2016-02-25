#include "bh.h"

#include <time.h>

long last_t, t;
double d;

double bh_delta()
{
	if(last_t == 0L)
		last_t = clock();
	t = clock();
	d = (t - last_t) / (double)CLOCKS_PER_SEC;
	last_t = t;
	return d;
}
