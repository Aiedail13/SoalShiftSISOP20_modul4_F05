# SoalShiftSISOP20_modul4_F05

Pada soal shift modul 4 kita diminta untuk membuat sebuah filesystem yang tugasnya melakukan enkripsi terhadap isi dari direktori /home/[user]/Documents. Secara singkatnya, filesystem ini memiliki 4 tugas utama. 
    1.  Tugas pertama adalah melakukan enkripsi caesar chiper. Direktori yang dienkripsi dengan caesar chiper adalah direktori yang dibuat dengan awalan "encv1_". Filesystem juga harus bisa melakukan enkripsi terhadap direktori yang di-rename dengan awalan encv1_ dan sebaliknya. 
    2.  Tugas kedua adalah melakukan split file menjadi beberapa bagian berukuran 1024bytes. Direktori yang dienkripsi dengan ara ini adalah direktori yang dibuat dengan awalan "encv2_". Filesystem juga harus bisa melakukan enkripsi terhadap direktori yang di-rename dengan awalan encv2_ dan sebaliknya. 
    3.  Tugas ketiga adalah filesystem harus mampu melakukan sinkronisasi 2 direktori secara otomatis.
    4.  Tugas keempat adalah mencatat perubahan yang terjadi dalam sebuah log.


```
void encrpt(char * input){
    int i;
    int shift;
    shift=10;
    int start=searchName(input);
    unsigned long int end=getExt(input);
    if((input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='/')||(input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='.'&&input[strlen(input)-3]=='/'))return;
    for(i=start;i < end;i++){
        if(input[i]=='/')continue;
        int id=where(input[i]);
        int newid=(id+shift)%strlen(customalpha);
        input[i]=customalpha[newid];       
    }

}
```

Fungsi diatas dilakukan untuk melakukan enkripsi terhadap sebuah string
dengan custom character yang digunakan untuk caesar chyper sebagai berikut

```
char customalpha[]={'9','(','k','u','@','A','W','1','[','L','m',
                    'v','g','a','x','6','q','`','5','Y','2','R',
                    'y','?','+','s','F','!','^','H','K','Q','i',
                    'B','X','C','U','S','e','&','0','M','.','b',
                    '%','r','I','\'','7','d',')','o','4','~','V',
                    'f','Z','*','{','#',':','}','E','T','t','$',
                    '3','J','-','z','p','c',']','l','n','h','8',
                    ',','G','w','P','_','N','D','|','j','O'};

```
```
void dencrpt(char * inputasli){
    char input[1024];
    char fpath[1024];
    sprintf(input,"%s",inputasli);
    int i;
    int shift;
    shift=10;
    unsigned long int end=getExt(input);
    int start=searchEncEnd(input);
    if((input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='/')||(input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='.'&&input[strlen(input)-3]=='/'))return;
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
    if(isFileExistsStats(fpath)){
        sprintf(inputasli,"%s",input);
    }else{
        sprintf(input,"%s",inputasli);
        end=strlen(input);
        if((input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='/')||(input[strlen(input)-1]=='.'&&input[strlen(input)-2]=='.'&&input[strlen(input)-3]=='/'))return;
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
}
```
Sedangkan untuk melakukan dencrypt dibuat fungsi deencrypt seperti diatas
 
 
```
void writeInfo(char *text, char* path)
{
    char* info = "INFO";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char log[1000];
    sprintf(log, "[%s]::[%04d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]::[%s]", info, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text, path);
    FILE *out = fopen("/home/seijaku/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return;
    
}
void writeInfo2Param(char *text, char* path, char * path2)
{
    char* info = "INFO";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char log[1000];
    sprintf(log, "[%s]::[%04d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]::[%s]::[%s]", info, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text, path,path2);
    FILE *out = fopen("/home/seijaku/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return;
    
}
void writeWarning(char *text, char* path)
{
    char* info = "WARNING";
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char log[1000];
    sprintf(log, "[%s]::[%04d][%02d][%02d]-[%02d]:[%02d]:[%02d]::[%s]::[%s]", info, tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, text, path);
    FILE *out = fopen("/home/seijaku/fs.log", "a");  
    fprintf(out, "%s\n", log);  
    fclose(out);  
    return;
    
}
```

Pada ketiga fungsi diatas digunakan untuk melakukan penyimpanan log. Untuk mendapatkan waktu digunakan struct tm dan time_t(menghgunakan fungsi time pada bahasa C). Lalu informasi dituliskan ke dalam file fs.log. 
    
