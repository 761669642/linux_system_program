#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *thread_func(void *arg)
{
	sleep(1);
	if (strcmp("1", (char *)arg) == 0)
		return (void *)1;
	else if (strcmp("2", (char *)arg) == 0)
		pthread_exit((void *)2);
	else if (strcmp("3", (char *)arg) == 0)
		exit(3);
}

int main(int argc, char **argv)
{
	int ret;
	pthread_t tid;
	void *rval;
	ret = pthread_create(&tid, NULL, thread_func, (void *)argv[1]);
	if (ret)
		perror("create thread failed");
	
	pthread_join(tid, &rval);
	printf("the join return val is %d\n", (int)rval);
}
