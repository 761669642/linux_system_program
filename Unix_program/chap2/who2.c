#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define NUM 16
#define NULLUT ((struct utmp *) NULL)
#define UTSIZE (sizeof(struct utmp))

//who2.c add the buffer to buffering the data read from file

char utmpbuf[NUM*UTSIZE];
int num_recs;
int cur_rec;
int fd_utmp = -1;

int utmp_open(char *filename)
{
	fd_utmp = open(filename, O_RDONLY);
	cur_rec = num_recs = 0;
	return fd_utmp;
}

int utmp_reload()
{
	int amt_read;
	amt_read = read(fd_utmp, utmpbuf, NUM*UTSIZE);
	num_recs = amt_read/UTSIZE;
	cur_rec = 0;
	return num_recs;
}

struct utmp* utmp_next()
{
	struct utmp *recp;
	if (fd_utmp == -1)
		return NULLUT;
	if (cur_rec == num_recs && utmp_reload() == 0)
		return NULLUT;
	recp = (struct utmp*) &utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
	return recp;
}

utmp_close()
{
	if (fd_utmp != -1)
		close(fd_utmp);
}

void show_info(struct utmp *utrecord)
{
	printf("%-8.8s ", utrecord->ut_name);
	printf("%-8.8s ", utrecord->ut_line);
	printf("%10ld ", utrecord->ut_time);
	printf("(%s)", utrecord->ut_host);
	printf("\n");
}

int main()
{
	struct utmp *utbufp;	
	//the "UTMP_FILE" is the marco of path of the utmp file in <utmp.h>
	if ((utmp_open(UTMP_FILE)) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}
	while ((utbufp = utmp_next()) != NULL)
		show_info(utbufp);
	utmp_close(fd_utmp);
	return 0;
}


