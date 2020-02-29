#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int main(void)
{
	int i;
	signal(SIGINT, SIG_IGN);
	for (i = 0; i < 5; i++) {
		sleep(1);
		printf("HAHA, you can't stop me!!\n");
	}
}
