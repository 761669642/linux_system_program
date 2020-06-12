#include <pthread.h>
#include <stdio.h>

#define BUFF_SIZE 5
//#define PRODUCT_CNT 30

struct product_cons {
	int buffer[BUFF_SIZE];
	pthread_mutex_t lock;
	int readpos, writepos;
	pthread_cond_t notempty;
	pthread_cond_t notfull;
} buffer;

void init(struct product_cons *p)
{
	pthread_mutex_init(&p->lock, NULL);
	pthread_cond_int(&notempty, NULL);
	pthread_cond_int(&notfull, NULL);
	p->readpos = 0;
	p->writepos = 0;
}

void finish(struct product_cons *p)
{
	pthread_mutex_destory(&p->lock);
	pthread_cond_destory(&notempty);
	pthread_cond_destroy(&notfull);
	p->readpos = 0;
	p->writepos = 0;
}

void put(struct product_cons *p, int data)
{
	pthread_mutex_lock(&p->lock);
	if ((p->writepos+1) % BUFF_SIZE == p->readpos) {
		printf("producer wait for not full");
		pthread_cond_wait(&p->notfull, &p->lock);
	}
	p->buffer[p->writepos] = data;
	if (p->writepos

	pthread_mutex_unlock(&p->lock);
}

int get(struct product_cons *p)
{
	
}

void *producer(void *data)
{
	int i;
	for (i = 1; i <= 50; i++) {
		sleep(1);
		printf("put the %d product\n", i);
		put(&buffer, i);
		printf("put the %d product success!!!!\n", i);
	}
	printf("producer stopped");
	return NULL;
}

void *consumer(void *data)
{
	static int cnt = 0;
	int num;
	while (1) {
		sleep(2);
		printf("get product");
		num = get(&buffer);
		printf("get the %d product success!!!!\n", num);
		if (cnt++ == 30) break;
	}

	printf("consumer stopped\n");
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	void *ret;
	init(&buffer);
	pthread_create(&tid1, NULL, producer, NULL);
	pthread_create(&tid2, NULL, consumer, NULL);

	pthread_join(tid1, &ret);
	pthread_join(tid2, &ret);

	finish(&buffer);
	return 0;
}
