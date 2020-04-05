#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	int len, i, apipe[2];
	char buf[200];

	if (pipe(apipe) == -1) {
		perror("could not make pipe");
		exit(1);
	}
	printf("Got a pipe! It is file descriptors: {%d %d}\n", apipe[0], apipe[1]);

	while (fgets(buf, 200, stdin)){
		len = strlen(buf);
		if (write(apipe[1], buf, len) != len) {
			perror("writing to pipe port1");
			break;
		}
		memset(buf, 0, 200);
		len = read(apipe[0], buf, 200);
		if (len == -1) {
			perror("reading from pipe port0");
			break;
		}
		if (write(1, buf, len) != len) {
			perror("writing to stdout");
			break;
		}
	}
}
