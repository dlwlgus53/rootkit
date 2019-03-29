#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main (int argc, char *argv[]){
	char input[] = " ";
	char *argument;
	while(strcmp(input, "quit")!= 0){
		printf(">");
		scanf ("%s", input);
		if(strcmp(input,"clear") == 0){
			printf("c\n");
		}
	}

 }
