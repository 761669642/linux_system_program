#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXMSG 10
#define TUNIT 20000

struct propset {
	char *str;
	int row;
	int delay;
	int dir;
};

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;
void *animate();
int setup();
int main(int argc, char *argv[])
{
	int c;
	pthread_t thrds[10];
	struct propset props[10];
	int num_msg;
	int i;
	if (argc == 1) {
		printf("usage: tanimate string..\n");
		exit(1);
	}
	num_msg = setup(argc-1, argv+1, props);
	for (i = 0; i < num_msg; i++) {
		if (pthread_create(&thrds[i], NULL, animate, &props[i])) {
			fprintf(stderr, "create thread failed\n");
			endwin();
			exit(0);
		}
	}

	while (1) {
		c = getch();
		if (c == 'q') break;
		if (c = ' ') {
			for (i = 0; i < num_msg; i++)
				props[i].dir = -props[i].dir;
		}
		if (c >= '0' && c <= '9') {
			i = c - '0';
			if (i < num_msg)
				props[i].dir = -props[i].dir;
		}
	}

	pthread_mutex_lock(&mx);
	for (i = 0; i < num_msg; i++)
		pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

int setup(int nstrings, char *strings[], struct propset props[])
{
	int num_msg = (nstrings > 10 ? 10 : nstrings);
	int i;
	srand(getpid());
	for (i = 0; i < num_msg; i++) {
		props[i].str = strings[i];
		props[i].row = i;
		props[i].delay = 1 + (rand() % 15);
		props[i].dir = ((rand()%2) ? 1 : -1);
	}

	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES-1, 0, "'q' to quit, '0'..'%d' to bounce", num_msg-1);
	return num_msg;
}

void *animate(void *arg)
{
	struct propset *info = arg;
	int len = strlen(info->str) + 2;
	int col = rand() % (COLS-len-3);
	while (1) {
		usleep(info->delay * TUNIT);
		pthread_mutex_lock(&mx);
		move(info->row, col);
		addch(' ');
		addstr(info->str);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();
		pthread_mutex_unlock(&mx);
		col += info->dir;
		if (col <= 0 && info->dir == -1)
			info->dir = 1;
		else if (col+len >= COLS && info->dir == 1)
			info->dir = -1;
	}
}