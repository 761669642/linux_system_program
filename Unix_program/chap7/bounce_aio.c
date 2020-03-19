#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <aio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define MESSAGE  "hello"
#define BLANK    "     "

int row = 10;
int col = 0;
int dir = 1;
int delay = 200;
int done = 0;
struct aiocb kbc_buf;

int set_ticker(int);
void on_alarm(int);
void on_input(int);
void setup_aio_buffer();

int main(void)
{
	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO, on_input);
	setup_aio_buffer();
	aio_read(&kbc_buf);

	signal(SIGALRM, on_alarm);
	set_ticker(delay);
	
	mvaddstr(row, col, MESSAGE);

	while (!done) pause();
	endwin();
}

void setup_aio_buffer()
{
	static char input[1];

	kbc_buf.aio_fildes = 0;
	kbc_buf.aio_buf = input;
	kbc_buf.aio_nbytes = 1;
	kbc_buf.aio_offset = 0;
	
	kbc_buf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	kbc_buf.aio_sigevent.sigev_signo = SIGIO;

}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);
	mvaddstr(row, col, BLANK);
	col += dir;
	mvaddstr(row, col, MESSAGE);
	refresh();

	if (dir == -1 && col <= 0) dir = 1;
	else if (dir == 1 && col >= COLS-strlen(MESSAGE)) dir = -1;
}

void on_input(int signum)
{
	int c;
	char *cp = (char *)kbc_buf.aio_buf;
	if (aio_error(&kbc_buf) != 0)
		perror("reading failed");
	else {
		if (aio_return(&kbc_buf) == 1) {
			c = *cp;
			if (c == 'q' || c == EOF) done = 1;
			else if (c = 'r') dir = -dir;
		}
	}
	aio_read(&kbc_buf);
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
