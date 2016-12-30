#include <stdio.h>

int main(int argc, const char *argv[])
{
	int a = -3;
	int b = 3;
	a = a & b;
	//运算都是计算机在执行，所以“a & b”中的“a”“b”,都是以补码的方式参与运算
	printf("a:%d\n",a);
	//打印是为了给人看，所以此处a是源码

	return 0;
}
