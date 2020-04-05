#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define oops(m,x) {perror(m); exit(x);}

int main(char argc, char **argv)
{
	int thepipe[2], newfd, pid;
	if (argc != 3) {
		fprintf(stderr, "usage: pipe cmd1 cmd2\n");
		exit(1);
	}
	if (pipe(thepipe) == -1)
		oops("get a pipe failed", 1)//oop is a marco, don't need ';'
	pid = fork();
	if (pid == -1)
		oops("fork failed", 2)
	else if (pid > 0) {
		close(thepipe[1]);
		if (dup2(thepipe[0], 0) == -1)
			oops("failed dup pipe0 to stdin", 3)
		close(thepipe[0]);
		execlp(argv[2], argv[2], NULL);
		oops(argv[2], 4)
	} else {
		close(thepipe[0]);
		if (dup2(thepipe[1], 1) == -1)
			oops("failed dup pipe1 to stdout", 5)
		close(thepipe[1]);
		execlp(argv[1], argv[1], NULL);
		oops(argv[1], 6)
	}

}
