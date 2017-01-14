#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 #include <string.h>


int main (int argc, char *argv[])
{
	int fd = open("./text",O_RDONLY);
	if(fd < 0)
	{
		perror("open test error\n");
		exit(1);
	}
	
	
	int new_len = lseek(fd,0,SEEK_END);
	int len = new_len;
	char buf[100] = {0};
	
	int i=0;
	while(1)
	{	
		len = new_len;
		new_len = lseek(fd,0,SEEK_END);	
	
		if(new_len != len)
		{
			lseek(fd,len-new_len,SEEK_END);
			
			printf("cur pos = %d\n",lseek(fd,0,SEEK_CUR));
			printf("len-new_len =%d\n",len-new_len);
			printf("new_len - len=%d\n",new_len-len);
			
			i = read(fd,buf,new_len-len);
			printf("read %d bytes,%s\n",i,buf);
			memset(buf,0,100);
		}	
		
	}
	
	close(fd);
	return 0;
}
