#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr;
	struct hostent *hp;
	int sock_id, sock_fd;
	char message[200];
	int messlen;

	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket")
	bzero(&servaddr, sizeof(servaddr));
	hp = gethostbyname(argv[1]);
	if (hp == NULL) 
		oops(argv[1])
	bcopy(hp->h_addr, (struct sockaddr *)&servaddr.sin_addr, hp->h_length);
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_family = AF_INET;
	if (connect(sock_id, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
		oops("connect");

	messlen = read(sock_id, message, 200);
	if (messlen == -1)
		oops("read socket")
	printf("%s", message);
	close(sock_id);
}
