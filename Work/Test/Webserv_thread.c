#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>

void * handle_clnt (void *arg);
void send_file(char *file_name, int clnt_sock);
void error_handling(char *msg);

char webpage[] = "HTTP/1.1 200 OK\r\n"
		"Server:Linux Web Server\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n\r\n"
		"<!DOCTYPE html>\r\n"
		"<html><head><title> My Web Page </title>\r\n"
		"<style>body {background-color: #FFFF00 }</style></head>\r\n"
		"<body><center><h1>재롱이가 나라를 구한다!</h1><br>\r\n"
	    "<img src=\"index.jpg\"></center></body></html>\r\n";

pthread_mutex_t mutx;
int clnt_socks[256];
int clnt_cnt=0;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if(argc!=2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error!");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error!");

	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++]=clnt_sock;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP : %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	char buf[1024];
	char *file_name;
	int i;

	read(clnt_sock, buf, sizeof(buf));
	file_name=strstr(buf, "/");
	file_name=strtok(file_name, " ");

	if(file_name!=NULL) send_file(file_name, clnt_sock);

	pthread_mutex_lock(&mutx);
	for(i=0;i<clnt_cnt;i++)
	{
		if(clnt_sock==clnt_socks[i])
		{
			while(i++<clnt_cnt-1)
				clnt_socks[i]=clnt_socks[i+1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

void send_file(char *file_name, int clnt_sock)
{
	int i;
	int fd;
	char img_buf[2000000];

	if(strcmp(file_name, "/index.jpg")==0)
	{
		fd=open("index.jpeg", O_RDONLY);
		read(fd, img_buf, sizeof(img_buf));
		write(clnt_sock, img_buf, sizeof(img_buf));
	}
	else
		write(clnt_sock, webpage, sizeof(webpage));
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
