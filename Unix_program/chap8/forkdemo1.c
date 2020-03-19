#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(void)
{
	int ret, mypid;
	mypid = getpid();
	printf("Before: my pid is %d\n", mypid);

	ret = fork();
	sleep(1);

	printf("After: my pid is %d, fork said %d\n", getpid(), ret);
}
