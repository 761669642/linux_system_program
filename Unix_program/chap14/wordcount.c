#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

int total_words;
void *count_words(void *);

int main(int argc, char **argv)
{
	pthread_t t1,t2;
	if (argc != 3) {
		printf("usage: %s file1 file2\n", argv[0]);
		exit(1);
	}
	total_words = 0;
	pthread_create(&t1, NULL, count_words, (void *)argv[1]);
	pthread_create(&t2, NULL, count_words, (void *)argv[2]);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("%5d: total words\n", total_words);
}

void *count_words(void *f)
{
	char *filename = (char *)f;
	FILE *fp;
	int c, prevc = '\0';
	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("fopen faild");
		exit(1);
	}
	while ((c = getc(fp)) != EOF) {  //here, variable c will read '\n' as a char
		if (!isalnum(c) && isalnum(prevc))
			total_words++;
		prevc = c;
	}
	if (prevc == '\n')
		printf("the last char is %c", prevc);
	fclose(fp);
	return NULL;
}