#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

#include <readline/readline.h>
#include <readline/history.h>
//Define colors to be used in the simple shell
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

//-----------------------------------------------------------------------------------------
int arg_count(char *buf)
{
	////////////////
	//printf("I am inside arg_count fun\n"); 

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
	//printf("I am inside arg_values fun\n");
	count++;
	char ** arg_list = malloc((count) * sizeof(char*));
	
	for (int i =0 ; i < count; ++i)
        	arg_list[i] = malloc(100 * sizeof(char));
	
	//char *arg_list[count + 1];
	int init_size = strlen(buf);

	char *copy_of_buffer;
	copy_of_buffer = calloc(init_size+1, sizeof(char)); 

    	strcpy(copy_of_buffer, buf);

	char* delim = delim_char;
	int i = 0;
	char *ptr = strtok(copy_of_buffer, delim);
	while(ptr != NULL)
	{
		////////////////
		//printf("ptr = %s\n", ptr);
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
	//printf("I am inside isVar fun\n");
	
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
	//printf("I am inside isVar fun %d, %d \n", var, count);
	if (var && count == 1){
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------------------
bool varIsLocal(char* variable)
{
	FILE* file_path = fopen("data/DB.csv", "r");

	if (!file_path){
		printf(RED "Can't open file\n" RESET);
		return false;
	}
	
	char buffer[255];

	while (fgets(buffer, 255, file_path)){
		char* var = strtok(buffer, ", ");
		if (strcmp(var, variable) == 0){
			fclose(file_path);
			return true;
               	}
	}
	fclose(file_path);
	return false;
}
//----------------------------------------------------------------------------------------------
int add_variable(char* variable, char* value)
{
	///////////////////
	//printf("I am inside add_variable fun\n");
	
	FILE* file_path = fopen("data/DB.csv", "ab+");
	if (!file_path){
		printf(RED "Can't open file\n" RESET);
		return 1;
	}
	fprintf(file_path, "%s, %s\n", variable, value);
	fclose(file_path);
	return 0;
}
//------------------------------------------------------------------------------------------------
int delete_variable(char* variable)
{
	//////////////
	printf("I am inside delete_variable fun\n");

	FILE *file, *temp;
	char filename[] = "data/DB.csv";
	char temp_filename[] = "data/temp___DB.csv";
	char buffer[255];
	file = fopen(filename, "r");
	temp = fopen(temp_filename, "w");

	if (file == NULL || temp == NULL){
		printf("Error opening file(s) for changing variable\n");
		return 1;
	}
	while (fgets(buffer, 255, file))
	{
		char *copy_of_buffer;
		copy_of_buffer = calloc(strlen(buffer)+1, sizeof(char)); 
    		strcpy(copy_of_buffer, buffer);

		char *var = strtok(buffer, ", ");
		if (strcmp(var, variable) != 0){
			fputs(copy_of_buffer, temp);
		}
		free(copy_of_buffer);
	}
	fclose(file);
	fclose(temp);

	remove(filename);
	rename(temp_filename, filename);
	return 0;
}
//----------------------------------------------------------------------------------------------
int change_variable(char* variable, char* value){
	///////////////
	//printf("I am inside the change_variable fun\n");

	int deleted = delete_variable(variable);
	int added = add_variable(variable, value);
	if (deleted == 0 && added == 0){
		//the variable is updated
		return 0;
	}
	else //the variable is not updated and there is a problem either in delete or add functions
	{
		return 1;
	}
}
//------------------------------------------------------------------------------------------------
bool isEmpty(){
	FILE* file_path = fopen("data/DB.csv", "ab+");
	if (!file_path){
		printf(RED "Can't open file\n" RESET);
		return true;
	}
	fseek (file_path, 0, SEEK_END);
    	int size = ftell(file_path);

    	if (size == 0) {
       		printf("file is empty\n");
		return true;
    	}
	return false;
}
//------------------------------------------------------------------------------------------------
int count_of_variables(){

}
//------------------------------------------------------------------------------------------------

int delete_all_variables() {
        //////////////
	//printf("I am inside delete_all_variables fun\n");
	
	char filename[] = "data/DB.csv";
	if (access(filename, F_OK) == 0)
		remove(filename);

	FILE *file;
	file = fopen(filename, "w+");
	if (file == NULL){
		printf("Error opening file\n");
		return 1;
	}
	fclose(file);
	//created new empty DB.csv file	
	return 0;
}
//------------------------------------------------------------------------------------------------
int print_local_var(){
	/////////////////////
	//printf("I am inside print_local_var fun\n");

	FILE* file = fopen("data/DB.csv", "r");
	if (!file){
		printf("Can't open file\n");
		return 1;
	}
	
	char buffer[255];
	int row = 0;
	int column = 0;

	while (fgets(buffer, 255, file)){
		column = 0;
		char* value = strtok(buffer, ", ");
		while(value){
			if (column == 1)
				printf(" = ");
			printf("%s", value);
			value = strtok(NULL, ", ");
			column++;
		}
		//printf("\n");
	}
	fclose(file);
	return 0;
}
//------------------------------------------------------------------------------------------------
char* value_of(char* variable)
{
	/////////////////////
	//printf("I am inside value_of fun\n");

	FILE *file;
	char filename[] = "data/DB.csv";
	char buffer[2048];
	file = fopen(filename, "r");
	if (file == NULL){
		printf("Error opening file\n");
		return NULL;
	}
	while (fgets(buffer, 2048, file))
	{
		char *value = strtok(buffer, ", ");
		if (strcmp(value, variable) == 0){
			value = strtok(NULL, ", ");
			return value;
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------------------------------
bool check_export(char* buf)
{
	////////////////
	//printf("I am inside check_export fun\n");
	
	bool can_exp = true;
	int count = arg_count(buf);
	if (count == 1)
		return false;
	// arg_values is spliting the buffer string to list of strings
	char** arg_list= arg_values(count+1, buf, " ");
	
	char* copy_of_first_arg = malloc(strlen(arg_list[0])+1* sizeof(char));	
	strcpy(copy_of_first_arg, arg_list[0]);

	if (strcmp(copy_of_first_arg, "export") == 0){
		for (int i = 1; i < count; i++){
			char *var;
			var = calloc(strlen(arg_list[i])+1, sizeof(char)); 
			strcpy(var, arg_list[i]);
			if(!varIsLocal(var)){
				can_exp = false;
				free(var);
				break;
			}
			free(var);
		}
	}
	else{
		can_exp = false;
	}
	free(arg_list);
	free(copy_of_first_arg);

	return can_exp;
}
//-------------------------------------------------------------------------------------------------
int export_variables(char* buf)
{
	////////////////
	//printf("I am inside add_env fun and buf = %s\n", buf);
	int len, status;
	
	int count =arg_count(buf);
	char** arg_list= arg_values(count+1, buf, " ");
	
	for (int i = 1; i < count; i++) 
	{
		char* val = value_of(arg_list[i]);
		if (val == NULL){
			return -1;
		}
		len = strlen(val);
		val[len - 1] = 0;

		char *copy_of_val;
		copy_of_val = calloc(strlen(val)+1, sizeof(char)); 
    		strcpy(copy_of_val, val);

		// first way to add environment variable
		//char* full_variable = strcat(strcat(arg_list[i], "="), copy_of_val);
		//////////////////
		//printf("adding %s to env\n", full_variable);
		//int status = putenv(full_variable);	
		
		/////////////////
		//printf("var= %s, val = %s\n",arg_list[i], copy_of_val); 
		status =  setenv(arg_list[i], copy_of_val, 1);				
		free(copy_of_val);
		if (status == -1)
			break;	
	}
	free(arg_list);
	return status;
}
//-------------------------------------------------------------------------------------------------
bool check_to_unset(char* buf)
{
	////////////////
	//printf("I am inside check_env fun\n");
	
	bool is_env = true;
	int count = arg_count(buf);
	if (count == 1)
		return false;
	// arg_values is spliting the buffer string to list of strings
	char** arg_list= arg_values(count+1, buf, " ");
	
	char* copy_of_first_arg = malloc(strlen(arg_list[0])+1* sizeof(char));	
	strcpy(copy_of_first_arg, arg_list[0]);

	if (strcmp(copy_of_first_arg, "unset") == 0){
		for (int i = 1; i < count; i++){
			char *var;
			var = calloc(strlen(arg_list[i])+1, sizeof(char)); 
			strcpy(var, arg_list[i]);
			if(getenv(var) == NULL){
				is_env = false;
				printf("*There is unenvironment variable");
				free(var);
				break;
			}
			free(var);
		}
	}
	else{
		is_env = false;
	}
	free(arg_list);
	free(copy_of_first_arg);

	return is_env;
}
//-------------------------------------------------------------------------------------------------
int unset(char* buf)
{
	int status;
	int count =arg_count(buf);
	char** arg_list= arg_values(count+1, buf, " ");
	for (int i = 1; i < count; i++) 
	{
		status =  unsetenv(arg_list[i]);				
		if (status == -1)
			break;	
	}
	free(arg_list);
	return status;
}
//-------------------------------------------------------------------------------------------------
int str_length(char str[]) {
       int count; 
       for (count = 0; str[count] != '\0'; ++count);
       return count; 
}
//-------------------------------------------------------------------------------------------------
bool buf_is_spaces(char buf[]){
	int space_count;
	for (int i = 0; buf[i] != '\0'; i++){
		if (buf[i] == ' '){
			space_count++;
			///////////////////
			//printf("%d\n", space_count);
		}
	}
	if (space_count == str_length(buf))
		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------------------

int main()
{
	// call delete_all_variables to delete the variable in database if exists
	delete_all_variables();
	while(1){	
		
		int count;
		char buf[2048];
		printf(GRN "[mohieldin@boss" MAG " ~" RESET"]$ ");
		fgets(buf, 2048, stdin);

		int len = str_length(buf);
		////////////////
		//printf("len= %d\n", len);
		
		buf[len - 1] = 0;
		
		//////////////////////
		//printf("accepted the command\n");	
		
		// if the user typed enter or only spaces it will start new line
		if (str_length(buf) == 0 || buf_is_spaces(buf))
			continue;

		if (strcmp(buf, "set") == 0)
		{
			///////////////
			//printf("I am inside set_comp\n");
			print_local_var();
			continue;
		}


		if (check_export(buf))
		{
			///////////////////
			//printf("I am inside add_eniv comp\n");
			export_variables(buf);
			continue;
		}

		if (check_to_unset(buf)){
			unset(buf);
			continue;	
		}

		if (isVar(buf)){
			char** variable_and_value = arg_values(3, buf, "=");
			if (!varIsLocal(variable_and_value[0]))
			{		
				add_variable(variable_and_value[0], variable_and_value[1]);
			}
			else if (varIsLocal(variable_and_value[0]))
			{
				change_variable(variable_and_value[0], variable_and_value[1]);
			}
			///////////////
			//print_local_var();
			free(variable_and_value);
			continue;
		}
		
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
			char **arg_list = arg_values(count+1, buf, " ");
			execvp(arg_list[0], arg_list);
			printf(CYN "command failed :)\n" RESET);
			return -1;
		}
	}
	return 0;
}
