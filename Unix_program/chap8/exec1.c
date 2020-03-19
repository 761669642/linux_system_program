#include <unistd.h>
#include <stdio.h>

int main(void)
{
	char *argv[3] = {
		"ls",
		"-l",
		NULL
	};
	printf("*** About to exec ls -l\n");
	execvp("ls", argv);
	printf("*** ls is done. bye\n");
}
