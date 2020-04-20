#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "dgram.h"

#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char **argv)
{
	int sock_id;
	char *msg;
	struct sockaddr_in saddr;
	if (argc != 4)
		oops("usage: dgsend host port 'msg'\n")
	msg = argv[3];
	if ((sock_id = make_dgram_client_socket()) == -1) 
		oops("make dgram client socket failed!")
	make_internet_address(argv[1], atoi(argv[2]), &saddr);
	if (sendto(sock_id, msg, strlen(msg), 0, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
		oops("sendto failed")
	return 0;
}
