#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void f(int);

int main(void)
{
	int i;
	signal(SIGINT, f);
	for (i = 0; i < 5; i++) {
		printf("hello\n");
		sleep(1);
	}
}

void f(int signum)
{
	printf("OUCH!\n");
}
