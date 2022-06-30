#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int i;
	struct hostent *host;
	if(argc != 2) {
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}

	//성공 시 hostent 구조체 변수의 주소값, 실패시 NULL 포인터 반환
	host = gethostbyname(argv[1]);
	if(!host)
		error_handling("gethost...error!");

	//공식 도메인 이름
	printf("Official name : %s \n", host->h_name);
	//공식 도메인 이름 이외의 도메인 이름
	for(i=0;host->h_aliases[i];i++)
		printf("Aliases %d : %s \n", i+1, host->h_aliases[i]);
	//주소정보 타입(AF_INET : IPv4)
	printf("Address type : %s \n", (host->h_addrtype == AF_INET)?"AF_INET" : "AF_INET6");
	//IP주소정보
	for(i=0;host->h_addr_list[i];i++)
		printf("IP addr %d : %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
