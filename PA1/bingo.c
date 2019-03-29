#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void write1(int index);
void write2(int index, int id);

int main (int argc, char *argv[]){
	char input[] = " ";
	char *argument;

	printf("This is bingo.c\n\n");
	printf("choose the function(input number)\n");
	printf("--1. input userid\n");
	printf("--2. get user log\n");
	printf("--3. make imortal/mortal process\n");
	printf("--4. hide/show module\n");
	printf("----------------------\n");
	
	while(strcmp(input, "quit")!= 0){
		printf(">");
		scanf ("%s", input);

		if(strcmp(input,"1") == 0){
			int uid=0;

			printf("input user id : ");
			scanf("%d",&uid);

			write2(input,uid);

		}else if(strcmp(input,"2") == 0){
			int uid=0;
			index =1;

			printf("input user id : ");
			scanf("%d",&uid);

			

		}else if(strcmp(input,"3") == 0){
			int uid=0;
			index =1;

			printf("input user id : ");
			scanf("%d",&uid);

			hello_write(index,uid);

		}else if(strcmp(input,"4") == 0){
			int uid=0;
			index =1;

			printf("input user id : ");
			scanf("%d",&uid);

			hello_write(index,uid);

		}else{
			printf("no such function\n");
		}
		

	}

 }

void write1(int index) 
{
	FILE *file_pointer;
	file_pointer=fopen("/proc/hellokernelworld", "w");
	fprintf(file_pointer, "bingo.c");
	fclose(file_pointer);
	return  ;
}

void write2(int index, int id) 
{
	FILE *file_pointer;
	file_pointer=fopen("/proc/hellokernelworld", "w");
	fprintf(file_pointer, "bingo.c");
	fclose(file_pointer);
	return  ;
}
