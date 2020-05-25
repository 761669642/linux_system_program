#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *thread_fun(void *arg)
{
	int ret;
	ret = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if (ret) {
		perror("pthread set cancel state disable fialed\n");
	}
	printf("I am new thread\n");
	sleep(4);

	printf("about to cancle\n");
	ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (ret)
		perror("pthread set cancel state enable failed\n");
	printf("now use printf to cancel once\n");
	printf("then use printf to cancle twice\n");

	return (void *)20;
}

int main()
{
	pthread_t tid;
	void *rval;
	int ret = 0;
	ret = pthread_create(&tid, NULL, thread_fun, NULL);
	if (ret) {
		perror("create thread failed");
		return 0;
	}
	sleep(2);

	ret = pthread_cancel(tid);
	if (ret)
		perror("cancel thread failed");
	
	ret = pthread_join(tid, &rval);
	if (ret) 
		perror("join thread failed");
	
	printf("the join thread cancel return val is %d\n", (int *)rval);
	return 0;
}
