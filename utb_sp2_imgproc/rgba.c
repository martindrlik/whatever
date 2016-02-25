#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct rgb32
{
	uint32_t r, g, b;
}
rgb32;

typedef struct bgr
{
	uint8_t b, g, r;
}
bgr;

int w, h;

void add(bgr *px, int x, int y, rgb32 *c, uint32_t mul)
{
	(*c).r += px[y*w + x].r * mul;
	(*c).g += px[y*w + x].g * mul;
	(*c).b += px[y*w + x].b * mul;
}

rgb32 sum3x3(bgr *px, int x, int y, const uint32_t mat[])
{
	rgb32 c;

	c.r = 0;
	c.g = 0;
	c.b = 0;

	if (y + 1 < h) {
		if (x - 1 >= 0)
			add(px, x - 1, y + 1, &c, mat[0]);
		add(px, x, y + 1, &c, mat[1]);
		if (x + 1 < w)
			add(px, x + 1, y + 1, &c, mat[2]);
	}
	if (x - 1 >= 0)
		add(px, x - 1, y, &c, mat[3]);
	add(px, x, y, &c, mat[4]);
	if (x + 1 < w)
		add(px, x + 1, y, &c, mat[5]);
	if (y - 1 >= 0) {
		if (x - 1 >= 0)
			add(px, x - 1, y - 1, &c, mat[6]);
		add(px, x, y - 1, &c, mat[7]);
		if (x + 1 < w)
			add(px, x + 1, y - 1, &c, mat[8]);
	}

	return c;
}

bgr blur(bgr *px, int x, int y)
{
	bgr b;
	rgb32 c;

	static const uint32_t m[] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
	};

	c = sum3x3(px, x, y, m);

	b.r = c.r / 9.0;
	b.g = c.g / 9.0;
	b.b = c.b / 9.0;

	return b;
}

bgr gray(bgr *px, int x, int y)
{
	bgr p;
	double d;
	uint8_t lum;

	p = px[y*w + x];
	d = p.r*0.21 + p.g*0.75 + p.b*0.07;
	lum = (uint8_t)fmin(255, d);

	p.r = lum;
	p.g = lum;
	p.b = lum;

	return p;
}

bgr sharp(bgr *px, int x, int y)
{
	bgr b;
	rgb32 c;

	static const uint32_t m[] = {
		0, -1, 0,
		-1, 5, -1,
		0, -1, 0,
	};

	c = sum3x3(px, x, y, m);

	b.r = c.r;
	b.g = c.g;
	b.b = c.b;

	return b;
}

/*
 * read reads the file named by name and returns the contents.
 * A successful call returns != NULL.
 */
uint8_t *read(const char *name, size_t *len)
{
	FILE *file;
	size_t n;
	uint8_t *data = NULL;

	file = fopen(name, "r+b");
	if (file == NULL) {
		perror("fopen");
		return NULL;
	}

	if (fseek(file, 0, SEEK_END) != 0) {
		perror("fseek");
		goto close;
	}

	*len = ftell(file);
	if (*len == EOF) {
		perror("ftell");
		goto close;
	}

	data = malloc(*len * sizeof(uint8_t));
	if (data == NULL) {
		perror("malloc");
		goto close;
	}

	rewind(file);
	n = fread(data, sizeof(uint8_t), *len, file);
	if (n != *len) {
		perror("fread");
		free(data);
		data = NULL;
	}

close:
	if (fclose(file) == EOF) {
		perror("close");
	}

	return data;
}

/*
 * write writes data to a file named by name. If the file does not exists,
 * write creates it.
 * A successful call returns == len.
 */
size_t write(const char *name, uint8_t *data, size_t len)
{
	FILE *file;
	size_t n;

	file = fopen(name, "w+b");
	if (file == NULL) {
		perror("fopen");
		return 0;
	}

	n = fwrite(data, sizeof(uint8_t), len, file);
	if (n != len)
		perror("fwrite");

	if (fclose(file) == EOF)
		perror("close");

	return n;
}

int main(int argc, char **argv)
{
	size_t len;
	uint8_t *src;
	uint8_t *dst;
	int ofs;
	int x, y;
	bgr *spx;
	bgr *dpx;
	bgr (*filter)(bgr *, int x, int y) = &gray;

	if (argc <= 2) {
		fprintf(stderr, "usage: %s input.bmp ouput.bmp [blur|gray|sharp]\n", argv[0]);
		return 1;
	}

	if (argc > 3) {
		if (strcmp(argv[3], "blur") == 0)
			filter = &blur;
		else if (strcmp(argv[3], "sharp") == 0)
			filter = &sharp;
	}

	src = read(argv[1], &len);
	if (src == NULL) {
		fprintf(stderr, "unable to read: %s\n", argv[1]);
		return 1;
	}

	dst = malloc(len * sizeof(uint8_t));
	if (dst == NULL) {
		perror("malloc");
		free(src);
		return 1;
	}

	memcpy(dst, src, len);

	ofs = *((int*)(src + 10));
	w = *((int*)(src + 18));
	h = *((int*)(src + 22));

	spx = (bgr *)(src + ofs);
	dpx = (bgr *)(dst + ofs);

	for (x = 0; x < w; x++) {
		for (y = 0; y < h; y++) {

			dpx[y*w + x] = filter(spx, x, y);

		}
	}

	free(src);

	if (write(argv[2], dst, len) != len) {
		fprintf(stderr, "unable to write: %s\n", argv[2]);
		free(dst);
		return 1;
	}

	free(dst);
	return 0;
}
