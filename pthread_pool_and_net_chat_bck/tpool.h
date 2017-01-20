#ifndef __TPOOL_H__
#define __TPOOL_H__
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>

typedef struct task{ //任务节点
	void *(*func)(void *);
	void *arg;
	struct task *next;
}task_t;

typedef struct tpool{
	task_t *mytask;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	pthread_t *tid;
}tpool_t;

tpool_t *tpool_create(int num);
void tpool_add_task(tpool_t *tp,void *(*func)(void *),void *arg);
#endif
