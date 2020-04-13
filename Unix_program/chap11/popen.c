#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

FILE *Mypopen(char *command, char *mode)
{
	int pfp[2], pid;
	FILE *fp;
	int parent_end, child_end;

	if (*mode == 'r') {
		parent_end = READ;
		child_end = WRITE;
	} else if (*mode == 'w') {
		parent_end = WRITE;
		child_end = READ;
	} else 
		return NULL;
	
	if (pipe(pfp) == -1) return NULL;
	pid = fork();
	if (pid == -1) {
		close(pfp[0]);
		close(pfp[1]);
		return NULL;
	} else if (pid > 0) {
		if (close(pfp[child_end] == -1))
			return NULL;
		return fdopen(pfp[parent_end], mode);//notice that  mode="w/r" instead of 'w/r'(*mode)
	} else {
		if (close(pfp[parent_end]) == -1) exit(1);
		if (dup2(pfp[child_end], child_end) == -1) exit(1);
		if (close(pfp[child_end]) == -1) exit(1);
		execl("/bin/bash", "bash", "-c", command, NULL);
		exit(1);
	}

}

int main(void)
{
	int i = 0;
	char buf[200];
	FILE *fp = Mypopen("who|sort", "r");
	while (fgets(buf, 100, fp) != NULL)
		printf("%3d %s", i++, buf);
	return 0;
}
