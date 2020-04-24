#include <stdio.h>
#include <pthread.h>

void *print_msg(void *msg);

int main(void)
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, print_msg, (void *)"hello world");
	pthread_create(&t2, NULL, print_msg, (void *)"goodbye world");
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}

void *print_msg(void *msg)
{
	int i;
	for (i = 0; i < 5; i++) {
		printf("%s\n", (char *)msg);
		sleep(1);
	}
	return NULL;
}
