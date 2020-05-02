# SoalShiftSISOP20_modul4_F05

Pada soal shift modul 4 kita diminta untuk membuat sebuah filesystem yang tugasnya melakukan enkripsi terhadap isi dari direktori /home/[user]/Documents. Secara singkatnya, filesystem ini memiliki 4 tugas utama. 
    1.  Tugas pertama adalah melakukan enkripsi caesar chiper. Direktori yang dienkripsi dengan caesar chiper adalah direktori yang dibuat dengan awalan "encv1_". Filesystem juga harus bisa melakukan enkripsi terhadap direktori yang di-rename dengan awalan encv1_ dan sebaliknya. 
    2.  Tugas kedua adalah melakukan split file menjadi beberapa bagian berukuran 1024bytes. Direktori yang dienkripsi dengan ara ini adalah direktori yang dibuat dengan awalan "encv2_". Filesystem juga harus bisa melakukan enkripsi terhadap direktori yang di-rename dengan awalan encv2_ dan sebaliknya. 
    3.  Tugas ketiga adalah filesystem harus mampu melakukan sinkronisasi 2 direktori secara otomatis.
    4.  Tugas keempat adalah mencatat perubahan yang terjadi dalam sebuah log.


## Nomor 1
Enkripsi versi 1:
1. Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
2. Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
3. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip.
4. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
5. Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key.


>9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO

Misal kan ada file bernama “kelincilucu.jpg” dalam directory FOTO_PENTING, dan key yang dipakai adalah 10

>“encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg

Note : Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt.

6. Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya.

Pada nomor 1 terjadi kesalahan kamu dalam mengartikan maksud soal sehingga sistem kami menampilkan file yang terenkripsi pada fuse dan file pada Document tetap normal.

```c
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

Pada fungsi encrpt path akan di enkripsi denga shift 10, yang di enkripsi oleh fungsi ini adalah nama file/direktori, untuk mengecek ekstensi file kita menggunakan funsi getExt() yang mengembalikan index character '.' pada file, dan jika tidak ditemukan '.' hinggga menemukan '/' maka akan dikembalikan strlen dari inputan. Return value dari getExt akan menjadi batas kita melakukan enkripsi.

Fungsi diatas dilakukan untuk melakukan enkripsi terhadap sebuah string
dengan custom character yang digunakan untuk caesar chyper sebagai berikut

```c
char customalpha[]={'9','(','k','u','@','A','W','1','[','L','m',
                    'v','g','a','x','6','q','`','5','Y','2','R',
                    'y','?','+','s','F','!','^','H','K','Q','i',
                    'B','X','C','U','S','e','&','0','M','.','b',
                    '%','r','I','\'','7','d',')','o','4','~','V',
                    'f','Z','*','{','#',':','}','E','T','t','$',
                    '3','J','-','z','p','c',']','l','n','h','8',
                    ',','G','w','P','_','N','D','|','j','O'};

```
```c
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

Sedangkan untuk melakukan dencrypt dibuat fungsi dencrypt seperti diatas.
Pada fungsi dekrips hampir sama caranya cuma karena kita tidak bisa melakukan pengecekan apakh direktori yang masuk adalah file atau folder maka pertama kita asumsikan semua adalah file. kemudian kita dekripsi. Dekripsi dimulai dari path didalam folder encv1_ hingga ekstensi file. Jika path yang asli sudah ditemukan maka kita lakukan pengecekan apakah pada path tersebut benar sebuah file dengan isFileExistsStats(). Kalau iya maka dekripsi kita selesai, kalau tidak maka kita dekripsi lagi tanpa memperdulikan ekstensi.


Beberapa fungsi fuse yang sudah berhasil diimplementasi.

```c
static  int  xmp_getattr(const char *path, struct stat *stbuf)

{
    int res;
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
```
pada fungsi getattr jika berada pada direktori encv1_ maka akan di dekripsi terlebih dahulu untuk mendapatkan path originalnya.

```c

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
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
    int res = 0;
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;
    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;

        st.st_mode = de->d_type << 12;
        char buff[1000];
		sprintf(buff, "%s/%s", path,de->d_name);
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
```

Pada readdir jika direktori berada pada encv1_ maka direktori yang dibaca akan didekripsi terlebih dahulu. jika sudah terdekripsi maka path tersebut akan di buka. Selama membuka path tersebut nama nama yang ada pada direktori itu akan kita enkripsi.

```c
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)

{
    char fpath[1000];
	char spath[1000];
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
    int res = 0;
    int fd = 0 ;
    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1){
        return -errno;
    }else{
        writeInfo("CAT", (char*)fpath);
    }
    res = pread(fd, buf, size, offset);

    if (res == -1)
        res = -errno;
    close(fd);

    return res;
}
```
pada fungsi xmp_read disini juga jika file berada pada direktori encv1_ maka akan didekripsi terlebih dahulu dengan fungsi dencrpt(). Kemudian mengggunakan fungsi bawaan fuse open untuk membuka file. Jika file berhasil dibuka maka akan tercatat di fs.log untuk implementasi nomor 4 menggunakan fungsi  writeInfo().

```c
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
	res = mkdir((const char*)jalan2, mode);
	if (res == -1){
		return -errno;
    }else
    {
        writeInfo("MKDIR", jalan2);
    }
    

	return 0;
}
```

Untuk mkdir jika ada paa direktori encv1_ kita perlu melakukan dekripsi terhadap path selain nama dari folder itu. Kemudian menggunakan fungsi bawaan mkdir(). Jika folder berhasil buat maka akan tercatat di fs.log untuk implementasi nomor 4 menggunakan fungsi  writeInfo().

```c
static int xmp_rmdir(const char *path)
{
    char jalan[1024];
    char jalan2[1024];
    if(strstr(path,"encv1_")){
        sprintf(jalan,"%s",path);
        dencrpt(jalan);
        sprintf(jalan2,"%s%s",dirpath,jalan);
    }else{
        sprintf(jalan2,"%s%s",dirpath,path);
    }
	int res;
	res = rmdir(jalan2);
	if (res == -1){
		return -errno;
    }else{
        writeWarning("RMDIR", jalan2);
    }

	retu
    rn 0;
}
```

```c
static int xmp_rename(const char *from, const char *to)
{
    int res,i;
    char dari[1024],ke[1024],name[1024],jalan[1024];
    sprintf(dari,"%s%s",dirpath,from);
    sprintf(ke,"%s%s",dirpath,to);
    if(strstr(dari,"encv1_")){
        dencrpt(dari);
        for(i=strlen(dari);i>=0;i--){
            if(dari[i]=='/'){
                break;
            }
        }
        sprintf(name,"%s",dari+i);
        strcpy(jalan,"");
        strncpy(jalan,dari,strlen(dari)-strlen(name));
        sprintf(dari,"%s%s",jalan,name);
    }
    if(strstr(ke,"encv1_")){
        for(i=strlen(ke);i>=0;i--){
            if(ke[i]=='/'){
                break;
            }
        }
        sprintf(name,"%s",ke+i);
        dencrpt(ke);
        strcpy(jalan,"");
        strncpy(jalan,ke,strlen(ke)-strlen(name));
        sprintf(ke,"%s%s",jalan,name);
    }
 	res = rename(dari, ke);
	if (res == -1){
		return -errno;
    }else{
        writeInfo2Param("MOVE", (char*)from,(char*)to);
    }
	return 0;
}
```

Untuk rmdir jika dilakukan pada direktori encv1_ kita perlu melakukan dekripsi terhadap path nya. Kemudian menggunakan fungsi bawaan rmdir(). Jika folder berhasil hapus maka akan tercatat di fs.log untuk implementasi nomor 4 menggunakan fungsi  writeWarning().
fungsi rename untuk merename suatu file bisa menggunakan perintah mv. Untuk argumen from kita perlu melakukan dekripsi untuk semua pathnya. kemudain untuk argumen to kita harus melakukan dekripsi untuk path selain namanya. kemudain menggunakan fungsi rename() untuk melakukan rename file itu. Untuk implementasi nomor 4 menggunakan writeInfo2Param().
 
 Kendala yang kami temui dalam implementasi sistem ini adalah
- path yang di passing adalah relative path sehingga sering kali tidak terbaca.
- susah mendeteksi letak kesalahan yang terjadi karena sering nya suatu system call dipanggil dalam menjalankan suatu command.

Untuk sistem dimana file pada Documents direname belum sempat kami implementasi. Tapi kami sudah mencoba dan menemukan beberapa kendala 
- System call yang terus di panggil berkali kali menyebabkan susah melakukan debugging pada enkripsi file/folder. 
- Melakukan tracking pada file yang sudah dienkripsi. Sering kali terjadi suatu file/folder dienkripsi berkali - kali. Kami belum menemukan letak yang tepat untuk melakukan enkripsi.


## Nomor 4
Log system:

1. Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
2. Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu INFO dan WARNING.
3. Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
4. Sisanya, akan dicatat dengan level INFO.
5. Format untuk logging yaitu:
   >[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]

   

```c
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
    
