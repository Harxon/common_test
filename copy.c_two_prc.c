#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, const char *argv[])
{
	int fd_src = -1;
	int fd_dts = -1;
	off_t offn = -1;
	
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
	//whence the src file descripter
	if(0 > lseek(fd_src, 0, SEEK_SET)){
		perror("src lseek failed");
		goto main_err2;
	}
	pid_t pid = -1;
	if(0 > (pid = fork())){
		perror("fork error");
		goto main_err2;
	}else if(0 == pid){
		//Child process copy half past of the src file to dts file 
		int fd_src_cld = -1;
		int fd_dts_cld = -1;
		//to make a different file descripter in kernel ,prevent to mix with
		//another process
		close(fd_dts);
		close(fd_src);
		if(0 > (fd_src_cld = open(argv[1], O_RDWR))){
			perror("fd_src_cld open failed");
			goto pid_child_err0;
		}
		if(0 > (fd_dts_cld = open(argv[2], O_RDWR))){
			perror("fd_dts open failed");
			goto pid_child_err1;
		}
		//copy
		off_t num =offn - offn/2;
		int ret = -1;
		int cnt = 0;
		char ch = '\0';
		 
		if(0 > (ret = lseek(fd_src_cld, num, SEEK_SET))){
			perror("lseek failed");
			goto pid_child_err2;
		}
		if(0 > (ret = lseek(fd_dts_cld, num, SEEK_SET))){
			perror("lseek failed ");
			goto pid_child_err2;
		}
		while(cnt <= num){
			cnt++;
			if(0 > (ret = read(fd_src_cld, &ch, 1))){
				perror("read failed");
				goto pid_child_err2;
			}
			
			if(0 > (ret = write(fd_dts_cld, &ch, 1))){
				perror("write failed");
				goto pid_child_err2;
			}
		}

		close(fd_src_cld);
		close(fd_dts_cld);
		exit(0);

pid_child_err2:
		close(fd_dts_cld);
pid_child_err1:
		close(fd_src_cld);
pid_child_err0:
		exit(-1);
		//end of child process
	}else{
		//Parent process 
		//copy
		int ret = -1;
		int cnt = 0;
		char ch = '\0';
		 
		while(cnt <= offn){
			cnt++;
			if(0 > (ret = read(fd_src, &ch, 1))){
				perror("read failed");
				goto pid_parent_err0;
			}
			
			if(0 > (ret = write(fd_dts, &ch, 1))){
				perror("write failed");
				goto pid_parent_err0;
			}
		}

		close(fd_src);
		close(fd_dts);
		wait(NULL);
		exit(0);
pid_parent_err0:
		close(fd_src);
		close(fd_dts);
		wait(NULL);
		exit(-1);
	}//end of parent process

	return 0;
main_err2:
	close(fd_dts);
main_err1:
	close(fd_src);
main_err0:
	return -1;
}
