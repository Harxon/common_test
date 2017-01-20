#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tpool.h"


#define N 32
#define err_log(log)\
	do{\
		perror(log);\
		exit(1);\
	}while(0)
typedef struct sockaddr SA;

void* my_task_handler(void *arg){

	ssize_t bytes;
	int clientfd = (int)arg;
	char buf[N] = {0};
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(SA);
	//接收发送数据 
	printf("%s, clientfd:%d\n",__func__, clientfd);
	while(1)
	{
		bytes = recv(clientfd,buf,sizeof(buf),0);
		if(bytes==0)
		{
			puts("peer exit");
			break;
		}else if(bytes <0)
		{
			perror("fail to recv");
			break;
		}
		puts(buf);
		bzero(buf,sizeof(buf));
		send(clientfd,"hello client",12,0);
	}
	close(clientfd);
	return NULL;
}

int main(int argc, const char *argv[])
{
	int serverfd,clientfd;
	struct sockaddr_in serveraddr,clientaddr;
	socklen_t len=sizeof(SA);
	//	char buf[N]={0};
	bzero(&serveraddr,len);
	bzero(&clientaddr,len);
#if 0
	if(argc != 3)
	{
		fprintf(stdout,"<%s IP PORT>",argv[0]);
		return -1;
	}
#endif
	//1.创建流式套接字
	if((serverfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		err_log("fail to socket");
	}
	//2.填充本地的网络信息
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi("8888"));
	serveraddr.sin_addr.s_addr=inet_addr("192.168.1.7");

	//3.绑定套接字
	if(bind(serverfd,(SA*)&serveraddr,len)<0)
	{
		close(serverfd);
		err_log("fail to bind");
	}
	//4.监听套接字
	if(listen(serverfd,5)<0)
	{
		close(serverfd);
		err_log("fail to listen");
	}
	//创建线程池
	tpool_t *tp;
	tp=tpool_create(10);


	while(1)
	{
		//5.链接客户端请求
		if((clientfd=accept(serverfd,(SA*)&clientaddr,&len))<0)
		{
			perror("fail to accept");
			continue;
		}
		printf("clientfd %d\n",clientfd);
		printf("%s, my_task_handler:%p\n",__func__, my_task_handler);
		tpool_add_task(tp,my_task_handler,(void*)clientfd);
	}
	close(serverfd);
	return 0;
}
