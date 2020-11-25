#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	char *shm  = "myshm";
	char *shm1 = "myshm1";
	int shm_id, shm_id1;
	char *buf;
	pid_t pid;

	pthread_mutex_t *mutex;
	pthread_mutexattr_t mutexattr;

	shm_id1 = shm_open(shm1, O_RDWR|O_CREAT, 0644);
	ftruncate(shm_id1, 100);
	mutex = (pthread_mutex_t *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_id1, 0);

	pthread_mutexattr_init(&mutexattr);
	pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_PRIVATE);
	pthread_mutex_init(mutex, &mutexattr);

	shm_id = shm_open(shm, O_RDWR|O_CREAT, 0644);
	ftruncate(shm_id, 100);
	buf = (char *)mmap(NULL, 100, PROT_READ|PROT_WRITE, MAP_SHARED, shm_id, 0);

	pid = fork();
	if (pid == 0) {
		sleep(1);
		printf("This is the child process\n");
		pthread_mutex_lock(mutex);
		memcpy(buf, "hello", 6);
		printf("child buf is: %s\n", buf);
		pthread_mutex_unlock(mutex);
	} else if (pid > 0) {
		printf("This is the parent process\n");
		pthread_mutex_lock(mutex);
		memcpy(buf, "world", 6);
		sleep(3);
		printf("parent buf is: %s\n", buf);
		pthread_mutex_unlock(mutex);
	}
	pthread_mutexattr_destroy(&mutexattr);
	pthread_mutex_destroy(mutex);

	munmap(buf, 100);
	shm_unlink(shm);

	munmap(mutex, 100);
	shm_unlink(shm1);
}

