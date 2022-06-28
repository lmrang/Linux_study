#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[] = "Hello World!";

	if(argc!=2)
	{
		printf("Usage : %s <prot>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);	//IPv4, TCP, protocol
	if(serv_sock == -1)								//실패시 -1, 성공시 파일 디스크립터
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));	//초기화
	serv_addr.sin_family=AF_INET;				//주소체계
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);//32비트 IPv4 인터넷 주소, 
	serv_addr.sin_port=htons(atoi(argv[1]));	//16비트 TCP/UDP PORT번호

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
	//주소정보를 할당할 소켓의 파일 디스크립터, 주소정보 구조체, 구조체 변수의 길이
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)		//연결요청 대기 큐로 보내는  소켓(문지기역할, 최대 5개)
		error_handling("listen() error");

	clnt_addr_size=sizeof(clnt_addr);	//실질적으로 데이터를 송수신하는 소켓
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1)
		error_handling("accept() error");

	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
