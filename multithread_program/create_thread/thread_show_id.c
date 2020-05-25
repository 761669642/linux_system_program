#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

void *print_id(void *s)
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();

	printf("%s: process id is %u and thread id is 0x%x\n", (char *)s, pid, tid);
	return (void *)0;
}

int main(void)
{
	pthread_t ntid;
	int ret;
	ret = pthread_create(&ntid, NULL, print_id, (void *)"son thread");
	if (ret) {
		perror("pthread_create failed");
		exit(1);
	}
	sleep(2);
	print_id("main thread");
}
