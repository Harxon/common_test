#include "pthread_pool.h"
#include <stdio.h>
#include <error.h>

void* start_routine(void* arg){

	pool_t* pt = (pool_t*)arg;
	printf("%s, in\n",__func__);
	while(1){
		pthread_mutex_lock(&pt->lock);
		while(!pt->task_p){
			pthread_cond_wait(&pt->cond, &pt->lock);
		}
		printf("%s, do...\n",__func__);
		pool_task_t* t = pt->task_p;
		pt->task_p = t->next;
		pthread_mutex_unlock(&pt->lock);

		t->routine_handler(t->arg);
		free(t);
		t = NULL;
	}
	pthread_exit(0);
	return (void*)0;
}

pool_t* pthread_pool_create(int cnt){
	int i = 0;
	//init pool
	pool_t* pt = NULL;
	if(NULL == (pt = malloc(sizeof(pool_t)))){
		perror("malloc pt failed");
		return NULL;
	}
	pthread_mutex_init(&pt->lock, NULL);
	pthread_cond_init(&pt->cond, NULL);
	pt->task_p = NULL;

	//apply tid for pthreads
	if(NULL == (pt->tid_p = malloc(sizeof(pthread_t)*cnt))){
		perror("malloc tid_p failed");
		free(pt);
		return NULL;
	}
	//create pthreads and transfer pool's info to each of them
	for(i = 0;i<cnt;i++){
		pthread_create(&pt->tid_p[i], NULL,start_routine, (void *)pt);
	}
	return pt;
}
int pour_task_method(pool_t* pt, void*(*func)(void*),void* arg ){
	pool_task_t* t = NULL;
	if(NULL == (t = malloc(sizeof(pool_task_t)))){
		perror("malloc new task node failed");
		return -1;
	}
	pthread_mutex_lock(&pt->lock);
	t->next = pt->task_p;
	pt->task_p = t;
	pthread_mutex_unlock(&pt->lock);
	t->routine_handler = func;
	t->arg = arg;
	
	return 0;
}
int wake_pthread(pool_t* pt){
	pthread_cond_broadcast(&pt->cond);
	return 0;
}
