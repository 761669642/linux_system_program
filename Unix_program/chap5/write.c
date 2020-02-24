#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 1000
int main(int argc, char **argv)
{
	int fd;
	char buf[SIZE];
	if (argc != 2) {
		fprintf(stderr, "Error, the parameters' number is wrong");
		exit(1);
	}

	fd = open(argv[1], O_WRONLY);
	if (fd == -1) {
		perror(argv[1]);
		exit(1);
	}

	while(fgets(buf, SIZE, stdin) != NULL) {
		if (write(fd, buf, strlen(buf)) == -1)
			break;
	}
	close(fd);
}
