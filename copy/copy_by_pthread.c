#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>

#define PRC_NUM 16 

typedef struct copy_info{
	char* src;
	char* dts;
	off_t len;
	off_t start;
	off_t end;
	
	int thread_num;
	void* data;
	struct copy_info* next;
}linknode_t,*linklistp_t;

int add_new_cpinfo_node(linklistp_t hp, off_t start, off_t end, int thread_num);
linklistp_t create_linklist_head(const char* src, const char* dts, off_t len, int thread_num);
int display_node_info(linklistp_t p);


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

int display_node_info(linklistp_t p){
	
	printf("p->src:%s, p->dts:%s\n",p->src, p->dts);
	printf("p->len:%ld, p->start:%ld, p->end:%ld\n",p->len, p->start, p->end);
	printf("p->thread_num:%d, p->data:%p, p->next:%p\n",p->thread_num, p->data, p->next);

	return 0;
}
void* pthread_handler(void* args){
	linklistp_t p = args;
	printf("###\n");
	printf("p:%p, args:%p\n", p, args);
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

	pthread_t thread[PRC_NUM] = {0};

	
	//judgement the arguments
	if(argc < 3){
		printf("Wrong format!\n eg: ./a.out + src_filename + dts_filename\n");
		exit(0);
	}
	//open a src cp file
	if(0 > (fd_src = open(argv[1],O_RDWR))){
		perror("fd_src open failed");
		goto main_err0;
	}
	//create new cp file
	if(0 > (fd_dts = creat(argv[2],S_IRWXU))){
		if(EEXIST == errno){
			puts("same name");
			goto main_err1;
		}
		perror("can't create new dts_filename\n");
		goto main_err1;
	}
	//measure the length of src file
	if(0 > (offn = lseek(fd_src, 0, SEEK_END) - 1)){
		perror("src lseek failed");
		goto main_err2;
	}
	printf("offn:%ld\n",offn);
	//whence the src file descripter
	if(0 > lseek(fd_src, 0, SEEK_SET)){
		perror("src lseek failed");
		goto main_err2;
	}
	//decide the number of copying file pthread by file length 
	//offn  vs  2^10 , <2^10 cp use 1p;<2^20 cp use 2p;<2^30 cp use 3p;...
	thread_num = (int)((log(offn)/log(2)))/10 + 1;
	printf("thread_num:%d\n",thread_num);
	//for safety, but it's impossible for below,~.~
	if(thread_num > PRC_NUM)thread_num = PRC_NUM;
	//cut num pieces of offn, and store them at list
	//@ create list by thread_num  
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
	//pthread 
	//find linklist head
	linklistp_t p = NULL;
	p = hp->next;
	//create designed pthread_handler by (2^n / 1024)
	for(i = 0;i < thread_num;i++){
		if(0 > (ret = pthread_create(&thread[i], NULL, pthread_handler, (void*)p))){
			perror("pthread_create failed");
			goto main_err4;
		}
		//find next cpinfo from linklist node,transfer from it to pthread_handler 
		if(i < (thread_num - 1)){
			p = p->next;
		}
	}

	for(i=0; i<thread_num; i++){
		pthread_join(thread[i], NULL);
	}
	return 0;

main_err4:
	//rm dts 
	//pthread_join 
main_err3:
main_err2: 
	close(fd_dts);
main_err1:
	close(fd_src);
main_err0:
	return -1;
}
