#define FUSE_USE_VERSION 28

#include <fuse.h>

#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <fcntl.h>

#include <dirent.h>

#include <errno.h>

#include <sys/time.h>
#include <sys/stat.h>
#include<stdbool.h>


static  const  char *dirpath = "/home/seijaku/Documents";
///////////////////////////1Util////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char customalpha[]={'9','(','k','u','@','A','W','1','[','L','m',
                    'v','g','a','x','6','q','`','5','Y','2','R',
                    'y','?','+','s','F','!','^','H','K','Q','i',
                    'B','X','C','U','S','e','&','0','M','.','b',
                    '%','r','I','\'','7','d',')','o','4','~','V',
                    'f','Z','*','{','#',':','}','E','T','t','$',
                    '3','J','-','z','p','c',']','l','n','h','8',
                    ',','G','w','P','_','N','D','|','j','O'};


unsigned long int getExt(char * input){
    int i;
    printf("get eXTXXX %s %lu\n",input,strlen(input));
    if(strcmp(input,".")==0||strcmp(input,"..")==0||input[strlen(input)-1]=='.')return strlen(input);
    else{
        for(i=strlen(input);i>=0;i--){
            if(input[i]=='/'){
                return strlen(input);
            }
            if(input[i]=='.'){
                return i;
            }
        }
    }
    
    return strlen(input);
}
int isFileExistsStats(const char *pathku)
{
    struct stat stats;

    stat(pathku, &stats);
    if (stats.st_mode & __S_IFREG){
        printf("INI FILE %s\n",pathku);
        return 1;
    }
    printf("Bukan FIle\n");
    return 0;
}
int searchEncEnd(char * input){
    int i;
    char * p=strstr(input,"encv1_");
    int encidx=p-input;
    for(i=encidx;i<strlen(input);i++){
        if(input[i]=='/'){
            return i+1;
        }
    }
    return strlen(input);
}

int where(char charku){
    int i;
    for(i=0;i<strlen(customalpha);i++){
        if(charku==customalpha[i]){
            return i;
        }
    }
    return -1;
}

int searchName(char * input){
    int i;

    int limit=searchEncEnd(input)-1;
    printf("search limit %d\n",limit);
    for(i=strlen(input)-1;i>=limit;i--){
        printf("search %c limit %d %d\n",input[i],i,limit);
        if(input[i]=='/'&& i >= limit){
            printf("found \\ at %d %d\n",i,limit);
            return i;
        }
    }
    return strlen(input);
}

void encrpt(char * input){
    int i;
    int shift;
    shift=10;
    int start=searchName(input);
    unsigned long int end=getExt(input);
    printf(" input %s %lu enc start %d  end %lu\n",input,strlen(input),start,end);
    if((input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='/')||(input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='.'&&input[strlen(input)-3]=='/'))return;
    printf("[+]enc %s to",input);
    for(i=start;i < end;i++){
        if(input[i]=='/')continue;
        int id=where(input[i]);
        int newid=(id+shift)%strlen(customalpha);
        input[i]=customalpha[newid];
        
    }
    printf(" %s\n",input);

}
void dencrpt(char * inputasli){
    char input[1024];
    char fpath[1024];
    sprintf(input,"%s",inputasli);
    int i;
    int shift;
    shift=10;
    unsigned long int end=getExt(input);
    int start=searchEncEnd(input);
    printf(" input %s %lu dec start %d  end %lu\n",input,strlen(input),start,end);
    if((input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='/')||(input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='.'&&input[strlen(input)-3]=='/'))return;
    printf("[-][-]dec %s\n",input);
    for(i=start;i < end;i++){
        if(input[i]=='/')continue;
        int id=where(input[i]);
        int newid;
        if(id-shift<0){
            newid=id-shift+strlen(customalpha);
        }
        else{
            newid=id-shift;
        }
        input[i]=customalpha[newid];
        
    }
    sprintf(fpath,"%s%s",dirpath,input);
    printf("dec fpath %s\n",fpath);
    if(isFileExistsStats(fpath)){
        printf("here %s\n",fpath);
        sprintf(inputasli,"%s",input);

    }else{
        sprintf(input,"%s",inputasli);
        end=strlen(input);
        if((input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='/')||(input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='.'&&input[strlen(input)-3]=='/'))return;
        printf("[-][-]dec %s\n",input);
        for(i=start;i < end;i++){
            if(input[i]=='/')continue;
            int id=where(input[i]);
            int newid;
            if(id-shift<0){
                newid=id-shift+strlen(customalpha);
            }
            else{
                newid=id-shift;
            }
            input[i]=customalpha[newid];
        }
        sprintf(inputasli,"%s",input);

    }
    printf("to %s\n",input);
}



//////////////////////////////////////////////////4Util////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void writeI(char *text, char* path)
{
    char* info = "INFO";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char log[1000];
    sprintf(log, "[%s]::[%02d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]::[%s]", info, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text, path);
    FILE *out = fopen("/home/seijaku/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return;
    
}
void writeR(char *text, char* path, char * path2)
{
    char* info = "INFO";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char log[1000];
    sprintf(log, "[%s]::[%02d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]::[%s]::[%s]", info, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text, path,path2);
    FILE *out = fopen("/home/seijaku/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return;
    
}
void writeW(char *text, char* path)
{
    char* info = "WARNING";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char log[1000];
    sprintf(log, "[%s]::[%04d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]::[%s]", info, tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text, path);
    FILE *out = fopen("/home/seijaku/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return;
    
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Util2/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void splitter (char* a){
//     char output[1024];
//     sprintf(output,"%s.",a);
//     pid_t child_id;
//     int status;
//     child_id = fork();
//     if (child_id < 0) {
//         exit(EXIT_FAILURE);
//     }

//     if (child_id == 0) {
        
//         char *argv[] = {"split", "-b", "1k","-a","3","-d", a , output, NULL};
//         execv("/usr/bin/split", argv);
//     } 
//     else {
//         while((wait(&status))>0);
//         remove(a);
//     } 
// }



// void join (char* a,char* b){
//     FILE* mainj;
//     char joined[1024];
//     sprintf(joined,"/home/feraldy/Downloads/%s",a);
// 	mainj = fopen(joined,"a+");

//     FILE* need;
// 	char this[1024];
// 	sprintf(this,"/home/feraldy/Documents/%s",b);
// 	need = fopen(this,"r");
// 	int c;
//      while(1) {
//    			c = fgetc(need);
//    			if( feof(need) ) {
//    				break;
//    			}
//    			fprintf(mainj,"%c",c);
//    	}
// }


// int main() {
//     DIR *d;
// struct dirent *dir;
// chdir("/home/feraldy/Documents");
//   d = opendir("/home/feraldy/Documents");
//         if(d){
//         while((dir = readdir(d))!=NULL){
//             if(dir->d_type != DT_DIR){ 
//                splitter(dir->d_name);
//             }
//         }
//     }closedir(d);

//     d = opendir("/home/feraldy/Documents");
//         if(d){
//         while((dir = readdir(d))!=NULL){
//             if(dir->d_type != DT_DIR){ 
               
//                char nama[1024],full[1024],dest[1024];
//                strcpy(full,dir->d_name);
//                int len = strlen(full);
//                strncpy(nama,full,len-4);
//                join(nama,full);
               
//             }
//         }
//     }closedir(d);
// }
  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
//getattr
static  int  xmp_getattr(const char *path, struct stat *stbuf)

{
    printf("getattr %s \n",path);
    int res;

    //printf("get attr dir :%s fpath: %s\n",dirpath,path);
    
    char fpath[1000];
	char spath[1000];
	sprintf(spath,"%s",path);
    if(strstr(spath,"encv1_")){
	    dencrpt(spath);
    }
    sprintf(fpath,"%s%s",dirpath,spath);
    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;

}

  
//readdir
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)

{

    //printf("1 readddir path %s\n",path);
    char fpath[1000];
	char spath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);

    }

    else {
        
        sprintf(spath,"%s",path);
        if(strstr(spath,"encv1_")){
            dencrpt(spath);
        }
        sprintf(fpath,"%s%s",dirpath,spath);
    }
    //printf("2 readdir dir :%s fpath: %s\n",fpath,spath);
    int res = 0;

    

    DIR *dp;

    struct dirent *de;

    

    (void) offset;

    (void) fi;

    

    dp = opendir(fpath);
    //printf("[3]opendir %s\n",fpath);

    if (dp == NULL)
        return -errno;

    

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;

        st.st_mode = de->d_type << 12;
        char buff[1000];
		sprintf(buff, "%s/%s", path,de->d_name);
        //printf("[4]buff %s\n",buff);
        char *p=strstr(buff,"encv1_");
        if(p){
            
		    encrpt(buff);
        }
        sprintf(buff, "%s", buff+strlen(path)+1);

        res = (filler(buf, buff, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;

}

  

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)

{

    //printf(" readd %s",path);
    char fpath[1000];
	char spath[1000];
	
    //printf("read %s\n",path);
    if(strcmp(path,"/") == 0||strcmp(path,".") == 0||strcmp(path,"..") == 0)

    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else {
        sprintf(spath,"%s",path);
        if(strstr(spath,"encv1_")){
		    dencrpt(spath);
        }
        sprintf(fpath,"%s%s",dirpath,spath);
    }
    //printf("read dir :%s fpath: %s\n",dirpath,path);
    int res = 0;

    int fd = 0 ;

    

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1)
        return -errno;

    

    res = pread(fd, buf, size, offset);

    if (res == -1)
        res = -errno;

    

    close(fd);

    return res;

}
static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    int i;
    char jalan[1024];
    char name[1024];
    char jalan2[1024];
    if(strstr(path,"encv1_")){
        for(i=strlen(path);i>=0;i--){
            if(path[i]=='/'){
                break;
            }
        }
        sprintf(name,"%s",path+i);
        strcpy(jalan,"");
        strncpy(jalan,path,strlen(path)-strlen(name));
        dencrpt(jalan);
        sprintf(jalan2,"%s%s%s",dirpath,jalan,name);
    }else{
        sprintf(jalan2,"%s%s",dirpath,path);
    }
    printf("path %s jalan name %s%s\n",path,jalan,name);
    printf("-------mkdir %s\n",jalan2);
    sleep(1);
	res = mkdir((const char*)jalan2, mode);
	if (res == -1){
		return -errno;
    }else
    {
        writeI("MKDIR", jalan2);
    }
    

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	
    printf("[----]from %s to %s\n",from,to);
    
    int res,i;
    char dari[1024],ke[1024],name[1024],jalan[1024];
    sprintf(dari,"%s%s",dirpath,from);
    sprintf(ke,"%s%s",dirpath,to);
    if(strstr(dari,"encv1_")){
        dencrpt(dari);
        printf("[---mv dari %s\n",dari);
        for(i=strlen(dari);i>=0;i--){
            if(dari[i]=='/'){
                break;
            }
        }
        sprintf(name,"%s",dari+i);
        printf("[---mv  name %s\n",name);
        strcpy(jalan,"");
        strncpy(jalan,dari,strlen(dari)-strlen(name));
        printf("[---mv dari jalan%s\n",jalan);
        sprintf(dari,"%s%s",jalan,name);
    }
    if(strstr(ke,"encv1_")){
        printf("[---mv ke %s\n",ke);
        for(i=strlen(ke);i>=0;i--){
            if(ke[i]=='/'){
                break;
            }
        }
        sprintf(name,"%s",ke+i);
        printf("[---mv ke name %s\n",name);
        dencrpt(ke);
        strcpy(jalan,"");
        strncpy(jalan,ke,strlen(ke)-strlen(name));
        printf("[---mv ke jalan %s\n",jalan);
        sprintf(ke,"%s%s",jalan,name);
    }
    printf("[----------------]dari %s ke %s",dari,ke);
	res = rename(dari, ke);
	if (res == -1){
		return -errno;
    }else{
        writeR("MOVE", (char*)from,(char*)to);
    }
	return 0;
}



static struct fuse_operations xmp_oper = {

.getattr = xmp_getattr,
.readdir = xmp_readdir,
.read = xmp_read,
.mkdir		= xmp_mkdir,
.rename		= xmp_rename
};

  

int  main(int  argc, char *argv[])

{

umask(0);

return fuse_main(argc, argv, &xmp_oper, NULL);

}