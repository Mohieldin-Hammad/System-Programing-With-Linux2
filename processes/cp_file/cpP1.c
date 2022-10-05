#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//ssize_t read(int fildes, void *buf, size_t nbyte);
//ssize_t write(int fildes, const void *buf, size_t nbyte);/

int main(int argc, char * argv[]){
	char buf[100];
	int src_fd = open(argv[1], O_RDONLY);
	int dest_fd = open(argv[2], O_CREAT | O_WRONLY, 0644);

	int read_count;
	while((read_count = read(src_fd, buf, 100)) > 0){
		write(dest_fd, buf, read_count);
	}
	close(src_fd);
	close(dest_fd);
	return 0;
}
