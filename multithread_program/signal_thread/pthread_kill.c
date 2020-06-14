#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

void *thread_func(void *arg)
{
	sleep(1);
	printf("this is a new thread\n");
	return (void *)0;
}

int main()
{
	int ret;
	pthread_t tid;
	void *rval;
	ret = pthread_create(&tid, NULL, thread_func, NULL);
	if (ret) {
		perror("create thread failed");
		return 0;
	}
//	sleep(1);
	printf("tid is %d\n", tid);
	ret = pthread_kill(tid, SIGQUIT);
//	if (ret) {
//		fprintf(stderr, "the thread is not killed, there may be errors\n");
//	}

	ret = pthread_join(tid, &rval);
	printf("the return val of join thread is %d\n", (int)rval);
	return 0;
}
//
//
