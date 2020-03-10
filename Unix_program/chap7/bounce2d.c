#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#define DFL_SYMBOL 'o'
#define BLANK   ' '
#define TOP_ROW  5
#define BOT_ROW  20
#define LEFT_EDGE  10
#define RIGHT_EDGE  70
#define X_INIT  10
#define Y_INIT  10
#define TICKS_PER_SEC  50
#define X_TTM  5
#define Y_TTM  8
struct ppball {
	int y_pos, x_pos,
		y_ttm, x_ttm,
		y_ttg, x_ttg,
		y_dir, x_dir;
	char symbol;
};

int set_ticker(int);
void set_up();
void wrap_up();
void ball_move(int);
int bounce_or_lose(struct ppball *bp);

struct ppball the_ball;

int main(void)
{
	int c;
	
	set_up();

	while (1) {
		c = getch();
		if (c == 'q') break;
		if (c == 'f') the_ball.x_ttm--;
		if (c == 'f') the_ball.x_ttm++;
		if (c == 's') the_ball.y_ttm--;
		if (c == ' ') the_ball.y_ttm++;
	}
	wrap_up();
	return 0;
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

void wrap_up()
{
	set_ticker(0);
	endwin();
}

void set_up()
{
	the_ball.y_pos = Y_INIT;
	the_ball.x_pos = X_INIT;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM;
	the_ball.y_dir = 1;
	the_ball.x_dir = 1;
	the_ball.symbol = DFL_SYMBOL;

	initscr();
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
	refresh();

	signal(SIGALRM, ball_move);
	set_ticker(1000/TICKS_PER_SEC);
}

void ball_move(int signum)
{
	int y_cur, x_cur, moved;

	signal(SIGALRM, SIG_IGN);
	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;
	moved = 0;

	if (the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1) {
		the_ball.y_pos += the_ball.y_dir;
		the_ball.y_ttg = the_ball.y_ttm;
		moved = 1;
	}
	if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1) {
		the_ball.x_pos += the_ball.x_dir;
		the_ball.x_ttg = the_ball.x_ttm;
		moved = 1;
	}

	if (moved) {
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
		bounce_or_lose(&the_ball);
		move(LINES-1, COLS-1);
		refresh();
	}
	signal(SIGALRM, ball_move);
}

int bounce_or_lose(struct ppball *bp)
{
	int return_val = 0;
	if (bp->y_pos == TOP_ROW) {
		bp->y_dir = 1;
		return_val = 1;
	} else if (bp->y_pos == BOT_ROW) {
		bp->y_dir = -1;
		return_val = 1;
	}

	if (bp->x_pos == LEFT_EDGE) {
		bp->x_dir = 1;
		return_val = 1;
	} else if (bp->x_pos == RIGHT_EDGE) {
		bp->x_dir = -1;
		return_val = 1;
	}
	return return_val;
}
