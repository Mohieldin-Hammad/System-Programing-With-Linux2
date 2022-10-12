#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

//Define colors to be used in the simple shell
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

struct variables {
        char* var;
	char* val;
        struct variables* next;
};

//-----------------------------------------------------------------------------------------
int arg_count(char *buf)
{
	////////////////
	printf("I am inside arg_count fun\n");
	int count = 0;
	for (int i = 0; buf[i] != '\0'; i++){
		if (buf[i] == ' ' && buf[i + 1] != ' ' && buf[i+1] != '\0')
		       count++;
	}
	// this means that if for example buf == "ls" the count will be 1 becouse there is no spaces
	// after ls and there is no arguments
	return ++count;
}

//---------------------------------------------------------------------------------------------------
char** arg_values(int count, char buf[], char* delim_char)
{
	////////////////
	printf("I am inside arg_values fun\n");
	count++;
	char ** arg_list = malloc((count) * sizeof(char*));
	
	for (int i =0 ; i < count; ++i)
        	arg_list[i] = malloc(20 * sizeof(char));

	//char *arg_list[count + 1];
	int init_size = strlen(buf);
	char* delim = delim_char;
	int i = 0;

	char *ptr = strtok(buf, delim);

	while(ptr != NULL)
	{
		////////////////
		printf("ptr = %s\n", ptr);
		
		arg_list[i] = ptr;
		ptr = strtok(NULL, delim);
		i++;
	}	
	
	arg_list[i] = NULL;
		return arg_list;
}
//--------------------------------------------------------------------------------------------------
bool isVar(char* buf)
{
	////////////////
	printf("I am inside isVar fun\n");
	bool var = true;
	int count = 0;
	for (int i = 0; i < strlen(buf); i++){
		//////////////
		//printf("buf[%d] = %c\n", i, buf[i]);

		if (buf[i] == '=')
			count++;


		if (buf[i] == ' '){
			var = false;
			break;
		}
	}
	////////////////////////////////////
	printf("I am inside isVar fun %d, %d \n", var, count);
	if (var && count == 1){
		printf("yes it is variable\n");
		return true;
	}
	printf("no it is not variable\n");
	return false;
}

//----------------------------------------------------------------------------------------------
bool varIsLocal(variables* vars, char* buf)
{
	if (vars == NULL)
		return false;

	////////////////
	printf("I am inside varIsLocal fun\n");
	char *var = arg_values(3, buf, "=")[0];
        for ( ; vars != NULL; vars = vars->next) {
		///////////////
		printf("vars->var = %s, vars->val = %s\n", vars->var, vars->val);
		if (strcmp(vars->var , var) == 0){
			/////////////
			printf("vars->var = %s, var = %s\n", vars->var, var);
			printf("yes var is local\n");
			delete_vars(vars);
			return true;
		}
	}
	

	///////////
	printf("no var is not local\n");
	delete_vars(vars);
	return false;
}


//----------------------------------------------------------------------------------------------
variables* create_variable(char* variable, char* value)
{
	////////////////
	printf("I am inside create_variable fun\n");
	variables* newVar = malloc(sizeof(variables));
	if (newVar != NULL){
		newVar->var = variable;
		newVar->val = value;
		newVar->next = NULL;
	}
	return newVar;
}

//-------------------------------------------------------------------------------------------
variables* add_variable(variables* oldVar, char* buf)
{
	printf("----------------------------\n");

	////////////////
	printf("I am inside add_variable fun\n");
	char **arg_list = arg_values(3, buf, "=");

	///////////////
	printf("add_variable: arg_list[0] = %s, arg_list[1] = %s\n", arg_list[0], arg_list[1]);
	variables* newVar = create_variable(arg_list[0], arg_list[1]);
	if (newVar != NULL){
		if (oldVar != NULL)
			newVar->next = oldVar;
		else
			newVar->next = NULL;
	}
	///////////////////
	printf("add_variable: newVar->var = %s, newVar->val = %s\n", newVar->var, newVar->val);
	printf("----------------------------\n");

	return newVar;

}



//------------------------------------------------------------------------------------------------
void delete_vars(variables* oldVars) {
        if (NULL != oldVars->next) {
                delete_vars(oldVars->next);
        }
        free(oldVars);
}


//-------------------------------------------------------------------------------------------------
int main()
{
	variables* myVars = NULL;
	while(1)
	{	
		
		int count;
		char buf[100];
		printf(GRN "[mohieldin@boss" MAG " ~" RESET"]$ ");
		fgets(buf, 100, stdin);
		int len = strlen(buf);
		buf[len - 1] = 0;
	
		//////////////////////
		//printf("accepted the command\n");	
		// support Enter
		if (strlen(buf) == 0)
			continue;


		/////////////////
		printf("----------------------------\n");
		if (isVar(buf) && !varIsLocal(myVars, buf))
		{	
			////////////////////
			printf("I am inside adding var comp\n");
			
			myVars = add_variable(myVars, buf);
			///////////////
			//print_local_var(myVars);
			continue;
		}
		printf("----------------------------\n");


		int ret_pid = fork();
		
		/////////////////////
		printf("after fork\n");
		if (ret_pid < 0)
			printf("Fork failed\n");
		else if (ret_pid > 0)
				}
	}
	return 0;
}

