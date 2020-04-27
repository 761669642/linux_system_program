#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int row, col, dir, delay;
void *moving_msg();

int main(void)
{
	int ndelay;
	int c;
	pthread_t msg_thread;
	row = 10;
	col = 0;
	dir = 1;
	delay = 200;
	initscr();
	crmode();
	noecho();
	clear();
	if (pthread_create(&msg_thread, NULL, moving_msg, "hello")) {
		fprintf(stderr, "create thread failed\n");
		endwin();
		exit(0);
	}
	while (1) {
		ndelay = 0;
		c = getch();
		if (c == 'q') break;
		if (c == ' ') dir = -dir;
		if (c == 'f' && delay > 2) ndelay = delay/2;
		if (c == 's') ndelay = delay * 2;
		if (ndelay > 0)
			delay = ndelay;
	}
	pthread_cancel(msg_thread);
	endwin();
}

void *moving_msg(char *msg)
{
	while (1) {
		usleep(delay * 1000);
		clear();
		move(row, col);
		addstr(msg);
		refresh();
		col += dir;
		if (col <= 0 && dir == -1) dir = 1;
		else if (col+strlen(msg) >= COLS && dir == 1) dir = -1;
	}
}
