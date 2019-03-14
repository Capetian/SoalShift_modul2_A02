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
#include <dirent.h>
//referensi:
//https://stackoverflow.com/questions/31156400/rename-files-in-folder-using-c-code

int main() {
  pid_t pid, sid;
  struct dirent *de;
  int status;
  char src[] = "/home/bimo/Desktop/testing/";
  char dest[] = "/home/bimo/modul2/";
  char dest2[] = "/home/bimo/modul2/gambar/";
  char ext[] = "_grey.png";
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
    
    DIR *drd = opendir(dest2);  

    if (drd == NULL)  
    { 
      if ((pid = fork()) == -1) {
        exit(1);
      } 
      else if (pid == 0) {  
        execlp("mkdir","mkdir", dest, NULL);
        _exit(1);
      }
      while((wait(&status)) > 0);
      if ((pid = fork()) == -1) {
        exit(1);
      } 
      else if (pid == 0) {  
        execlp("mkdir","mkdir", dest2, NULL);
        _exit(1);
      }
      while((wait(&status)) > 0);
    }
    else
    {
      closedir(drd); 
    }
    
    DIR *dr = opendir(src);
    if (dr == NULL)  
    { 
      exit(EXIT_FAILURE);
    } 

    while ((de = readdir(dr)) != NULL) 
    {
      if (strstr(de->d_name, ".png") != NULL) {

        char newName[256];
        char oldName[256]; 
        sprintf(newName,"%s%s", dest2, de->d_name);
        sprintf(oldName,"%s%s", src, de->d_name);
        int len = strlen(newName) + 1;  
        memmove(newName +(len-5), ext, strlen(ext)+1);   
        rename(oldName,newName);
        
      }

    } 
    closedir(dr);
    sleep(3);
  }

  exit(EXIT_SUCCESS);
  return 0;
}
