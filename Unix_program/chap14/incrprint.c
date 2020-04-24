#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int counter = 0;
void *print_count(void *);

int main(void)
{	
	pthread_t t1;
	int i;
	pthread_create(&t1, NULL, print_count, NULL);
	for (i = 0; i < 5; i++) {
		counter++;
		sleep(1);
	}
	pthread_join(t1, NULL);
}

void *print_count(void *m)
{
	int i;
	for (i = 0; i < 5; i++) {
		printf("count = %d\n", counter);
		sleep(1);
	}
	return NULL;
}


