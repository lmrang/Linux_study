#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void *arg);
void send_msg(char *msg, int len);
void error_handling(char *msg);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);			//쓰레드 초기화
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);	//소켓 생성

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error!");
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);			//임계영역 시작
		clnt_socks[clnt_cnt++]=clnt_sock;	//클라이언트 추가
		pthread_mutex_unlock(&mutx);		//임계영역 끝

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);	//쓰레드 생성(handle_clnt)
		pthread_detach(t_id);	//쓰레드 소멸
		printf("Connected client IP : %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);	//소켓 닫기
	return 0;
}

void * handle_clnt(void* arg)
{
	int clnt_sock = *((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];

	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)	//입력버퍼 읽기
		send_msg(msg, str_len);

	pthread_mutex_lock(&mutx);	//임계영역 시작
	for(i=0;i<clnt_cnt;i++)	//클라이언트 숫자 만큼 반복
	{
		if(clnt_sock==clnt_socks[i])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;	//클라이언트 숫자 감소
	pthread_mutex_unlock(&mutx);	//임계영역 끝
	close(clnt_sock);
	return NULL;
}
void send_msg(char *msg, int len)
{
	int i;
	pthread_mutex_lock(&mutx);	//임계영역 시작
	for(i=0;i<clnt_cnt;i++)
		write(clnt_socks[i], msg, len);	//출력버퍼로 내보내기
	pthread_mutex_unlock(&mutx);//임계영역 끝
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
