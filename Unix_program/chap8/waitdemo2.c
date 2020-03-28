#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELAY 2

void child_code(int);
void parent_code(int);

int main(void)
{
	int pid;

	printf("Before: my pid is %d\n", getpid());
	if ((pid = fork()) == -1)
		perror("fork");
	else if (pid == 0)
		child_code(DELAY);
	else
		parent_code(pid);
}

void child_code(int delay)
{
	printf("child %d here, will sleep for %d seconds\n", getpid(), delay);
	sleep(delay);
	printf("child down, then exit.\n");
	exit(17);
}

void parent_code(int childpid)
{
	int wait_ret;
	int exit_num;
	int high_8, low_7, bit_7;
	wait_ret = wait(&exit_num);
	printf("done waiting for %d, wait returned: %d\n, the exit number is 0x%2x\n", childpid, wait_ret, exit_num);

	high_8 = exit_num >> 8;
	low_7 = exit_num % (1<<7);
	bit_7 = exit_num & (1<<7);
	printf("status: exit = %d, sig = %d, core = %d\n", high_8, low_7, bit_7);
}
