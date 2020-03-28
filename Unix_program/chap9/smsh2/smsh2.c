#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT "> "

static void setup();

int main(void)
{
	char *cmdline, *prompt, **argv;
	int result;
	
	prompt = DFL_PROMPT;
	setup();
	
	while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
		if ((argv = splitline(cmdline)) != NULL) {
			result = process(argv);
			freelist(argv);
		}
		free(cmdline);
	}
	return 0;
}

static void setup()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr, "Error: %s, %s\n", s1, s2);
	exit(n);
}
