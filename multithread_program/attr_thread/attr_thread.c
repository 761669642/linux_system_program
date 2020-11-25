#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func1(void *arg)
{
	printf("This is the thread 1\n");
	return (void *)0;
}

void *thread_func2(void *arg)
{
	printf("This is the thread 2\n");
	return (void *)0;
}

int main(void)
{
	pthread_t tid1, tid2;
	int err;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	err = pthread_create(&tid1, &attr, thread_func1, NULL);
	if (err) {
		perror("create thread 1 failed!");
		return 0;
	}

	err = pthread_create(&tid2, NULL, thread_func2, NULL);
	if (err) {
		perror("create thread 2 failed!");
		return 0;
	}

	err = pthread_join(tid1, NULL);
	if (err)
		perror("thread 1 join failed");
	else 
		printf("thread 1 join success\n");

	err = pthread_join(tid2, NULL);
	if (err)
		perror("thread 2 join failed");
	else 
		printf("thread 2 join success\n");
	
	pthread_attr_destroy(&attr);
	return 0;

}
