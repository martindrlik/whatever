#include "bh.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * bit returns bit at n (0 is lowest) of h; returns -1 for n < 0.
 */
int bit(int h, int n)
{
	if(n < 0)
		return -1;
	return (h & (1<<n)) != 0;
}

/*
 * to_hexch returns hexadecimal character representation of n that is between 0
 * and 15, both included; for other n returns '?'.
 */
char to_hexch(int n)
{
	if(n >= 0 && n <= 9)
		return n+'0';
	if(n >= 10 && n <= 15)
		return n+('A'-10);
	return '?';
}

const int menu = 0;
const int game = 1;
const int over = 2;
const int levl = 3;

char hxlife[] = "A # # #";
const char *bits[] = {
	"       ",
	"      0",
	"      1",
	"    0  ",
	"    1  ",
	"  0    ",
	"  1    ",
	"0      ",
	"1      ",
};
int bitrows[10];
int caret;

/**
 * set_hxlife sets and prints hexadecimal given by h and life to game screen.
 */
void set_hxlife(int h, int life)
{
	hxlife[0] = to_hexch(h);
	if(life < 1)
		hxlife[2] = ' ';
	else
		hxlife[2] = '#';
	if(life < 2)
		hxlife[4] = ' ';
	else
		hxlife[4] = '#';
	if(life < 3)
		hxlife[6] = ' ';
	else
		hxlife[6] = '#';
	bh_center(game, -6, hxlife);
}

/**
 * hint prints hint for hexadecimal h to game screen.
 */
void hint(int h)
{
	static char hnt[] = "1 0 1 0";
	int i;
	for(i = 0; i < 4; i++)
		hnt[i * 2] = bit(h, 3 - i) ? '1' : '0';
	bh_center(game, 4, hnt);
}

/**
 * rand_bits returns random index into bits[].
 */
int rand_bits()
{
	return rand() % 9;
}

/**
 * set_caret sets and prints caret ^ and eventualy discards unwanted falling
 * bit.
 */
void set_caret(int c)
{
	static char t[] = "1 0 1 0";
	if(c < 0)
		c = 0;
	else if(c > 3)
		c = 3;
	caret = c;
	if(bits[bitrows[8]][c * 2] != ' ')
		bitrows[8] = 0;
	strncpy(t, bits[bitrows[8]], 7);
	t[c * 2] = '^';
	bh_center(game, 3, t);
}

/**
 * rand_hex returns random hexadecimal number that is most likely not repeated
 * within 16 times of calling rand_hex().
 */
int rand_hex()
{
	static char used[16] = {0};
	static int count = 0;
	int i;
	int h;

	count++;
	if(count % 16 == 0) {
		for(i = 0; i < 16; i++)
			used[i] = 0;
		count = 0;
	}

	for(i = 0; i < 64; i++) {
		h = rand() % 16;
		if(used[h])
			continue;
		used[h] = 1;
		return h;
	}
	return h;
}

/**
 * play starts a new game with difficulty given by level.
 *
 * Returns nonzero to indicate next level or game over - check *life to find
 * out if next level or game over; returns 0 to exit.
 *
 * Notice that level is used to calculate time to move falling bits and should
 * be [0, positive_considerable_value).
 */
int play(int level, int *life)
{
	char answer[] = "_ _ _ _";
	double t = 0.0;
	double wait = 0.0;
	double wait_t = 0.0;
	int done = 0;
	int h;
	int i;

	assert(level > 0);
	*life = 3;

	h = rand_hex();
	set_hxlife(h, *life);
	for(i = 0; i < 10; i++)
		bitrows[i] = 0;
	set_caret(0);
	bh_center(game, 4, answer);

	for(;;) {
		if(bh_kbhit()) {
			switch(bh_getch()) {
			case 'a':
				if(wait > 0.0)
					break;
				set_caret(0);
				break;
			case 's':
				if(wait > 0.0)
					break;
				set_caret(1);
				break;
			case 'd':
				if(wait > 0.0)
					break;
				set_caret(2);
				break;
			case 'f':
				if(wait > 0.0)
					break;
				set_caret(3);
				break;
			case 'h':
				if(wait > 0.0)
					break;
				hint(h);
				wait = 1.6;
				wait_t = bh_delta();
				(*life)--;
				set_hxlife(h, *life);
				break;
			case 'q':
				return 0;
			}
		}
		if(wait > 0.0) {
			wait_t += bh_delta();
			if(wait_t < wait)
				continue;
			wait_t = 0.0;
			wait = 0.0;
			if(done || *life <= 0)
				return 1;
			bh_center(game, 4, answer);
			bh_delta();
		}
		t += bh_delta();
		if(t < 1.0 - level*0.1)
			continue;
		t = 0.0;

		for(i = 9; i >= 0; i--) {
			if(i == 0)
				bitrows[i] = rand_bits();
			else
				bitrows[i] = bitrows[i-1];
			if(i < 8)
				bh_center(game, i - 5, bits[bitrows[i]]);
		}
		set_caret(caret);

		for(i = 0; i < 4; i++) {
			int a, b;
			int bi;

			bi = bitrows[9];

			if(!bi)
				break;

			a = bit(h, 3 - i);
			b = bits[bi][i * 2];

			if(b == ' ')
				continue;
			if(!a && b == '0')
				answer[i * 2] = b;
			else if(a && b == '1')
				answer[i * 2] = b;
			else {
				answer[i * 2] = '#';
				(*life)--;
				set_hxlife(h, *life);
			}
		}
		bh_center(game, 4, answer);
		for(i = 0; i < 4; i++) {
			char a = answer[i * 2];
			if(a == '#' || a == '_')
				break;
			if(i == 3)
				done = 1;
		}
		if(*life > 0 && !done)
			continue;
		if(done)
			bh_center(game, 3, "* * * *");
		else
			hint(h);
		/* wait to show complete answer */
		wait = 1.6;
		wait_t = bh_delta();
	}
	return 1;
}

int main(int argc, char **argv)
{
	atexit(bh_exit);

	if(!bh_create(menu)) {
		fprintf(stderr, "bh_create(menu) failed\n");
		return 1;
	}
	if(!bh_create(game)) {
		fprintf(stderr, "bh_create(game) failed\n");
		return 1;
	}
	if(!bh_create(over)) {
		fprintf(stderr, "bh_create(over) failed\n");
		return 1;
	}
	if(!bh_create(levl)) {
		fprintf(stderr, "bh_create(levl) failed\n");
		return 1;
	}
	bh_center(menu, -6, "Binary Hero");
	bh_center(menu, -4, "a - 2^3");
	bh_center(menu, -3, "s - 2^2");
	bh_center(menu, -2, "d - 2^1");
	bh_center(menu, -1, "d - 2^0");
	bh_center(menu, 1, "h - show in-game hint (consumes 1 life)");
	bh_center(menu, 3, "q - quit");
	bh_center(menu, 6, "Press any key to play");
	bh_center(game, 6, "Press q to exit to main menu");
	bh_center(over, -6, "Game Over :-(");
	bh_center(over, 6, "Press any key to exit to main menu");
	bh_center(levl, -6, "Next Level :-)");
	bh_center(levl, 6, "Press any key to continue");

	srand(time(0));

	for(;;) {
		int ch;
		int i;
		int level = 1;
		int life;
		int total_life = 0;

		bh_display(menu);
		ch = bh_getch();
		if(ch == 'q')
			break;

		for(;;) {
			bh_display(game);

			if(!play(level, &life))
				break;

			assert(life >= 0 && life <= 3);

			if(life == 0) {
				bh_display(over);
				bh_getch();
				break;
			}

			bh_display(levl);
			bh_getch();

			level++;
			total_life += life;
		}
	}

	return 0;
}
