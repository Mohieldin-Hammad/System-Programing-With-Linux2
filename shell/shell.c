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

//struct variables *current = NULL;
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
	
	printf("this one for testing ------%s: %s\n", arg_list[0], arg_list[1]);
	
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
bool varIsLocal(char* variable){
	FILE* file_path = fopen("data/DB.csv", "r");

	if (!file_path){
		printf(RED "Can't open file\n" RESET);
		return false;
	}
	
	char buffer[100];

	while (fgets(buffer, 100, file_path)){
		char* var = strtok(buffer, ", ");
		if (strcmp(var, variable) == 0){
			fclose(file_path);
			//////////////
			printf("var is local\n");
			return true;
               	}
	}
	fclose(file_path);
	return false;
}

//----------------------------------------------------------------------------------------------
int add_variable(char* variable, char* value){
	///////////////////
	printf("I am inside add_variable fun\n");
	
	FILE* file_path = fopen("data/DB.csv", "ab+");
	if (!file_path){
		printf(RED "Can't open file\n" RESET);
		return 1;
	}

	fprintf(file_path, "%s, %s\n", variable, value);
	printf("New variable is added to data base\n");
	fclose(file_path);
	return 0;
}

//------------------------------------------------------------------------------------------------
int delete_variable(char* variable){
	//////////////
	printf("I am inside delete_variable fun\n");

	FILE *file, *temp;
	char filename[] = "data/DB.csv";
	char temp_filename[] = "data/temp___DB.csv";
	char buffer[100];
	file = fopen(filename, "r");
	temp = fopen(temp_filename, "w");

	if (file == NULL || temp == NULL){
		printf("Error opening file(s) for changing variable\n");
		return 1;
	}
	while (fgets(buffer, 100, file))
	{
		char *copy_of_buffer;
		copy_of_buffer = calloc(strlen(buffer)+1, sizeof(char)); 

    		strcpy(copy_of_buffer, buffer);
		char *var = strtok(buffer, ", ");
		printf("transfering: %s\n", var);
		printf("hole buffer: %s\n", copy_of_buffer);

		if (strcmp(var, variable) != 0){
			fputs(copy_of_buffer, temp);
		}
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
	printf("I am inside the change_variable fun\n");

	int deleted = delete_variable(variable);
	int added = add_variable(variable, value);

	if (deleted == 0 && added == 0){
		printf("the variable is updated\n");
		return 0;
	}
	else{
		printf("deleted: %d, added: %d", deleted, added); 
		printf("the variable is not updated and there is a problem either in delete or add functions\n");
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

    	if (0 == size) {
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
	printf("I am inside delete_all_variables fun\n");
	
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
	//////////////
	printf("created new empty DB.csv file\n");	
	return 0;
}
//------------------------------------------------------------------------------------------------
int print_local_var(){
	/////////////////////
	printf("I am inside print_local_var fun\n");

	FILE* file = fopen("data/DB.csv", "r");
	
	if (!file){
		printf("Can't open file\n");
		return 1;
	}
	
	char buffer[100];
	int row = 0;
	int column = 0;

	while (fgets(buffer, 100, file)){
		column = 0;

		char* value = strtok(buffer, ", ");

		while(value){
			if (column == 1)
				printf(" = ");
			printf("%s", value);
			value = strtok(NULL, ", ");
			column++;
		}
		printf("\n");
	}
	fclose(file);
	return 0;

}
//------------------------------------------------------------------------------------------------
char* value_of(char* variable){
	
	/////////////////////
	printf("I am inside value_of fun\n");

	FILE *file;
	char filename[] = "data/DB.csv";
	char buffer[100];
	file = fopen(filename, "r");

	if (file == NULL){
		printf("Error opening file\n");
		return NULL;
	}
	while (fgets(buffer, 100, file))
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
	printf("I am inside check_export fun\n");
	printf("buf after joining check_export = %s\n", buf);

	bool can_exp = true;
	int count = arg_count(buf);
	printf("count inside check_export = %d\n", count);
	printf("buf after getting the count inside check_export = %s\n", buf);

	char** arg_list= arg_values(count+1, buf, " ");
	printf("buf before copy_of_first_arg = %s\n", buf);

	char* copy_of_first_arg = malloc(strlen(arg_list[0])+1* sizeof(char));
	
	strcpy(copy_of_first_arg, arg_list[0]);
	
	if (strcmp(copy_of_first_arg, "export") == 0){
		printf("buf before starting the for loop inside check_export = %s\n", buf);

		for (int i = 1; i < count; i++){
			char *var;
			var = calloc(strlen(arg_list[i])+1, sizeof(char)); 
			strcpy(var, arg_list[i]);
			if(!varIsLocal(var)){

				printf("There is word export but there is variable that is not local\n");
				can_exp = false;
				break;
			}
		}
	}
	else{
		printf("There is no word export\n");
		can_exp = false;
	}
	printf("buf before exit the check_export = %s\n", buf);
	printf("can_exp is true\n");
	return can_exp;

}

//-------------------------------------------------------------------------------------------------

int export_variables(char* buf)
{
	////////////////
	printf("I am inside add_env fun and buf = %s\n", buf);
	int len;
	int count = arg_count(buf);
	char** arg_list= arg_values(count+1, buf, " ");
	for (int i = 1; i < count; i++)
	{
		char* val = value_of(arg_list[i]);
		if (val != NULL){
			int len = strlen(val);
			val[len - 1] = 0;
			/////////////////
			printf("var= %s, val = %s\n",arg_list[i], val); 
			printf("adding %s to env\n", val);
			
			const char *copy_of_val = (const char*)val;
			
			
			int status = setenv(arg_list[i], copy_of_val, 1);
			
			if (status == 0)
				printf("adding variable is worked properly\n");
			else
				printf("adding variable to env is failed:)\n");
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------


int main()
{
	// call delete_all_variables to delete the variable in database if exists
	delete_all_variables();

	while(1){	
		
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

		if (strcmp(buf, "set") == 0)
		{
			///////////////
			printf("I am inside set_comp\n");
			print_local_var();
			continue;
		}


		if (check_export(buf))
		{
			///////////////////
			printf("I am inside add_eniv comp\n");
			printf("buf before joining the export_variable = %s\n", buf);
			export_variables(buf);
			continue;
		}


		/////////////////
		printf("----------------------------\n");
		if (isVar(buf)){
			char** variable_and_value = arg_values(3, buf, "=");
			if (!varIsLocal(variable_and_value[0]))
			{	
				////////////////////
				printf("I am inside adding var comp\n");
			
				add_variable(variable_and_value[0], variable_and_value[1]);
			}
			else if (varIsLocal(variable_and_value[0])){
				////////////////////
				printf("I am inside changing var comp\n");
			
				change_variable(variable_and_value[0], variable_and_value[1]);
			}
			///////////////
			print_local_var();
			continue;

		}
		printf("----------------------------\n");
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
			printf("----------------------------\n");
			//////////////////
			printf("ret_pid == 0\n");
			count = arg_count(buf);
			printf("count of words: %d\n", count);
			char **arg_list = arg_values(count+1, buf, " ");
			printf("----------------------------\n");
			execvp(arg_list[0], arg_list);
			printf(CYN "command failed :)\n" RESET);
			return -1;
		}
	}
	return 0;

}

