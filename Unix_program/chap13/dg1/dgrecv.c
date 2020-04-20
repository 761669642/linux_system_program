#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dgram.h"

#define oops(msg) {perror(msg); exit(1);}

void say_who_called();
int main(int argc, char **argv)
{
	int port, sock_id;
	char buf[200];
	size_t msglen;
	struct sockaddr_in saddr;
	socklen_t saddrlen;

	if (argc == 1 || (port = atoi(argv[1])) <= 0)
		oops("usage: dgrecv portnumber\n")
	if ((sock_id = make_dgram_server_socket(port)) == -1) 
		oops("cannot make dgram server socket")
	saddrlen = sizeof(saddr);
	while ((msglen = recvfrom(sock_id, buf, 200, 0, (struct sockaddr *)&saddr, &saddrlen)) > 0) {
		buf[msglen] = '\0';
		printf("dgrecv: got a message: %s\n", buf);
		say_who_called(&saddr);
	}
	return 0;
}

void say_who_called(struct sockaddr_in *addrp)
{
	char host[200];
	int port;
	get_internet_address(host, 200, &port, addrp);
	printf("from: %s: %d\n", host, port);
}
