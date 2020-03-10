#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>

#define MESSAGE  "hello"
#define BLANK    "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;

int set_ticker(int);
void on_alarm(int);
void on_input(int);
void enable_kbd_signals();

int main(void)
{
	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO, on_input);
	enable_kbd_signals();
	signal(SIGALRM, on_alarm);
	set_ticker(delay);
	
	mvaddstr(row, col, MESSAGE);

	while (!done) pause();
	endwin();
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);
	mvaddstr(row, col, BLANK);
	col += dir;
	mvaddstr(row, col, MESSAGE);
	refresh();
}

void on_input(int signum)
{
	int c = getch();
	if (c == 'q' || c == EOF) done = 1;
	else if( c == 'r') dir = -dir;
}

void enable_kbd_signals()
{
	int fd_flags;
	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}

int set_ticker(int n_msec)
{
	struct itimerval new_timeset;
	long n_sec, n_usec;

	n_sec = n_msec / 1000;
	n_usec = (n_msec % 1000) * 1000;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usec;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usec;
	
	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
