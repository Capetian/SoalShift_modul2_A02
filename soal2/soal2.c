#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main() {
  pid_t pid, sid;

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

  if ((chdir("/home/paksi/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
	struct stat owo;
        char nama_file[10] = "elen.ku";
        char path[50] = "hatiku/", www[10] = "www-data";
	chmod (path, 0777);

        stat("hatiku/elen.ku", &owo);
        struct passwd *pw = getpwuid(owo.st_uid);
        struct group *gr = getgrgid(owo.st_gid);
        printf("%s %s\n", pw->pw_name, gr->gr_name);
        if(strcmp(pw->pw_name, www)==0 && strcmp(gr->gr_name, www)==0){ 
        strcat(path, nama_file);
        remove(path);
        }
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
  return(0);
}
