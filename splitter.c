
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>


void splitter (char* a){
    char output[1024];
    sprintf(output,"%s.",a);
    pid_t child_id;
    int status;
    child_id = fork();
    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (child_id == 0) {
        
        char *argv[] = {"split", "-b", "1k","-a","3","-d", a , output, NULL};
        execv("/usr/bin/split", argv);
    } 
    else {
        while((wait(&status))>0);
        remove(a);
    } 
}



void join (char* a,char* b){
    FILE* mainj;
    char joined[1024];
    sprintf(joined,"/home/feraldy/Downloads/%s",a);
	mainj = fopen(joined,"a+");

    FILE* need;
	char this[1024];
	sprintf(this,"/home/feraldy/Documents/%s",b);
	need = fopen(this,"r");
	int c;
     while(1) {
   			c = fgetc(need);
   			if( feof(need) ) {
   				break;
   			}
   			fprintf(mainj,"%c",c);
   	}
}


int main() {
    DIR *d;
struct dirent *dir;
chdir("/home/feraldy/Documents");
  d = opendir("/home/feraldy/Documents");
        if(d){
        while((dir = readdir(d))!=NULL){
            if(dir->d_type != DT_DIR){ 
               splitter(dir->d_name);
            }
        }
    }closedir(d);

    d = opendir("/home/feraldy/Documents");
        if(d){
        while((dir = readdir(d))!=NULL){
            if(dir->d_type != DT_DIR){ 
               
               char nama[1024],full[1024],dest[1024];
               strcpy(full,dir->d_name);
               int len = strlen(full);
               strncpy(nama,full,len-4);
               join(nama,full);
               
            }
        }
    }closedir(d);
}
  

