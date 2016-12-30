#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int rm_multiple_slash(char* s){
	int i = 0, len = 0;
	len = strlen(s);

	printf("before s:%s\n",s);
	for(i=0;i<len;i++){
		if(('/' == s[i]) && ('/' == s[i+1])){
			memcpy(s+i,s+i+1,len-i);
			len--;
		}
	}
	printf("after s:%s\n",s);
	return 0;
}

int list_dir(char* pathname){
	DIR* dirp = NULL;
	struct dirent* dt = NULL;
	char child_pathname[256] = {0};

	dirp = opendir(pathname);
	if(dirp == NULL){
		printf("err:%s\n",strerror(errno));
		return -1;
	}

	while((dt = readdir(dirp))){
		if(DT_DIR == dt->d_type){
			if(!strcmp(dt->d_name,".") || (!strcmp(dt->d_name,".."))){
				continue;
			}else{
				sprintf(child_pathname,"%s/%s",pathname,dt->d_name);
				rm_multiple_slash(child_pathname);
				printf("child_pathname:%s\n",child_pathname);
				list_dir(child_pathname);
			}
		}else{
			printf("d_name:%s\n",dt->d_name);
		}
	}
	closedir(dirp);
	return 0;
}

int main(int argc, const char *argv[])
{	
	char pathname[256] = {0};
	if(argc > 1){
		strcpy(pathname,argv[1]);
	}else{
		strcpy(pathname,"./");
	}
	list_dir(pathname);

	return 0;
}
