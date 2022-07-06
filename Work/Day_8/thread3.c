#include <stdio.h>
#include <pthread.h>
void *thread_summation(void* arg);
int sum=0;	//전역변수(data) 공유되는 영역

int main(int argc, char *argv[])
{
	pthread_t id_t1, id_t2;
	int range1[]={1, 5};
	int range2[]={6, 10};

	pthread_create(&id_t1, NULL, thread_summation, (void*)range1);	//쓰레드 1 생성
	pthread_create(&id_t2, NULL, thread_summation, (void*)range2);	//쓰레드 2 생성

	pthread_join(id_t1, NULL);	//쓰레드 1 종료
	pthread_join(id_t2, NULL);	//쓰레드 2 종료
	printf("result : %d \n", sum);
	return 0;
}

void * thread_summation(void* arg)
{
	int start=((int*)arg)[0];
	int end=((int*)arg)[1];

	while(start<=end)
	{
		sum+=start;
		start++;
	}
	return NULL;
}
