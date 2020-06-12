#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void sig_handler1(int arg)
{
	printf("thread1 get signal\n");
}

void sig_handler2(int arg)
{
	printf("thread2 get signal\n");
}

void *thread_fun1(void *arg)
{
	printf("new thread 1\n");
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigaddset(&act.sa_mask, SIGQUIT);
	act.sa_handler = sig_handler1;
	sigaction(SIGQUIT, &act, NULL);
	pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
	sleep(2);
	return (void *)0;
}


void *thread_fun2(void *arg)
{
	printf("new thread 2\n");
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigaddset(&act.sa_mask, SIGQUIT);
	act.sa_handler = sig_handler2;
	sigaction(SIGQUIT, &act, NULL);
	//pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
	sleep(2);
	return (void *)0;
}

int main()
{
	pthread_t tid1, tid2;
	int ret, s;
	ret = pthread_create(&tid1, NULL, thread_fun1, NULL);
	if (ret) {
		perror("create thread 1 failed");
		return 0;
	}
	ret = pthread_create(&tid2, NULL, thread_fun2, NULL);
	if (ret) {
		perror("create thread 2 failed");
		return 0;
	}
	sleep(1);

	ret = pthread_kill(tid1, SIGQUIT);
	if (ret) {
		perror("kill thread 1 failed");
		return 0;
	}
	ret = pthread_kill(tid2, SIGQUIT);
	if (ret) {
		perror("kill thread 2 failed");
		return 0;
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;

}
