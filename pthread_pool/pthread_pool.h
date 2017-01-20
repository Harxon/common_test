#ifndef __PTHREAD_POOL__
#define __PTHREAD_POOL__ 
#include <stdlib.h>
#include <pthread.h>
typedef struct task_node_info{
	void* (*routine_handler)(void* arg);
	void* arg;

	struct task_node_info* next;
}pool_task_t;

typedef struct pool_info{
	pthread_t* tid_p;
	pthread_mutex_t lock;
	pthread_cond_t cond;

	pool_task_t* task_p;
}pool_t;

void* start_routine(void* arg);
pool_t* pthread_pool_create(int cnt);
int pour_task_method(pool_t* pt,void*(*func)(void*arg),void* arg);
int wake_pthread(pool_t* pt);
#endif
