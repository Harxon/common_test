#include <stdio.h>
#include <strings.h>

int main(int argc, const char *argv[])
{
	int ret = 0;
	int d = 2;
	char s[32] = {0};

	do{
		d--;
		puts("#####begin scanf########");
		printf("Input a string>>");
		scanf("%s",s);
		//getchar();
		//puts("getchar() get a '\\n'...");
		printf("%s\n",s);
	}while(d);

	bzero(s,sizeof(s));
	//scanf不会把回车换行符号接受到自己的字符串中
	//fgets会接受任何输入的字符，包括最后作为结束的换行符号
	//使用完scanf以后需要使用getchar来处理scanf未接受的垃圾字符，如（‘\n’），否则fgets会接受到这样的垃圾字符
	
	do{
		puts("#####begin fgets########");
		printf("fgets a string>>");
		fgets(s,sizeof(s),stdin);
		//getchar();
		//puts("getchar() get a '\\n'...");
		printf("%s\n",s);
	}while(0);

	return 0;
}
