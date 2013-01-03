#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COMMAND_CHARS	256
#define MAX_ARG_NUM	20
#define MAX_IO_NUM	2

enum{
	NORMAL,
	INPUT,
	OUTPUT
};

enum{
	UNSET,
	SET
};

int state = NORMAL;
char * found = NULL;
char * token = NULL;

char command[MAX_COMMAND_CHARS];
char *args[MAX_ARG_NUM];
char *ios[MAX_IO_NUM];
int iof[MAX_IO_NUM];
int args_i = 0, ios_i = 0;
char * save_ptr;

void init()
{
	state = NORMAL;
	found = NULL;
	token = NULL;

	args_i = ios_i = 0;
	bzero(command,MAX_COMMAND_CHARS);
	bzero(args,sizeof(char*) * MAX_ARG_NUM);
	bzero(ios,sizeof(char*) * MAX_IO_NUM);
	bzero(iof,sizeof(int) * MAX_IO_NUM);
}

void print_args()
{
	int i;
	for(i=0;i<args_i;i++){
		printf(",argument:%s",args[i]);
	}
	
	if(iof[0] | iof[1]){
		printf(",in-file:");
		if(ios[0])
			printf("%s",ios[0]);
		else 
			printf("NULL");

		printf(",out-file:");
		if(ios[1])
			printf("%s",ios[1]);
		else 
			printf("NULL");
	}
	printf("\n");
}

void getinputs(){
	if( !fgets(command, MAX_COMMAND_CHARS, stdin) ) 
		exit(0);

	save_ptr = strchr(command,'\n');
	*save_ptr = ' ';

	token = strtok_r(command," ",&save_ptr);

	if(0 == strcmp("exit",token))
		exit(0);
}
	

int main(int argc,char * argv[]){

	while(1){
		printf("mysh%% ");

		init();

		getinputs();

		printf("Show the command:\n");
		printf("command:%s",token);

		while( (token = strtok_r(NULL," ",&save_ptr)) != NULL){
		
			if(*token == '<'){
				iof[0] = SET;			
				if( isgraph(*(token + 1))){
					ios[0] = token + 1;
					state = NORMAL; 
				} else {
					state = INPUT;
				}

			} else if(*token == '>'){
				iof[1] = SET;
				if( isgraph(*(token + 1))){
					ios[1] = token + 1;
					state = NORMAL;
				}else{
					state = OUTPUT;
				}
			} else {
				switch(state){
				case NORMAL:
					if((found = strchr(token,'<'))){
						*found = '\0';
						args[args_i++] = token;
						iof[0] = SET;
						if( isgraph(*(found+1))) {
							ios[0] = found + 1;
							state = NORMAL;
						}else{
							state = INPUT;
						}
					}else if((found = strchr(token,'>'))){
						*found = '\0';
						args[args_i++] = token;
						iof[1] = SET;
						if( isgraph(*(found+1))) {
							ios[1] = found + 1;
							state = NORMAL;
						}else{
							state = OUTPUT;
						}
					
					}else{
						args[args_i++] = token;
					}

					break;
				case INPUT:
					if(! isspace(*token) ){
						ios[0] = token;
						state = NORMAL;
					}
					break;
				case OUTPUT:
					if(! isspace(*token) ){
						ios[1] = token;
						state = NORMAL;
					}
					break;
				default:
					break;	
			    }
			}

		}
        
		print_args();
	}
	return 0;	
}
