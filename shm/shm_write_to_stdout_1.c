#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

typedef struct {
	pid_t pid;
	char buf[32];
}msg_t;

struct SHM_MSG{
	int shmid;
	msg_t* p;
}shm_msg;

void get_other_process_pid(int signum){

//	printf("signum: %d\n", signum);
}
void common_handler(int signum){
	printf("%d is handling IPC_RMID\n...",signum);
	shmdt(shm_msg.p);
	shmctl(shm_msg.shmid, IPC_RMID, NULL);
	puts("done!");
}

int main(int argc, const char *argv[])
{
	key_t key = -1;
	int shmid = -1;
	pid_t pid_o = -1;
	char buf[32] = {};
	bool crt = false;
	
	if(-1 == (key = ftok(argv[1], 'a'))){
		perror("ftok failed");
		exit(-1);
	}else{}

	if(-1 ==(shmid = shmget(key, sizeof(msg_t), IPC_CREAT|IPC_EXCL|0666))){
		if(EEXIST == errno){
			if(-1 == (shmid = shmget(key, sizeof(msg_t), IPC_CREAT))){
				perror("shmget reopen failed");
				exit(-1);
			}else{}
		}else{
			perror("failed to create shm");
			exit(-1);
		}
	}else{
		crt = true;
		shm_msg.shmid = shmid;
	}
	printf("shmid: %d\n", shmid);

	msg_t* p = (msg_t*)shmat(shmid, NULL, 0);
	if((void*)-1 == (void*)p){
		perror("failed to shmat");
		exit(-1);
	}else{
		shm_msg.p = p;
	}
	
	signal(SIGUSR1, get_other_process_pid);
	if(crt){
		signal(SIGQUIT, common_handler);
		signal(SIGTSTP, common_handler);
		signal(SIGINT, common_handler);
	}else{}

	p->pid = getpid();
	pause();
	pid_o = p->pid;
	printf("%d is linked to me(%d).Below is its words.\n",pid_o,getpid());
	while(1){
		kill(pid_o, SIGUSR1);
		pause();
		memcpy(buf, p->buf, strlen(p->buf));
		write(1, buf,strlen(buf)); 
	}
	
	
	return 0;
}
