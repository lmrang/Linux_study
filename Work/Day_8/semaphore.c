#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void * arg);
void * accu(void * arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char *argv[])
{
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);
	return 0;
}

void *read(void *arg)
{//세마포어가 1인 sem_two를 가진 read먼저 실행된다.
	int i;
	for(i=0;i<5;i++)
	{
		fputs("Input num : ", stdout);

		sem_wait(&sem_two);
		//세마포어 키 획득 (sem_two-1)
		scanf("%d", &num);
		sem_post(&sem_one);
		//세마포어 키 반납 (sem_one+1)
	}
	return NULL;
}
void *accu(void *arg)
{
	int sum =0, i;
	for(i=0;i<5;i++)
	{
		sem_wait(&sem_one);
		sum+=num;
		sem_post(&sem_two);
	}
	printf("Result : %d \n", sum);
	return NULL;
}

//세마포어의 키는 0이 아닌 값 모두 가능?
//post()를 계속 실행하면 값은 누적?
