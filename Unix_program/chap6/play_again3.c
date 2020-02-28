#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

#define QUESTION "Do you want anthor transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

int get_response(char *, int);
void tty_mode(int);
void set_crmode(void);
void set_nodelay_mode(void);
int get_ok_char(void);

int main(void)
{
	int response;
	tty_mode(0);
	set_crmode();
	set_nodelay_mode();
	response = get_response(QUESTION, TRIES);
	tty_mode(1);
	return response;
}

int get_response(char *question, int maxtries)
{
	int input;
	printf("%s (y/n)?", question);
	fflush(stdout);
	while (1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if (input == 'y') return 0;
		else if (input == 'n') return 1;
		else if (maxtries-- == 0) return 2;
		BEEP;
	}
}

void set_crmode(void)
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

int get_ok_char(void)
{
	int c;
	while((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
		;//do nothing
	return c;

}

void set_nodelay_mode(void)
{
	int termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	if (how == 0) {
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
	}
	else {
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}
