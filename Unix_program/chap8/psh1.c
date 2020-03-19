#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define MAXARGS 20
#define ARGLEN  100

char *makestring();
int execute();

int main(void)
{
	char *argv[MAXARGS+1];
	int numargs = 0;
	char argbuf[ARGLEN];
	
	while(numargs < MAXARGS) {
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

int execute(char *argv[])
{
	execvp(argv[0], argv);
	perror("execvp failed");
	exit(1);
}

char *makestring(char *buf)
{
	char *cp;
	buf[strlen(buf)-1] = '\0';
	cp = malloc(strlen(buf)+1);
	if (cp == NULL) {
		fprintf(stderr, "no memory, alloc failed\n");
		exit(1);
	}
	strcpy(cp, buf);
	return cp;
}
