#include "socklib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define oops(msg) {perror(msg); exit(1);}


void talk_with_server(int fd)
{
	char buf[200];
	int n;
	memset(buf, 0, sizeof(buf));
	n = read(fd, buf, 200);
	fputs(buf, stdout);
}

int main(int argc, char **argv)
{
	int fd;
	int port = 15000;
	fd = connect_to_server(argv[1], port);
	if (fd == -1)
		oops("connnect to server failed")
	talk_with_server(fd);
	close(fd);
}

