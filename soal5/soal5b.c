#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
//referensi:
//https://phoxis.org/2013/09/13/find-process-ids-of-a-running-process-by-name/ 
int main ()
{
  struct dirent *de;
  DIR *dr = opendir("/proc");
  FILE *fp;
  char *endptr;
  char target[] = "soal5";
  char exception[] ="soal5b";
  if (dr == NULL)  
  { 
    exit(EXIT_FAILURE);
  } 

  while ((de = readdir(dr)) != NULL) 
  {
    long findpid = strtol(de->d_name, &endptr, 10);
    char proc[100], *cmp;
    sprintf(proc,"/proc/%ld/cmdline",findpid);
    fp = fopen(proc, "r");
    if (fp) {
      fgets(proc,sizeof(proc),fp);
      if ( (cmp = strstr(proc,target)) != NULL ) {
        if ( (cmp = strstr(proc,exception)) != NULL ) {
          continue;
        }
        else
        {
          kill((pid_t)findpid, SIGKILL);
          break;
        }
        
      }
      
    }
    fclose(fp);
  } 

  closedir(dr);
}