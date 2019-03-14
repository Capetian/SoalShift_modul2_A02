# LAPORAN PENJELASAN SOAL SHIFT MODUL 2

1.    Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.

Jawab :


2.    Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file  bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

Jawab :

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

3.    Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
i)  mengekstrak file zip tersebut.
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
Catatan:  
  -Gunakan fork dan exec.
  -Gunakan minimal 3 proses yang diakhiri dengan exec.
  -Gunakan pipe
  -Pastikan file daftar.txt dapat diakses dari text editor

Jawab :


4.    Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh:
  File makan_enak.txt terakhir dibuka pada detik ke-1
  Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 
  -dilarang menggunakan crontab
  -Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

Jawab :

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

5.    Kerjakan poin a dan b di bawah:
a.Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
Ket:
  Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
  Per menit memasukkan log#.log ke dalam folder tersebut
  ‘#’ : increment per menit. Mulai dari 1
b.Buatlah program c untuk menghentikan program di atas.
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

Jawab :
