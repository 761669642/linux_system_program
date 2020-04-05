#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define oops(x,y)  {perror(x); exit(y);}
#define PAR_MESS "Dad have something to say to you!\n"
#define CHILD_MESS "shut up papa!\n"
int main(void)
{
	int pipefd[2];
	int len;
	char buf[200];
	int read_len;

	if (pipe(pipefd) == -1)
		oops("can't get a pipe", 1);
	switch(fork()) {
		case -1:
			oops("fork failed", 1);
		case 0:
			len = strlen(CHILD_MESS);
			while (1) {
				if (write(pipefd[1], CHILD_MESS, len) != len)
					oops("write pipe1",1);
				sleep(5);
			}
		default:
			len = strlen(PAR_MESS);
			while (1) {
				if (write(pipefd[1], PAR_MESS, len) != len)
					oops("write pipe1", 1);
				sleep(1);
				read_len = read(pipefd[0], buf, 200);
				if (read_len <= 0)
					break;
				write(1, buf, read_len);
			}
	}
}
