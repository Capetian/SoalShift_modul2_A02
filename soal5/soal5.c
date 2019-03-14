#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

//https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program
int main() {
  pid_t pid, sid;
  char src[] = "/home/bimo/Desktop/testing/";
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

  if ((chdir(src)) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    int i=1;
    int status;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char dir_name[256];
    char file_name[256];
    sprintf(dir_name,"/home/bimo/Desktop/testing/%02d:%02d:%d-%02d:%02d", src,tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    if ((pid = fork()) == -1) {
      exit(1);
    }
    else if (pid == 0) {
      execlp("mkdir", "mkdir", dir_name, NULL);
      _exit(1);
    }
      while((wait(&status)) > 0);

    for( i = 1; i <=30; i++)
    {
      if ((pid = fork()) == -1) {
      exit(1);
      }
      else if (pid == 0) {
      sprintf(file_name,"%s/log%d.txt",dir_name,i);
      execlp("cp", "cp","/var/log/syslog",file_name, NULL);
      _exit(1);
      }
      while((wait(&status)) > 0);
      sleep(60);
    }
    


 
    

  }
  
  exit(EXIT_SUCCESS);
}