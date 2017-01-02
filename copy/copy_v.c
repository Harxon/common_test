/*	
 *	拷贝文件的教学案例
 *	涉及知识：
 *	@函数封装
 *	@链式队列
 *	@线程同步之屏障
 *	注意：
 *		该cp功能并不完善，
 *		仅仅实现一个文件的拷贝，而且需要指定名称,作者眼花手拙，如有错误请您斧正！
 * */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
//支持的最大线程开启数量
#define PRC_NUM 16 

//拷贝文件时，需要的文件信息
typedef struct copy_info{
	char* src;/*源文件路径*/
	char* dts;/*目标文件路径*/
	off_t len;/*源文件文件长度（lseek测得长度 - 1）*/
	off_t start;/*拷贝的起始位置*/
	off_t end;/*拷贝的终止位置*/
	
	int thread_num;/*当前拷贝文件使用的线程数量*/
	void* data;/*自定义数据*/
	struct copy_info* next;
}linknode_t,*linklistp_t;

linklistp_t create_linklist_head(const char* src, const char* dts, off_t len, int thread_num);
int add_new_cpinfo_node(linklistp_t hp, off_t start, off_t end, int thread_num);
int display_node_info(linklistp_t p);

/*	向队列中添加拷贝文件的信息结构体
 *	参数：
 *	hp: 队列的头节点指针
 *	start:拷贝数据在源文件和目标文件中的开始位置
 *	end:拷贝文件在源文件和目标文件中的结束位置
 *	thread_num：当前拷贝文件使用的线程数量。
 * */
int add_new_cpinfo_node(linklistp_t hp, off_t start, off_t end, int thread_num){
	linklistp_t p = NULL;
	if(NULL == (p = malloc(sizeof(linknode_t)))){
		perror("malloc failed");
		return -1;
	}
	p->next = hp->next;
	hp->next = p;
	
	p->src = hp->src;
	p->dts = hp->dts;
	p->len = hp->len;

	p->start = start;
	p->end = end;
	p->thread_num = thread_num;

	return 0;
}
 /*
  *	创建队列头，填充源文件/目标文件的信息以及处理的线程数量
  *
  */
linklistp_t create_linklist_head(const char* src, const char* dts, off_t len, int thread_num){
	linklistp_t hp = NULL;
	if(NULL == (hp = malloc(sizeof(linknode_t)))){
		perror("malloc failed");
		return NULL;
	}
	hp->next = NULL;
	
	hp->src = (char*)src;
	hp->dts = (char*)dts;
	hp->len = len;
	hp->thread_num = thread_num;

	return hp;
}
//释放整个链表
void free_linklist(linklistp_t hp){
	linklistp_t p = hp->next;
	while(p){
		hp->next = p->next;
		free(p);
		p = hp->next;
	}
	free(hp);
	hp = NULL;
}
int display_node_info(linklistp_t p){
	
	printf("###\n");
	printf("p->thread_num:%d, p->data:%p\n",p->thread_num, p->data);
	printf("p:%p, p->next:%p\n",p, p->next);
	printf("p->src:%s, p->dts:%s, p->start:%ld, p->end:%ld, p->len:%ld\n",p->src, p->dts, p->start, p->end, p->len);

	return 0;
}
/*
 *	线程处理函数，每个线程处理一个节点的数据
 * */
void* pthread_handler(void* args){
	linklistp_t p = args;
	display_node_info(p);
	int ret = 0;
	int fd_src = -1, fd_dts = -1;
	char ch = '\0';

	if(0 > (fd_src = open(p->src, O_RDWR))){
		perror("open failed");
		goto pthread_err0;
	}
	if(0 > (fd_dts = open(p->dts, O_RDWR))){
		perror("open failed ");
		goto pthread_err1;
	}
	if(0 > (ret = lseek(fd_src, p->start, SEEK_SET))){
		perror("lseek failed");
		goto pthread_err2;
	}
	if(0 > (ret = lseek(fd_dts, p->start, SEEK_SET))){
		perror("lseek failed");
		goto pthread_err2;
	}
	while(p->end >= lseek(fd_src, 0, SEEK_CUR)){
		if(0 > read(fd_src, &ch, 1)){
			perror("read failed");
			goto pthread_err3;
		}
		if(0 > write(fd_dts, &ch, 1)){
			perror("write failed");
			goto pthread_err3;
		}
	}
	close(fd_dts);
	close(fd_src);
	return args;
pthread_err3:
pthread_err2:
	close(fd_dts);
pthread_err1:
	close(fd_src);
pthread_err0:
	return NULL;
}

int main(int argc, const char *argv[])
{
	int fd_src = -1;
	int fd_dts = -1;
	off_t offn = -1;
	off_t len_temp = -1, start_temp = -1, end_temp = -1;
	int ret = -1;
	int thread_num = 0, i = 0;
	char dts[32] = {0};

	pthread_t thread[PRC_NUM] = {0};
	
	if(argc < 3){
		printf("Wrong format!\n eg: ./a.out + src_filename + dts_filename\n");
		exit(0);
	}
	//打开源文件
	if(0 > (fd_src = open(argv[1],O_RDWR))){
		perror("fd_src open failed");
		goto main_err0;
	}
	//创建新文件
	if(0 > (fd_dts = creat(argv[2],S_IRWXU))){
		if(EEXIST == errno){
			puts("same name");
			goto main_err1;
		}
		perror("can't create new dts_filename\n");
		goto main_err1;
	}
	//得到新文件的长度，
	//注意要减1，因为lseek要偏移到文章的结尾EOF处，所以文章的实际长度要比lseek测得的小一个字符
	if(0 > (offn = lseek(fd_src, 0, SEEK_END) - 1)){
		perror("src lseek failed");
		goto main_err2;
	}
	printf("offn:%ld\n",offn);
	if(0 > lseek(fd_src, 0, SEEK_SET)){
		perror("src lseek failed");
		goto main_err2;
	}
	//根据文件的长度来推算需要几个处理的线程, 0 < text < 2^10用1个线程，2^10 < text < 2^20用2个线程，小于2^30用3个线程，
	thread_num = (int)((log(offn)/log(2)))/10 + 1;
	printf("thread_num:%d\n",thread_num);
	//最大线程处理数量的限定，此处没有实际作用
	if(thread_num > PRC_NUM)thread_num = PRC_NUM;
	//创建队列头，并填充必要信息
	linklistp_t hp = NULL;
	if(NULL == (hp = create_linklist_head(argv[1], argv[2], offn, thread_num))){
		perror("create_linklist_head failed");
		goto main_err3;
	}
	printf("debug:%s, %d\n",__func__,__LINE__);

	start_temp = 0;
	end_temp = 0;
	len_temp = offn/thread_num;
	for(i=0;i<thread_num;i++){

		end_temp = start_temp + len_temp;
		if(thread_num - 1 == i){
			end_temp = offn;
		}	
		if(0 > add_new_cpinfo_node(hp, start_temp, end_temp, thread_num)){
			perror("add_new_cpinfo_node failed");
			goto main_err3;
		}
		start_temp = end_temp;

	}
	//根据前面推算数来的线程数量来创建线程，并且每次都把新节点的信息传递进去
	linklistp_t p = NULL;
	p = hp->next;
	for(i = 0;i < thread_num;i++){
		if(0 > (ret = pthread_create(&thread[i], NULL, pthread_handler, (void*)p))){
			perror("pthread_create failed");
			goto main_err4;
		}
		if(i < (thread_num - 1)){
			p = p->next;
		}
	}
	
	//此处应该等待子线程结束，如果主线程立即退出会导致整个进程推出，所以用pthread_join阻塞等待所有进程退出
	for(i=0; i<thread_num; i++){
		pthread_join(thread[i], NULL);
	}
	
	//手动释放malloc申请的空间
	free_linklist(hp);
	return 0;

main_err4:
	//rm dts file
	//pthread_join 
main_err3:
main_err2: 
	close(fd_dts);
main_err1:
	close(fd_src);
main_err0:
	return -1;
}
