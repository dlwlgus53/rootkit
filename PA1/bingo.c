#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main (int argc, char *argv[]){
	char input[] = " ";
	char *argument;
	printf("This is bingo.c\n");
	

	while(strcmp(input, "quit")!= 0){
		printf("choose the function(input number)\n");
		printf("--1. input userid\n");
		printf("--2. get user log\n");
		printf("--3. make imortal/mortal process\n");
		printf("--4. hide/show module\n");
		printf("----------------------\n");
		printf(">");
		scanf ("%s", input);
		if(strcmp(input,"1") == 0){
			printf("1\n");
		}else if(strcmp(input,"2") == 0){
			printf("2\n");
		}else if(strcmp(input,"3") == 0){
			printf("3\n");
		}else if(strcmp(input,"4") == 0){
			printf("4\n");
		}else{
			printf("no such function\n");
		}
		

	}

 }

void hello_write(void) 
{
	FILE *file_pointer;
	file_pointer=fopen("/proc/openhook3", "w");
	fprintf(file_pointer, "bingo.c");
	fclose(file_pointer);
	return  ;
}
