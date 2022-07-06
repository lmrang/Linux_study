#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>
#define BUF_SIZE 2048
void error_handling (char *msg);
char webpage[] = "HTTP/1.1 200 OK\r\n"
		"Server:Linux Web Server\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n\r\n"
		"<!DOCTYPE html>\r\n"
		"<html><head><title> My Web Page </title>\r\n"
		"<style>body {background-color: #FFFF00 }</style></head>\r\n"
		"<body><center><h1>Hello world!!</h1><br>\r\n"
		"<img src=\"index.jpg\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	char img_buf[1000000];
	char buf[BUF_SIZE];
	int n, fd;
	int len;
	//pthread_t t_id;

	if(argc!=2)	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error!");
	if(listen(serv_sock, 5) ==-1)
		error_handling("listen() eror!");

	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		printf("connection....");
		read(clnt_sock, buf, BUF_SIZE);
		printf("%s", buf);

		write(clnt_sock, webpage, sizeof(webpage));		//클라이언트에게 송신

		n=read(clnt_sock, buf, BUF_SIZE);
		printf("%s", buf);

		fd=open("index.jpeg", O_RDONLY);
		len = read(fd, img_buf, sizeof(img_buf));
		write(fd, img_buf, len);
		close(fd);
	}
	close(clnt_sock);
	close(serv_sock);

	return 0;
}


void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
