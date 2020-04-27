#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

struct arg_set {
	char *fname;
	int count;
};

void *count_words(void *);
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	pthread_t t1,t2;
	struct arg_set args1, args2;
	if (argc != 3) {
		printf("usage: %s file1 file2\n", argv[0]);
		exit(1);
	}
	args1.fname = argv[1];
	args1.count = 0;
	pthread_create(&t1, NULL, count_words, (void *)&args1);
	args2.fname = argv[2];
	args2.count = 0;
	pthread_create(&t2, NULL, count_words, (void *)&args2);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	printf("%5d: %s\n", args1.count, args1.fname);
	printf("%5d: %s\n", args2.count, args2.fname);
	printf("%5d: total words\n", args1.count+args2.count);
}

void *count_words(void *f)
{
	struct arg_set *args = f;
	FILE *fp;
	int c, prevc = '\0';
	fp = fopen(args->fname, "r");
	if (fp == NULL) {
		perror("fopen faild");
		exit(1);
	}
	while ((c = getc(fp)) != EOF) {  //here, variable c will read '\n' as a char
		if (!isalnum(c) && isalnum(prevc))
			args->count++;
		prevc = c;
	}
	if (prevc == '\n')
		printf("the last char is %c", prevc);
	fclose(fp);
	return NULL;
}
