#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#define BUF_SIZE 32

int main(int argc, const char *argv[])
{
	if(argc < 3){
		perror("./a.out + src_filename + dts_filefilename");
		exit(-1);
	}
	FILE* src = fopen(argv[1], "r+");
	FILE* dts = fopen(argv[2], "w+");
	char buf[BUF_SIZE] = {};
	int r = 0,cnt = 0;

	while(fgets(buf, BUF_SIZE, src)){
		for(r=0; r<strlen(buf); r++){
			if('\n' == buf[r]){
				cnt++;
			}
		}
		fputs(buf, dts);
	//	bzero(buf, BUF_SIZE);
	}
	printf("rows:%d\n", cnt);
	
	return 0;
}
