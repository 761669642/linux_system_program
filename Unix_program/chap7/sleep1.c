#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void wakeup(int);

int main(void)
{
	printf("about to sleep 4s\n");
	signal(SIGALRM, wakeup);
	alarm(4);
	pause();
	printf("Morning is so soon?\n");
}

void wakeup(int signum)
{
	printf("Alarm signal received from the kernel!\n");
}
