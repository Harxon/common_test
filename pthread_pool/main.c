#include<stdio.h>
#include <unistd.h>
#include "pthread_pool.h"

void* my_printf(void* arg){
	printf("%d\n",(int)arg);
	return (void*)0;
}
int main(int argc, const char *argv[])
{
	pool_t* pt = NULL;
	pt = pthread_pool_create(4);
	sleep(1);
	pour_task_method(pt,my_printf,(void*)1);
	wake_pthread(pt);
	getchar();
	return 0;
}
