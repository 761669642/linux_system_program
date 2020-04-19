#include "socklib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define oops(msg) {perror(msg); exit(1);}

void process_request(int fd)
{
	int pid = fork();
	switch(pid) {
		case -1: return;
		case 0: dup2(fd, 1);
				close(fd);
				execl("/bin/date", "date", NULL);
				oops("execl failed")
				break;
		default: wait(NULL);
	}

}

int main(void)
{
	int sock_id, fd;
	int port = 15000;
	sock_id = make_server_socket(port);
	if (sock_id == -1)
		oops("make server socket failed")
	while (1) {
		fd = accept(sock_id, NULL, NULL);
		if (fd == -1)
			oops("accept failed")
		process_request(fd);
		close(fd);
	}
}
