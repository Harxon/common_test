#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

long get_file_size(const char* filename){
	struct stat buf;
	stat(filename, &buf);
	return buf.st_size;
}

int main(int argc, const char *argv[])
{
	if(argc < 2){
		perror("./a.out + filename");
		exit(-1);
	}
	long file_len = get_file_size(argv[1]);
	printf("file_len: %ld\n", file_len);
	int fd = open(argv[1], O_RDONLY);
	file_len = lseek(fd, 0, SEEK_END);
	printf("file_len: %ld\n", file_len);
	close(fd);
	return 0;
}
