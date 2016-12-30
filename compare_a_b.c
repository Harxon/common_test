#include <stdio.h>

int fun(char *s,char *d)
{
	while(*s == *d)
		if(*s == '\0')
			return 0;
		else
			s++,d++;
	return *s-*d;
}
int main(int argc, const char *argv[])
{
	char *a = "123";
	char *b = "12";
	printf("%d\n",fun(a,b));
	return 0;
}
