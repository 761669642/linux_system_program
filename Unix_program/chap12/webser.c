#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include "socklib.h"

#define oops(msg) {perror(msg); exit(1);}

void read_til_crnl(FILE *);
void process_rq(char *, int);
void header();
void cannot_do();
void do_404();
int isadir();
int not_exist();
void do_ls();
char *file_type();
int ends_in_cgi(char *f);
void do_exec();
void do_cat();

void read_til_crnl(FILE *fp)
{
	char buf[200];
	while (fgets(buf, 200, fp) != NULL && strcmp(buf, "\r\n") != 0);
}

void process_rq(char *rq, int fd)
{
	char cmd[200], arg[200];
	if (fork() != 0)
		return;
	strcpy(arg, "./");
	if (sscanf(rq, "%s%s", cmd, arg+2) != 2)
		return;
	if (strcmp(cmd, "GET") != 0)
		cannot_do(fd);
	else if (not_exist(arg))
		do_404(arg, fd);
	else if (isadir(arg))
		do_ls(arg, fd);
	else if (ends_in_cgi(arg))
		do_exec(arg, fd);
	else
		do_cat(arg, fd);
}

int main(int argc, char **argv)
{
	int sock_id, fd;
	FILE *fpin;
	char request[200];
	if (argc == 1) {
		fprintf(stderr, "usage: ws portnum");
	}
	sock_id = make_server_socket(atoi(argv[1]));
	if (sock_id == -1) 
		oops("make server socket failed")
	
	while (1) {
		fd = accept(sock_id, NULL, NULL);
		fpin = fdopen(fd, "r");

		fgets(request, 200, fpin);
		printf("got a call: request = %s", request);
		read_til_crnl(fpin);

		process_rq(request, fd);
		fclose(fpin);
	}
}

void header(FILE *fp, char *content_type)
{
	fprintf(fp, "HTTP/1.0 200 OK\r\n");
	if (content_type)
		fprintf(fp, "Content-type: %s\r\n", content_type);
}

void cannot_do(int fd)
{
	FILE *fp = fdopen(fd, "w");
	fprintf(fp, "HTTP/1.0 501 Not Implemented\r\n");
	fprintf(fp, "Content-type: text/plain\r\n");
	fprintf(fp, "\r\n");
	fprintf(fp, "That command is not yet implemented\r\n");
	fclose(fp);
}

void do_404(char *item, int fd)
{
	FILE *fp = fdopen(fd, "w");
	fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
	fprintf(fp, "Content-type: text/plain\r\n");
	fprintf(fp, "\r\n");
	fprintf(fp, "The item you requested: %s\r\nis not found\r\n", item);
	fclose(fp);
}

int isadir(char *f)
{
	struct stat info;
	return (stat(f, &info) != -1 && S_ISDIR(info.st_mode));
}

int not_exist(char *f)
{
	struct stat info;
	return (stat(f, &info) == -1);
}

void do_ls(char *dir, int fd)
{
	FILE *fp;
	fp = fdopen(fd, "w");
	header(fp, "text/plain");
	fprintf(fp, "\r\n");
	fflush(fp);
	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
	execlp("ls", "ls", "-l", dir, NULL);
	perror(dir);
	exit(1);
}

char *file_type(char *f)
{
	char *cp;
	if ((cp = strrchr(f, '.')) != NULL)
		return cp +1;
	return "";
}

int ends_in_cgi(char *f)
{
	return (strcmp(file_type(f), "cgi") == 0);
}

void do_exec(char *prog, int fd)
{
	FILE *fp;
	fp = fdopen(fd, "w");
	header(fp, NULL);
	fflush(fp);
	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
	execl(prog, prog, NULL);
	perror(prog);
}

void do_cat(char *f, int fd)
{
	char *extension = file_type(f);
	char *content = "text/plain";
	FILE *fpsock, *fpfile;
	int c;

	if (strcmp(extension, "html") == 0)
		content = "text/html";
	else if (strcmp(extension, "gif") == 0)
		content = "image/gif";
	else if (strcmp(extension, "jpg") == 0)
		content = "image/jpg";
	else if (strcmp(extension, "jpeg") == 0)
		content = "image/jpeg";
	
	fpsock = fdopen(fd, "w");
	fpfile = fopen(f, "w");
	if (fpsock != NULL && fpfile != NULL) {
		header(fpsock, content);
		fprintf(fpsock, "\r\n");
		while ((c = getc(fpfile)) != EOF)
			putc(c, fpsock);
		fclose(fpfile);
		fclose(fpsock);
	}
	exit(0);
}
