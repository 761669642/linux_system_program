#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//#define CLOSE_UP
#define USE_DUP2

int main(void)
{
	int fd;
	int newfd;
	char line[100];
	fgets(line, 100, stdin); 
	printf("%s", line);
	fgets(line, 100, stdin);
	printf("%s", line);

	fd = open("./test.txt", O_RDONLY);
#ifdef CLOSE_UP
	close(0);//close the stdin, then the fd order 0 will be free.
	newfd = dup(fd);
#else
	newfd = dup2(fd, 0);
#endif

	if (newfd != 0) {
		fprintf(stderr, "Could not open data as fd 0\n");
		exit(1);
	}
	close(fd);

	fgets(line, 100, stdin);
	printf("%s", line);
	fgets(line, 100, stdin);
	printf("%s", line);
}
