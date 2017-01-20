#include "tpool.h"

void *do_task(void *arg) //执行任务
{
	tpool_t *tp=(tpool_t*)arg;
	task_t *t = NULL;
	while(1)
	{
		pthread_mutex_lock(&tp->mutex);
		while(tp->mytask==NULL) //任务队列为空
		{
			pthread_cond_wait(&tp->cond,&tp->mutex);
		}
		t = tp->mytask;  //执行任务 并删除任务节点
		tp->mytask = t->next;
		pthread_mutex_unlock(&tp->mutex);
		printf("%s,3p:%p\n",__func__, t->func);
		printf("%s, my_task_handler:%p\n",__func__, t->func);
		t->func(t->arg);
		printf("%s,4p:%p\n",__func__, t->func);
		free(t);
		printf("%s,free(%p)\n",__func__, t);
		t = NULL;
	}

	return (void*)0;
}


tpool_t *tpool_create(int num)
{
	tpool_t *tp=(tpool_t *)malloc(sizeof(tpool_t));
	if(NULL == tp)
	{	
		perror("fail to malloc");
		return NULL;
	}
	tp->mytask=NULL;//初始化线程池
	pthread_mutex_init(&tp->mutex,NULL);
	pthread_cond_init(&tp->cond,NULL);
	tp->tid=(pthread_t *)malloc(num*sizeof(pthread_t));

	while(num--)
	{
		pthread_create(&tp->tid[num],NULL,do_task,(void *)tp);
	}
	return tp;

}
void tpool_add_task(tpool_t *tp,void *(*func)(void *),void *arg)//加入任务队列
{
	
	task_t *t=(task_t *)malloc(sizeof(task_t));
	t->func=func;
	t->arg=arg;
	t->next=NULL;
	
	pthread_mutex_lock(&tp->mutex);
	t->next=tp->mytask;
	tp->mytask=t;
	pthread_mutex_unlock(&tp->mutex);
	pthread_cond_broadcast(&tp->cond);//条件唤醒
}
