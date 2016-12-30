#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>

//拷贝数据时使用的最大线程数量
#define PRC_NUM 16 

//拷贝数据时采用分段拷贝，该结构体包含分段拷贝时所需
//的所有信息
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
//创建分段数据链表头；
linklistp_t create_linklist_head(void){
	linklistp_t hp = NULL;
	if(NULL == (hp = malloc(sizeof(linknode_t)))){
		perror("malloc linklist head failed");
		return NULL;
	}
	hp->next = NULL;
	bzero(hp, sizeof(linknode_t));

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
}
//审核拷贝文件个数,判断文件长度
int verify_src_file(linklistp_t hp, int argc, const char* argv[]){
	int i = 0;
	int fd_src = -1;
	int fd_dts = -1;
	//判断有效输入
	if(argc < 3){
		printf("Wrong format!\n eg: ./a.out + src_filename + dts_filename\n");
		goto verify_err0;
	}

	//open a src cp file
	if(0 > (fd_src = open(argv[1],O_RDWR))){
		perror("fd_src open failed");
		goto main_err0;
	}
	for(i = 0;i < (argc -2);i++){
		//创建拷贝目标文件
		if(0 > (fd_dts = creat(argv[2],S_IRWXU))){
			if(EEXIST == errno){
				puts("src and dts file has a same name");
				goto verify_err1;
			}
			perror("can't create new dts_filename\n");
			goto verify_err1;
		}	
	}

verify_err1:

verify_err0:
	return -1;
}
int main(int argc, const char *argv[])
{
	linklistp_t hp = NULL;

	//创建头节点
	if(NULL == (hp = create_linklist_head())){
		perror("create_linklist_head failed");
		goto main_err0;
	}
	
	


	free(hp);
	return 0;
main_err0:
	return -1;
}
