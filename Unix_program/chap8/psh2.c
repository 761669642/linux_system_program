#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXARGS 20
#define ARGLEN 100
char *makestring(char *buf);
void execute(char **);
int main(void)
{
	char *argv[MAXARGS+1];
	int numargs = 0;
	char argbuf[ARGLEN];

	while (numargs < MAXARGS) {
		printf("Arg[%d]?", numargs);
		if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
			argv[numargs++] = makestring(argbuf);
		else {
			if (numargs > 0) {
				argv[numargs] = NULL;
				execute(argv);
				numargs = 0;
			}
		}
	}
	return 0;
}

char *makestring(char *buf)
{
	char *cp = NULL;
	buf[strlen(buf)-1] = '\0';
	cp = (char *) malloc(strlen(buf)+1);
	if (!cp) {
		printf("malloc failed!\n");
		exit(1);
	}
	strcpy(cp, buf);
	return cp;
}

void execute(char *argv[])
{
	int pid, exit_status,wait_ret;
	pid = fork();
	if (pid == -1) {
		perror("fork faild");
		exit (1);
	} else if (pid == 0) {
		execvp(argv[0], argv);
		perror("execvp failed\n");
		exit(1);
	} else {
		wait_ret = wait(&exit_status);
		if (wait_ret != pid)
			fprintf(stderr, "wait child %d failed\n", pid);
		else
			printf("child exited with status %d, %d\n", exit_status>>8, exit_status&0xff);
	}
}
