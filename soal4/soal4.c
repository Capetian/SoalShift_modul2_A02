#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>
#include <string.h>

int main() {
  pid_t pid, sid;
  FILE *fp, *fp2;
  int c;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

 int counter=1;

  while(1) {

	char b[4];
	sprintf(b, "%d", counter);
	char path[]="/home/paksi/Documents/makanan/makan_enak.txt";
	struct stat nama_file;
	stat(path, &nama_file);
        time_t bukacoding = time(NULL);
        time_t bukamakan = nama_file.st_atime;

        if (difftime(bukacoding,bukamakan)<=30) {
;
		char newpath[]="/home/paksi/Documents/makanan/makan_sehat";
		char type[]=".txt";

           	strcat(newpath,b);
	   	strcat(newpath,type);
           	FILE *sehat=fopen(newpath,"w");
		fprintf(sehat,"Diet OwO");
           	fclose(sehat);
		counter++;
	}
	sleep(5);
  }
  exit(EXIT_SUCCESS);
}
