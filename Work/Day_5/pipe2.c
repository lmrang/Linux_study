#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds[2];
	char str1[]="who are you?";
	char str2[]="Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);	//공동 메모리 영역 생성(1개)
	pid=fork();
	if(pid==0)
	{
		write(fds[1], str1, sizeof(str1));
		sleep(2);	//sleep이 없으면 write 후 바로 read를 실행하게 되어 바로 출력되고 멈춤.
		read(fds[0], buf, BUF_SIZE);
		printf("Child proc output : %s \n", buf);
	}
	else
	{
		read(fds[0], buf, BUF_SIZE);
		printf("Parent proc output : %s \n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}
	return 0;
}
