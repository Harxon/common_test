#include <stdio.h>
#include <pthread.h>

void* start_routine(void* arg){
	
	sleep(2);
	printf("%d\n",*(int*)arg);
	*(int*)arg = 0;
	printf("%d\n",*(int*)arg);
	//return (void*)0;
}

int main(int argc, const char *argv[])
{
	pthread_t thread;
#if 0
	volatile int v = 1;
#else
	int v = 2;
#endif
	pthread_create(&thread,NULL,start_routine,&v);
	while(v)
	{
		puts("volatile");
		sleep(2);
	}
	return 0;
}
