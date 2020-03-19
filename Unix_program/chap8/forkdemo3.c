#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
	int ret;
	printf("Before: my pid is %d\n", getpid());

	ret = fork();
	if (ret == -1)
		perror("fork");
	else if (ret == 0)
		printf("I am the child, my pid is %d\n", getpid());
	else
		printf("I am the parent, my pid is %d and my child pid is %d\n", getpid(), ret);
}
