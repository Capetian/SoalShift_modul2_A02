#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

//referensi:
//https://gist.github.com/mplewis/5279108


void main() {

  int pid;
  int pipe1[2];
  int out;
  int status;

  if ((pid = fork()) == -1) {
    exit(1);
  } else if (pid == 0) {
    execlp("unzip", "unzip", "campur2.zip", NULL);
    _exit(1);
  }
  while((wait(&status)) > 0);

  if (pipe(pipe1) == -1) {
    exit(1);
  }
  out = open("daftar.txt",O_CREAT| O_APPEND | O_WRONLY);
  dup2(out,1);

  if ((pid = fork()) == -1) {
    exit(1);
  } else if (pid == 0) {
    dup2(pipe1[1], 1);
    close(pipe1[0]);
    close(pipe1[1]);

    execlp("ls", "ls", "./campur2/", NULL);
    _exit(1);
  }

  if ((pid = fork()) == -1) {
    exit(1);
  } else if (pid == 0) {
    dup2(pipe1[0], 0);
    close(pipe1[0]);
    close(pipe1[1]);
    execlp("grep", "grep", ".txt$", NULL);
    _exit(1);
  }

  close(pipe1[0]);
  close(pipe1[1]);



}




