# vector

Basic usage:
```
void
main(void)
{
	vector vec;
	int i;

	vec = vecmake(3);
	vecadd(&vec, 3);
	vecadd(&vec, 2);
	vecadd(&vec, 5);

	for(i = 0; i < vec.len; i++)
		printf("%d: %d\n", i, vec.p[i];
}
```
output:
```
	0: 3
	1: 2
	2: 5
```

