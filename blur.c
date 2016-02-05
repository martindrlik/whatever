#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const int R = 2;
const int G = 1;
const int B = 0;

typedef struct rgb
{
	uint32_t r, g, b;
}
rgb;

uint8_t *src;
int ofs;
int w, h;

void add(int x, int y, rgb *c, uint32_t mul)
{
	(*c).r += (src + ofs)[y*w*3 + x*3 + R] * mul;
	(*c).g += (src + ofs)[y*w*3 + x*3 + G] * mul;
	(*c).b += (src + ofs)[y*w*3 + x*3 + B] * mul;
}

int main(int argc, char **argv)
{
	FILE *file;
	size_t len;
	uint8_t *dst;
	size_t n;
	int x, y;
	rgb c;

	/* blur */
	const uint32_t fac = 16;
	const uint32_t mat[] =
	{
		1, 2, 1,
		2, 4, 2,
		1, 2, 1,
	};

	if (argc <= 1) {
		fprintf(stderr, "usage: %s 24bppBGR.bmp \n", argv[0]);
		return 0;
	}

	file = fopen(argv[1], "r+b");
	if (file == NULL) {
		perror("open image");
		return 0;
	}

	if (fseek(file, 0, SEEK_END) != 0) {
		perror("seek end");
		goto close;
	}

	len = ftell(file);
	if (len == EOF) {
		perror("ftell");
		goto close;
	}

	src = NULL;
	dst = NULL;

	src = malloc(len * sizeof(uint8_t));
	if (src == NULL) {
		perror("malloc");
		goto close;
	}
	dst = malloc(len * sizeof(uint8_t));
	if (dst == NULL) {
		perror("malloc");
		goto free;
	}

	rewind(file);
	n = fread(src, sizeof(uint8_t), len, file);
	if (n != len) {
		perror("fread");
		goto free;
	}

	memcpy(dst, src, len);

	ofs = *((int*)(src + 10));
	w = *((int*)(src + 18));
	h = *((int*)(src + 22));

	for (x = 0; x < w; x++) {
		for (y = 0; y < h; y++) {

			c.r = 0;
			c.g = 0;
			c.b = 0;

			if (y + 1 < h) {
				if (x - 1 >= 0)
					add(x - 1, y + 1, &c, mat[0]);
				add(x, y + 1, &c, mat[1]);
				if (x + 1 < w)
					add(x + 1, y + 1, &c, mat[2]);
			}
			if (x - 1 >= 0)
				add(x - 1, y, &c, mat[3]);
			add(x, y, &c, mat[4]);
			if (x + 1 < w)
				add(x + 1, y, &c, mat[5]);
			if (y - 1 >= 0) {
				if (x - 1 >= 0)
					add(x - 1, y - 1, &c, mat[6]);
				add(x, y - 1, &c, mat[7]);
				if (x + 1 < w)
					add(x + 1, y - 1, &c, mat[8]);
			}

			(dst + ofs)[y*w*3 + x*3 + R] = c.r / fac;
			(dst + ofs)[y*w*3 + x*3 + G] = c.g / fac;
			(dst + ofs)[y*w*3 + x*3 + B] = c.b / fac;

		}
	}

	rewind(file);
	n = fwrite(dst, sizeof(uint8_t), len, file);
	if (n != len) {
		perror("fwrite");
		goto free;
	}

free:
	free(src);
	free(dst);
close:
	if (fclose(file) == EOF) {
		perror("close");
	}

	return 0;
}
