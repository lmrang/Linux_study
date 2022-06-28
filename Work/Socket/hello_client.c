#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if(argc!=3)
	{
		printf("Usage : %s <IP> <prot>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);				//소켓생성
	if(sock == -1)
		error_handling("socket() error");

		memset(&serv_addr, 0, sizeof(serv_addr));		//초기화
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
		serv_addr.sin_port=htons(atoi(argv[2]));

		if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)	//실패시 -1
		//클라이언트 소켓의 파일 디스크립터, 서버의 주소정보, 구조체 크기
			error_handling("connect() error!");
		/*클라이언트 소켓 주소정보는?
		  connect 함수가 호출될때 운영체제(커널)에서
		  IP는 컴퓨터에 할당된, PORT는 임의로 선택해서 할당된다.
		  (명시적으로 호출할 필요없음!!) */

		str_len=read(sock, message, sizeof(message)-1);
		if(str_len == -1)
			error_handling("read() error!");

		printf("Message from server : %s \n", message);
		close(sock);
		return 0;
	}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
