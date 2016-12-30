#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	pid_t pid0 = fork();
	if(pid0 > 0){
		wait(NULL);
		printf("childpid: %d, pid: %d, ppid: %d,\n",pid0, getpid(), getppid());
	}else if(0 == pid0){

		pid_t pid1 = fork();
		if(pid1 > 0){
			wait(NULL);
			printf("childpid: %d, pid: %d, ppid: %d,\n",pid1, getpid(), getppid());
		}else if(0 == pid1){

			pid_t pid2 = fork();
			if(pid2 > 0){
				wait(NULL);
				printf("childpid: %d, pid: %d, ppid: %d,\n",pid2, getpid(), getppid());
			}else if(0 == pid2){
			
			}
		}
	}
	printf("...\n");
	return 0;
}
