#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out!");
	alarm(2);
	//2초후 시그널 전송
}

int main(int argc, char *argv[])
{
	int i;
	struct sigaction act;

	act.sa_handler=timeout;
	//시그널이 오면 timeout 실행

	sigemptyset(&act.sa_mask);
	//모든 시그널은 시그널을 처리하는 동안 블록되도록 설정

	act.sa_flags=0;
	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for(i=0;i<3;i++)
	{
		puts("wait....");
		sleep(100);
	}
	return 0;
}
