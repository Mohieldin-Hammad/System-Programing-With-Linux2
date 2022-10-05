#include <stdio.h>
#include <unistd.h>

int main(){
	char buf[255];
	
	if (getcwd(buf, sizeof(buf)) == NULL){
		perror("pwd error :)");
		return -1;
	}
	else{
		printf("%s\n", buf);
	}
	return 0;
}

