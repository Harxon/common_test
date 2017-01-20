#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define N 32
#define err_log(log)\
	do{\
		perror(log);\
		exit(1);\
	}while(0)
typedef struct sockaddr SA;

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr;
	socklen_t len=sizeof(SA);
	bzero(&serveraddr,len);
	char buf[N]={"hold on!..."};
	if(argc != 3)
	{
		fprintf(stdout,"<%s IP PORT>",argv[0]);
	//	return -1;
	}

	//1.创建流式套接字
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		err_log("fail to socket");
	}
	//2.填充服务器的网络信息
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi("8888"));
	serveraddr.sin_addr.s_addr=inet_addr("192.168.1.7");


	//3.请求链接服务器
	if(connect(sockfd,(SA*)&serveraddr,len)<0)
	{
		close(sockfd);
		err_log("fail to connect");
	}

	bzero(buf,sizeof(buf));
	while(1)
	{
#if 1
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf)-1]='\0';
#endif
		send(sockfd,buf,sizeof(buf),0);
		bzero(buf,sizeof(buf));

		recv(sockfd,buf,sizeof(buf),0);
		puts(buf);
		bzero(buf,sizeof(buf));
	}
	close(sockfd);
	



	return 0;
}
