#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);

char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;
	if(argc!=4) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	sprintf(name, "[%s]", argv[3]);		//name [name]형식으로 만들기
	sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");

	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);	//보내는 쓰레드 생성
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);	//받는 쓰레드 생성
	pthread_join(snd_thread, &thread_return);	//쓰레드(snd_thread) 종료를 대기
	pthread_join(rcv_thread, &thread_return);	//쓰레드(rcv_thread) 종료를 대기
	close(sock);	//소켓 닫기
	return 0;
}

void * send_msg(void * arg)
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	while(1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{//q, Q 입력시 소켓 닫기
			close(sock);
			exit(1);
		}
		sprintf(name_msg, "%s %s", name, msg);	//msg 생성
		write(sock, name_msg, strlen(name_msg));//출력버퍼로 내보내기
	}
	return NULL;
}
void * recv_msg(void * arg)
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	while(1)
	{
		str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);	//입력버퍼에 있는 메세지 읽기
		if(str_len==-1)	//오류
			return (void*)-1;
		name_msg[str_len]=0;
		fputs(name_msg, stdout);	//메세지 모니터에 출력
	}
	return NULL;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

