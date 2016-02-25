#ifndef _bh_h
#define _bh_h

void bh_exit();

/*
 * bh_delta returns elapsed time (in seconds) since last call of bh_delta.
 */
double bh_delta();

/*
 * bh_kbhit checks the console for keyboard input and returns nonzero value if
 * a key has been pressed; 0 otherwise.
 */
int bh_kbhit();

/*
 * bh_getch gets a character from the console without echo.
 */
int bh_getch();

/*
 * bh_create creates new console screen buffer identified by id.
 * Returns nonzero if no error; 0 otherwise.
 */
int bh_create(int id);

/*
 * bh_display shows console screen buffer specified by id.
 * Returns nonzero if no error; 0 otherwise.
 */
int bh_display(int id);

/*
 * bh_center writes text to console screen buffer specified by bufid at center
 * of the specified buffer with vertical offset y.
 *
 * Returns nonzero if no error; 0 otherwise.
 */
int bh_center(int id, int y, const char *text);

#endif /*_bh_h*/
