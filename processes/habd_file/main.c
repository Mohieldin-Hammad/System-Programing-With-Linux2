#include <stdio.h>
#include "header.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char * argv[]){
	cp_fun(argc, argv);
	remove(argv[1]);

}
