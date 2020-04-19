#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>

#define HOSTLEN 256
#define BACKLOG 1
#define oops(msg) {perror(msg); exit(1);}

static int make_server_socket_q(int, int);

int make_server_socket(int portnum)
{
	return make_server_socket_q(portnum, BACKLOG);
}

static int make_server_socket_q(int portnum, int backlog)
{
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sock_id;

	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket failed")
	bzero((void *)&saddr, sizeof(saddr));
	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);
	if (hp == NULL)
		oops("get host name failed")
	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(portnum);
	saddr.sin_family = AF_INET;
	if (bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		oops("bind failed")
	
	if (listen(sock_id, backlog) != 0)
		oops("listen failed")
	return sock_id;
}

int connect_to_server(char *host, int portnum)
{
	struct sockaddr_in servadd;
	struct hostent *hp;
	int serv_sockid;

	serv_sockid = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sockid == -1)
		oops("socket failed")
	bzero(&servadd, sizeof(servadd));
	hp = gethostbyname(host);
	if (hp == NULL)
		oops("get host name failed")
	bcopy((void *)hp->h_addr, (void *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(portnum);
	servadd.sin_family = AF_INET;
	
	if (connect(serv_sockid, (struct sockaddr *) &servadd, sizeof(servadd)) != 0)
		oops("connect failed")
	return serv_sockid;
}

