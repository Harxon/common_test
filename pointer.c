#include <stdio.h>

int main(int argc, const char *argv[])
{
	int* p = NULL;

	p = 0;
	*p = 0;
	printf("%p\n",p);
	return 0;
}
