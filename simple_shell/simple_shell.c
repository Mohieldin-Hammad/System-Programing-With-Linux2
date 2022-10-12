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


typedef struct varAndVal {
        char* var;
	char* val;
        struct varAndVal* next;
} variables;


int arg_count(char *buf)
{
	////////////////
	printf("I am inside arg_count fun\n");
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
	//arg_list[i] = NULL;
	//for (int j = 0; j< 3; j++)
	//	printf("%s\n", arg_list[j]);

	return arg_list;
}


variables* create_empty_variable_list()
{
	variables* newVar = malloc(sizeof(variables));
	newVar = NULL;
	return newVar;
}

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


variables* add_variable(variables* oldVar, char* buf)
{
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
	return newVar;

}


void print_local_var(variables* vars)
{
	////////////////
	printf("I am inside print_local_var fun\n");
	int i = 0;
	//variables* iter;
        /////////////
	printf("I am inside print_local_var before the loop\n");
	for ( ; vars != NULL; vars = vars->next) {
		printf("local_vars[%d]: %s = %s\n", i,vars->var , vars->val);
	}
}


int isVar(char* buf)
{	
	////////////////
	printf("I am inside isVar fun\n");
	int var = 1;
	int count = 0;
	for (int i = 0; i < strlen(buf); i++){
		//////////////
		//printf("buf[%d] = %c\n", i, buf[i]);
		
		if (buf[i] == '=')
			count++;

		
		if (buf[i] == ' ' ){
			var = 0;
			break;
		}
	}
	////////////////////////////////////
	printf("I am inside isVar fun %d, %d \n", var, count);
	if (var == 1 && count == 1){
		printf("yes it is\n");
		return 1;
	}
	return 0;
}

int varIsLocal(variables* vars, char* buf)
{
	////////////////
	printf("I am inside varIsLocal fun\n");
	char *var = arg_values(3, buf, "=")[0];
	//variables* iter;
        //for (iter = myVars; iter != NULL; iter = iter->next) {
	//	///////////////
	//	printf("iter->var = %s, iter->val = %s\n", iter->var, iter->val);
	//	if (strcmp(iter->var , var) == 0){
	//		/////////////
	//		printf("iter->var = %s, var = %s\n", iter->var, var);
	//		printf("yes var is local\n");
	//		return 1;
	//	}
	//}
	
	while(vars != NULL){
		printf("vars->var = %s, vars->val = %s\n", vars->var, vars->val);
		if (strcmp(vars->var , var) == 0){
			printf("vars->var = %s, var = %s\n", vars->var, var);
			printf("yes var is local\n");
			return 1;
		}
	}

	///////////
	printf("no var is not local\n");
	return 0;
}

variables* change_var(variables* vars, char* buf)
{
	////////////////
	printf("I am inside change_var fun\n");
	char** var = arg_values(2, buf, "=");
        while( vars != NULL ) {
		if (vars->var == var[0]){
			vars->val = var[1];
			break;
		}
		vars = vars->next;
	}
	return  vars;

}

char* value_of(variables* Variables, char* var){
	////////////////
	printf("I am inside value_of fun\n");
	variables* iter;
        for (iter = Variables; iter != NULL; iter = iter->next) {
		if (iter->var == var)
			return iter->val;	
	}
	return NULL;

}

void add_env(variables* vars, char* buf)
{
	////////////////
	printf("I am inside add_env fun\n");
	int len;
	int count = arg_count(buf);
	char** arg_list= arg_values(count, buf, " ");
	for (int i = 1; i < count; i++)
	{
		char* val = value_of(vars, arg_list[i]);
		if (val != NULL){
			setenv(arg_list[i], val, 1);
		}
	}
}

int check_export(variables* vars, char* buf)
{
	////////////////
	printf("I am inside check_export fun\n");
	int can_exp = 1;
	int count = arg_count(buf);
	char** arg_list= arg_values(count, buf, " ");
	char* newexp = malloc(strlen(arg_list[0])+1);
	strcpy(newexp, arg_list[0]);
	if (newexp == "export"){
		for (int i = 1; i < count; i++){
			char* var = malloc(strlen(arg_list[i])+1);
			strcpy(var, arg_list[i]);
			if(varIsLocal(vars, var) != 1){
				printf("There is variable that is not local");
				can_exp = 0;
				break;
			}
		}
	}
	else{
		can_exp = 0;
	}
	return can_exp;

}




int main()
{
	variables* myVars = create_empty_variable_list();
	////////////////////
	//printf("before while\n");
	while(1)
	{	
		/////////////////
		//printf("At the first of while\n");
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
		printf("%d\n", 1);

		if (strcmp(buf, "set") == 0)
		{
			///////////////
			printf("I am inside set_comp\n");
			print_local_var(myVars);
			continue;
		}

		/////////////////
		printf("%d\n", 2);

		
		if (check_export(myVars, buf) == 1)
		{
			///////////////////
			printf("I am inside add_eniv comp\n");
			add_env(myVars, buf);
			continue;
		}
		/////////////////
		printf("%d\n", 3);


		if (isVar(buf) == 1 && varIsLocal(myVars, buf) != 1)
		{	
			////////////////////
			printf("I am inside adding var comp\n");
			myVars = add_variable(myVars, buf);
			print_local_var(myVars);
			continue;
		}
		else if (isVar(buf) == 1 && varIsLocal(myVars, buf) == 1)
		{
			///////////////////
			printf("I am inside change_var comp\n");
			change_var(myVars, buf);
			continue;
		}
		//////////////////////
		printf("comp is done\n");

		int ret_pid = fork();
		
		/////////////////////
		printf("after fork\n");
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
			//////////////////
			printf("ret_pid == 0\n");
			count = arg_count(buf);
			char **arg_list = arg_values(count+1, buf, " ");
			execvp(arg_list[0], arg_list);
			printf(CYN "Exit failed :)\n" RESET);
			return -1;
		}
	}
	return 0;
}
