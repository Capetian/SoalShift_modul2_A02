# LAPORAN PENJELASAN SOAL SHIFT MODUL 2

1.    Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.

### Jawab :
Mendeklarasikan variabel yang dibutuhkan :
```
  	struct dirent *de;				// pointer untuk mengakses file pada directory
	DIR   *drd;					// pointer untuk mengkases directory tujuan
	DIR   *dr;					// pointer untuk mengkases directory asal
  	char src[] = "/home/bimo/Desktop/testing/";	// path directory asal
  	char dest[] = "/home/bimo/modul2/";		// path directory tujuan
	char dest2[] = "/home/bimo/modul2/gambar/";	// path directory tujuan
	char ext[] = "_grey.png";			// extension yg diinginkan
	char newName[256];				//variable untuk menyimpan path dan nama file baru
        char oldName[256]; 				//variable untuk menyimpan path dan nama file lama
```

Membuat directory tujuan:
```
	*drd = opendir(dest2);
	if (drd == NULL)  				// cek jika directory tujuan ada dengan membuka directory
	{ 						// jika tujuan tidak ditemukan, buat directory tujuan
	  ...
	  execlp("mkdir","mkdir", dest, NULL);		// dengan fork buat directory modul2
	  ...
	  execlp("mkdir","mkdir", dest2, NULL);		// dengan fork buat directory gambar
	  ...
	else
	{
	  closedir(drd); 				// jika tujuan ditemukan, tutup directory		
	}
```

Melakukan penggantian nama & directory:
```
    DIR *dr = opendir(src);				// buka directory asal
    ...
    while ((de = readdir(dr)) != NULL) 			// scan directory asal, pointer "de" menunjuk file yg sedang di-scan
    {
      if (strstr(de->d_name, ".png") != NULL) {		// cek jika nama file (disimpan di d_name) yang sedang ditunjuk mengandung ".png"
        sprintf(oldName,"%s%s", src, de->d_name);	// tambahkan path lama ke nama file
	sprintf(newName,"%s%s", dest2, de->d_name);	// tambahkan path baru ke nama file
        int len = strlen(newName) + 1;  
        memmove(newName +(len-5), ext, strlen(ext)+1);	// gantikan extensi ".png" dengan "_grey.png"
        rename(oldName,newName);			// pindahkan ke directory baru sekaligus gantikan namanya 
	...
```
### Output:

![picture](/ouput/soal1-1.jpg)

![picture](/ouput/soal1-2.jpg)

![picture](/ouput/soal1-3.jpg)



Program dalam bentuk daemon, sehingga bisa mengubah nama dan directory secara otomatis saat dimasukkan ke suatu folder.

2.    Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file  bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

### Jawab :

Mendeklarasikan variabel yang dibutuhkan :

        struct stat owo;                                  //deklarasi variabel owo
        char nama_file[10] = "elen.ku";                   //deklarasi variabel nama_file
        char path[50] = "hatiku/", www[10] = "www-data";  //deklarasi variabel path dan www

setelah itu mengambil info (owner dan group) dari elen.ku

        stat("hatiku/elen.ku", &owo);                     //mengambil info elen.ku dan memasukkannya ke variabel owo
        struct passwd *pw = getpwuid(owo.st_uid);         //mengambil owner dari owo
        struct group *gr = getgrgid(owo.st_gid);          //mengambil group dari owo
        printf("%s %s\n", pw->pw_name, gr->gr_name);
       
mengubah permission elen.ku menjadi 777 :

	chmod (path, 0777);

lalu memeriksa apa group dan owner adalah www-data

        if(strcmp(pw->pw_name, www)==0 && strcmp(gr->gr_name, www)==0){         //memeriksa apakah owner dan group itu ww-data 
        strcat(path, nama_file);                                                //mengambil file beserta path dari elen.ku
        remove(path);                                                           //untuk menghapus elen.ku

lalu membuat agar daemon berjalan setiap 3 detik :

        sleep(3);                                                               //membuat daemon berjalan setiap 3 detik
	
### Output:

![picture](/ouput/soal2.jpg)

3.    Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
i)  mengekstrak file zip tersebut.
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
Catatan:  
  -Gunakan fork dan exec.
  -Gunakan minimal 3 proses yang diakhiri dengan exec.
  -Gunakan pipe
  -Pastikan file daftar.txt dapat diakses dari text editor

### Jawab :
Tujuan dari program ini adalah untuk melakukan unzip kemudian ls | grep.

Mendeklarasikan variabel yang dibutuhkan :
```
  int pipe1[2];				// dibuat untuk membuat pipe. pipe[0] menyimpan input sedangkan pipe[1] menyimpan output
  int out;				// dibuat untuk membuka file tujuan
```
Unzip arsip campur2.zip :

```
	if ((pid = fork()) == -1) {				// fork untuk melakukan unzip
	    exit(1);
	} else if (pid == 0) {
 	execlp("unzip", "unzip", "campur2.zip", NULL);		// lakukan unzip pada campur2.zip
	...
	while((wait(&status)) > 0);				// parent menunggu sampai unzip selesai
```
Siapkan pipe dengan :
```
  if (pipe(pipe1) == -1) {
    exit(1);
}
```

Siapkan file output, sehingga output stream dari ls | grep akan masuk ke file tujuan:
```
  out = open("daftar.txt",O_CREAT| O_APPEND | O_WRONLY);	// buka file daftar.txt
  dup2(out,1);							// dup2( , 1) digunakan untuk mengalihkan out ke output stream program ini
```
Buat fork untuk melakukan command:
```
  if ((pid = fork()) == -1) {					// fork untuk melakukan ls
    ...
    dup2(pipe1[1], 1);						// alihkan sisi output pipe ke output stream				
    execlp("ls", "ls", "./campur2/", NULL);			// lakukan ls pade 
  ...
  if ((pid = fork()) == -1) {					// fork untuk melakukan ls
    dup2(pipe1[0], 0);						// Alihkan sisi input pipe ke input stream, 
    								// sehingga output dari ls akan menjadi input stream untuk grep.
    execlp("grep", "grep", ".txt$", NULL);			// Lakukan grep untuk memfilter hasil dari ls sehingga hanya ada
								// nama file yg mengadung .txt pada akhir nama file tersebut.
```
Karena file daftar.txt sudah dialihkan ke output stream program ini, maka hasil dari ls | grep akan dimasukkan ke file tersebut.

### Output:

![picture](/ouput/soal3-1.jpg)

![picture](/ouput/soal3-2.jpg)

![picture](/ouput/soal3-3.jpg)

4.    Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh:
  File makan_enak.txt terakhir dibuka pada detik ke-1
  Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 
  -dilarang menggunakan crontab
  -Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

### Jawab :

mendeklarasikan counter yang akan digunakan saat membuat file yang baru :

        int counter=1;                                                          //mendeklarasi counter

mengubah counter yang integer menjadi string :

        char b[4];                                                              //mendeklarasikan b untuk menyimpan counter yang telah diubah 
	sprintf(b, "%d", counter);                                              //mengubah counter menjadi string
        
mengambil data-data yang diperlukan :

	char path[]="/home/paksi/Documents/makanan/makan_enak.txt";             //mendeklarasikan path untuk directory makan_enak.txt
	struct stat nama_file;                                                  //mendeklarasikan variabel nama_file
	stat(path, &nama_file);                                                 //mengambil info dari path lalu dimasukkan ke nama_file
        time_t bukacoding = time(NULL);                                         //mengambil waktu program dijalankan        
        time_t bukamakan = nama_file.st_atime;                                  //mengambil waktu terakhir kali makan_enak.txt dibuka

memeriksa apakah saat program dijalankan file telah dibuka setidaknya 30 detik sebelumnya :

        	if (difftime(bukacoding,bukamakan)<=30) {                       //memeriksa apa jeda waktu antara program dan waktu terakhir file adalah 30 detik

		char newpath[]="/home/paksi/Documents/makanan/makan_sehat";     //mendeklarasikan newpath untuk membuat makan_sehat.txt
		char type[]=".txt";                                             

membuat file makan_sehat.txt :

           	strcat(newpath,b);                                              //menggabungkan newpath dengan b
	   	strcat(newpath,type);                                           //menggabungkan newpath yang telah digabung b dengan type
           	FILE *sehat=fopen(newpath,"w");                                 //membuat file newpath
		fprintf(sehat,"Diet OwO");                                      //mengisi makan_sehat.txt dengan "Diet OwO"
           	fclose(sehat);                                                  //menutup makan_sehat.txt
		counter++;                                                      //menambahkan counter

membuat agar daemon berjalan setiap 5 detik :

        sleep(5);

### Output:

![picture](/ouput/soal4.jpg)

5.    Kerjakan poin a dan b di bawah:
a.Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
Ket:
  Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
  Per menit memasukkan log#.log ke dalam folder tersebut
  ‘#’ : increment per menit. Mulai dari 1
b.Buatlah program c untuk menghentikan program di atas.
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

### Jawab :

#### Untuk 5a.


Mendeklarasikan variabel yang dibutuhkan :
```
    int i=1;					// untuk penamman log
    int status;
    time_t t = time(NULL);			// digunakan untuk menyimpan waktu directory dibuat (dalam bentuk detik sejak waktu epoch)
    struct tm tm = *localtime(&t);		// mengubah waktu menjadi format waktu lokal
```
Membuat serta menamakan folder sesuai waktu folder dibuat:
```
 	sprintf(dir_name,"/home/bimo/Desktop/testing/%02d:%02d:%d-%02d:%02d", src,tm.tm_mday,  
		tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min); 	// namakan folder sesuai format waktu yg diinginkan
	execlp("mkdir", "mkdir", dir_name, NULL);				// buat folder tersebut
	...
	 while((wait(&status)) > 0);						// parent menunggu pembuatan folder selesai
```
Membuat log pada folder yang dibuat :
```
    for( i = 1; i <=30; i++)							// loop selama 30 x
    {
    ...
      sprintf(file_name,"%s/log%d.txt",dir_name,i);				// namakan file sesuai index 
      execlp("cp", "cp","/var/log/syslog",file_name, NULL);			// copy dan rename syslog sesuai
      ...
      sleep(60);								// sleep agar file dibuat setiap 1 menit
    }										// setelah 30 x looping 30 menit sudah berlalu
```
Karena program yg dibuat dalam bentuk daemon, program akan loop kembali ke awal untuk membuat folder baru dengan log mulai dari log1.log.


#### Output:

![picture](/ouput/soal5-1.jpg)

![picture](/ouput/soal5-2.jpg)

![picture](/ouput/soal5-3.jpg)

#### Untuk 5b.
Program di atas dapat dimatikan dengan mencari pid program tersebut kemudian melakukan kill dengan signal SIGKILL. Kita bisa mendapatkan pid proses dengan menulusiri /proc, yaitu directory yang menyimpan informasi tentang proses yang sedang berjalan. Pada directory tersebut kita akan mencari proses dengan command yang mengandung nama program yang kita ingin matikan.


Mendeklarasikan variabel yang dibutuhkan :
```
  struct dirent *de;				// pointer untuk mengscan /proc	
  DIR *dr = opendir("/proc");			// pointer untuk membuka /proc
  FILE *fp;					// file pointer untuk membuka /cmdline
  char *endptr;					// untuk melakukan strtol()
  char target[] = "soal5";			// yg ingin dimatikan adalah proses ./soal5
  char exception[] ="soal5b";			// exception agar tidak membunuh diri sendiri
  long findpid 					// findpid akan menyimpan pid proses
  char proc[100]; 				// proc akan menyimpan directory ke cmdline suatu proses
  char *cmp;				
```

Buka kemudian scan directory /proc dengan:
```
 DIR *dr = opendir("/proc");
 ...
 while ((de = readdir(dr)) != NULL) 
```
Pada directory /proc command disimpan dalam directory /proc/[pid]/cmdline. Oleh karena itu kita dapat mendapatkan pid suatu proses dengan mendapatkan nama folder tersebut.
```
	long findpid = strtol(de->d_name, &endptr, 10);			// ubah d_name (yg mengandung pid) menjadi long int, 
									// findpid akan menyimpan pid proses yang sedang ditinjau
	sprintf(proc,"/proc/%ld/cmdline",findpid);			// proc akan menyimpan directory ke cmdline suatu proses
```
Buka file /cmdline pada folder proses dan cocokan command dengan nama program yg ingin dicari:
```
fp = fopen(proc, "r");							// buka cmdline proses
    if (fp) {
      fgets(proc,sizeof(proc),fp);
      if ( (cmp = strstr(proc,target)) != NULL ) {			// cek jika cmdline proses mengandung nama program yg diinginkan
        if ( (cmp = strstr(proc,exception)) != NULL ) {			// cek jika proses bukan program ini
          continue;
        }
        else
        {
          kill((pid_t)findpid, SIGKILL);				// jika cmdline mengandung "soal5" matikan proses
          break;
}
```



#### Output:

![picture](/ouput/soal5b.jpg)

