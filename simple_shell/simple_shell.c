#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

//Define colors to be used in the simple shell
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int arg_count(char *buf)
{
	int count = 0;
	for (int i = 0; buf[i] != '\0'; i++){
		if (buf[i] == ' ' && buf[i + 1] != ' ')
		       count++;
	}
	// this means that if for example buf == "ls" the count will be 1 becouse there is no spaces
	// after ls and there is no arguments
	if (count == 0)
		return count++;
	else
		return count;
}


char** arg_values(int count, char buf[])
{
	count++;
	char ** arg_list = malloc((count) * sizeof(char*));
	for (int i =0 ; i < count; ++i)
        	arg_list[i] = malloc(20 * sizeof(char));

	//char *arg_list[count + 1];
	int init_size = strlen(buf);
	char delim[] = " ";
	int i = 0;

	char *ptr = strtok(buf, delim);

	while(ptr != NULL)
	{
		arg_list[i] = ptr;
		ptr = strtok(NULL, delim);
		i++;
	}	
	arg_list[i] = NULL;
	return arg_list;
}


int main()
{
	int count;
	while(1)
	{	
		char buf[100];
		printf(GRN "[mohieldin@boss" MAG " ~" RESET"]$ ");
		fgets(buf, 100, stdin);
		int len = strlen(buf);
		buf[len - 1] = 0;
		
		// support Enter
		if (strlen(buf) == 0)
			continue;

		int ret_pid = fork();
	
		if (ret_pid < 0)
			printf("Fork failed\n");
		else if (ret_pid > 0)
		{
			int status;
			wait(&status);
			printf(YEL "The last command status = %d\n" RESET, status);
		}
		else if (ret_pid == 0)
		{
			
			count = arg_count(buf);
			char **arg_list = arg_values(count, buf);
			execvp(arg_list[0], arg_list);
			printf(CYN "Exit failed :)\n" RESET);
			return -1;
		}
	}
	return 0;
}
