#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define oops(m,x) {perror(m); exit(x);}
#define fatal(m,x) {fprintf(stderr, m); exit(x);}

void be_dc(int *, int *);
void be_bc(int *, int *);

int main(void)
{
	int pid, todc[2], fromdc[2];
	if (pipe(todc) == -1 || pipe(fromdc) == -1)
		oops("pipe create failed", 1)
	pid = fork();
	if (pid == -1)
		oops("fork failed", 2)
	else if (pid == 0)
		be_dc(todc, fromdc);
	else {
		be_bc(todc, fromdc);
		wait(NULL);
	}
}

void be_dc(int in[], int out[])
{	
	if (dup2(in[0], 0) == -1)
		oops("dup todc0 to stdin failed", 3)
	close(in[0]);
	close(in[1]);
	if (dup2(out[1], 1) == -1)
		oops("dup fromdc1 to stdout failed", 4)
	close(out[1]);
	close(out[0]);

	execlp("dc", "dc", "-", NULL);
	oops("run dc failed", 5)
}

void be_bc(int todc[], int fromdc[])
{
	int num1, num2;
	char operation[200], message[200];
	FILE *fpout, *fpin;

	close(todc[0]);
	close(fromdc[1]);
	fpout = fdopen(todc[1], "w");
	fpin = fdopen(fromdc[0], "r");
	if (fpout == NULL || fpin == NULL)
		fatal("Failed convering pipes to streams\n", 1)
	while (printf("tinybc: "), fgets(message, 200, stdin) != NULL) {
		if (sscanf(message, "%d%[-+*/^]%d", &num1, operation, &num2) != 3) {
			fprintf(stderr, "math syntax error!\n");
			continue;
		}
		if (fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
			fatal("writing to fpout failed\n", 1)
		fflush(fpout);
		memset(message, 0, 200);
		if (fgets(message, 200, fpin) == NULL) break;
		printf("%d %c %d = %s\n", num1, *operation, num2, message);
	}
	fclose(fpout);
	fclose(fpin);
}	
