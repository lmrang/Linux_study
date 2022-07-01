//자식 프로세스 파일 디스크립터와 부모 프로세스 파일 디스크립터 비교하기
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;

	sock=socket(PF_INET, SOCK_STREAM, 0);	//소켓생성, 파일디스크립터 반환

	pid=fork();
	if(pid==0)	//자식
		printf("Child sock : %d\n", sock);
	else		//부모
		printf("Parent sock : %d\n", sock);

	return 0;
}
