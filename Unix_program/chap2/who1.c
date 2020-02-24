#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST

void show_info(struct utmp *utrecord)
{
	printf("%-8.8s ", utrecord->ut_name);
	printf("%-8.8s ", utrecord->ut_line);
	printf("%10ld ", utrecord->ut_time);
#ifdef SHOWHOST
	printf("(%s)", utrecord->ut_host);
#endif
	printf("\n");
}

int main()
{
	struct utmp current_record;
	int    utmpfd;
	int    reclen = sizeof(current_record);
	//the "UTMP_FILE" is the marco of path of the utmp file in <utmp.h>
	if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
		perror(UTMP_FILE);
		exit(1);
	}
	while (read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
}


