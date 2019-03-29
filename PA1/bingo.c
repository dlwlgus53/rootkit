#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main (int argc, char *argv[]){
	char input[] = " ";
	char *argument;
	printf("This is bingo.c\n");
	

	while(strcmp(input, "quit")!= 0){
		printf("choose the function\n");
		printf("--1. input userid\n");
		printf("--2. get user log\n");
		printf("--3. make imortal/mortal process\n");
		printf("--4. hide/show module\n");
		printf("----------------------\n");
		printf(">");
		scanf ("%s", input);
		if(strcmp(input,"clear") == 0){
			printf("c\n");
		}
	}

 }
