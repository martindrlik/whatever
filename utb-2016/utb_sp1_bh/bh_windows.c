#include "bh.h"

#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define nbuf 5

HANDLE hstdout;
HANDLE buffer[nbuf];

void bh_exit()
{
	if(hstdout != 0 && hstdout != INVALID_HANDLE_VALUE)
		SetConsoleActiveScreenBuffer(hstdout);
}

int bh_kbhit()
{
	return _kbhit();
}

int bh_getch()
{
	return _getch();
}

int bh_create(int id)
{
	CONSOLE_CURSOR_INFO fo;

	assert(id >= 0 || id < nbuf);

	if(buffer[id] != 0 && buffer[id] != INVALID_HANDLE_VALUE)
		return 1; /* ok ignore; already created */

	hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hstdout == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "GetStdHandle failed (%d)\n", GetLastError);
		return 0;
	}

	buffer[id] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL);
	if(buffer[id] == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "CreateConsoleScreenBuffer failed (%d)\n", GetLastError());
		return 0;
	}

	/* hide cursor */
	fo.bVisible = 0;
	fo.dwSize = 100;
	if(!SetConsoleCursorInfo(buffer[id], &fo)) {
		fprintf(stderr, "SetConsoleCursorInfo failed (%d)\n", GetLastError());
		return 0;
	}

	return 1;
}

int bh_display(int id)
{
	assert(id >= 0 || id < nbuf);
	assert(buffer[id] != 0);
	assert(buffer[id] != INVALID_HANDLE_VALUE);
	if(SetConsoleActiveScreenBuffer(buffer[id]))
		return 1;
	fprintf(stderr, "SetConsoleActiveScreenBuffer failed (%d)\n", GetLastError());
	return 0;
}

void center_error(int id, int y, const char *text, const char *func)
{
	fprintf(stderr, "bh_center(id(%d), ", id);
	fprintf(stderr, "y(%d) ", y);
	fprintf(stderr, "text(%s)) failed:\n", text);
	fprintf(stderr, "%s failed (%d)\n", func, GetLastError());
}

int bh_center(int id, int y, const char *text)
{
	CONSOLE_SCREEN_BUFFER_INFO fo;
	COORD p;
	int n;

	assert(id >= 0 || id < nbuf);
	assert(buffer[id] != 0);
	assert(buffer[id] != INVALID_HANDLE_VALUE);

	if(!GetConsoleScreenBufferInfo(buffer[id], &fo)) {
		center_error(id, y, text, "GetConsoleScreenBufferInfo");
		return 0;
	}

	n = strlen(text);
	p.X = (fo.srWindow.Right - n)/2;
	p.Y = fo.srWindow.Bottom/2 + y;
	if(!SetConsoleCursorPosition(buffer[id], p)) {
		center_error(id, y, text, "SetConsoleCursorPosition");
		return 0;
	}
	if(!WriteConsole(buffer[id], text, strlen(text), NULL, NULL)) {
		center_error(id, y, text, "WriteConsole");
		return 0;
	}

	return 1;
}
