#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "stsong.h"
#include <stdlib.h>
Songstruct *lookdir(char *dir)
{
  struct dirent *entry;
  struct stat statbuf;
  Songstruct *startptr=NULL;
  DIR *dp;
  if((dp=opendir(dir))==NULL)
  {
    fprintf(stderr,"cannot open directory: %s\n",dir);
    return;
  }
  while((entry=readdir(dp))!=NULL)
  { 
    lstat(entry->d_name,&statbuf);
    if(S_ISDIR(statbuf.st_mode))
    {
    }else
    {
      //printf("%s\n",entry->d_name);
      startptr=insert(startptr,entry->d_name);
    }
  }
  closedir(dp);
  return startptr;
}


