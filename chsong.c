#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include "lookdir.h"
void *get_pthread(void *arg);
void *print_pthread(void *arg);
void print();
int fd_pipe[2];
int fd_fifo;
Songstruct *listptr;
int main(int argc,int *argv[])
{
  int fd;
  pid_t pid;
  char buf[100];
  unlink("/opt/my_fifo");
  mkfifo("/opt/my_fifo",O_CREAT|0666);
  perror("mkfifo");
  if(pipe(fd_pipe)<0){perror("pipe error\n");exit(-1);}
  pid=fork();
  if(pid<0)
  {
    perror("fork");
  }else if(pid==0)
  {
    close(fd_pipe[0]);
    dup2(fd_pipe[1],1);
    printf("child1\n");
    fd_fifo=open("/opt/my_fifo",O_RDWR);
    printf("child2\n");
    execlp("mplayer","mplayer","-slave","-quiet","-input","file=/opt/my_fifo","/music/Sleep Away.mp3",NULL);
  }else
  { 
    pthread_t tid1;
    pthread_t tid2;
    fd_fifo=open("/opt/my_fifo",O_RDWR);
    pthread_create(&tid1,NULL,get_pthread,NULL);
    pthread_create(&tid2,NULL,print_pthread,NULL);
    pthread1_join(tid1,NULL);
    pthread_join(tid2,NULL);
  }
  return 0;
}
void *get_pthread(void *arg)
{
	 //listptr是一個只到Songstruct 結構的指標(宣告在上面，我把它宣告成全域變數)，而該結構Songstruct 是我自己定義的，在stsong.h中
  listptr=lookdir("/music/");//lookdir參數為要搜尋目錄的路徑，根據自己版子歌曲所在的目錄決定
  printf("----------------%s\n",listptr->song_name);  //test 這裡能正確印出來該目錄的檔案內容，代表串列有製作成功
  printf("----------------%s\n",listptr->nextPtr->song_name);  //test 這裡能正確印出來該目錄的檔案內容，代表串列有製作成功
  printf("----------------%s\n",listptr->nextPtr->nextPtr->song_name);  //test 這裡能正確印出來該目錄的檔案內容，代表串列有製作成功
  char buf[100];
   printf("*******************buf=%p,,,listptr=%p\n",buf,listptr);
  while(1)
  {
   printf("thread1\n");
   printf("please input your cmd:");
   //if(listptr->song_name==listptr->song_name){printf("**********111   listptr->song_name==%s\n",listptr->song_name);}
   fflush(stdout);

   printf("\nbefore fgets********* listprt->song_name=%s\n",listptr->song_name);//test，映的出listptr->song_name
   fgets(buf,sizeof(buf),stdin);

   if(listptr==NULL){printf("*************listptr==NULL\n");}  //test，覺得fgets有問題，檢察listptr這個指標還在不在，結果listptr還存在，所以不會進這個if，會到else
   else if(listptr->song_name=="chsongarm"){printf("**********   listptr->song_name==chsongarm\n");}  //test，這裡也不會執行
   else{printf("************after fgets*********listptr->song_name=%s\n",listptr->song_name);}  //test ，會進入這裡 映不出listptr->song_name
   
   buf[strlen(buf)]='\0';

   printf("buf[0]=%c\n",buf[0]);
   
   if(buf[0]=='6'){printf("%s\n",listptr->song_name);}  //test，不用理她
   
   printf("*%s*\n",buf);
   if(write(fd_fifo,buf,strlen(buf))!=strlen(buf)){perror("write");}
  }  
}
void *print_pthread(void *arg)
{
  char buf[100];
  close(fd_pipe[1]);
  int size=0;
  while(1)
  {
     printf("thread2");
     size=read(fd_pipe[0],buf,sizeof(buf));
     buf[size]='\0';
     //sleep(3);
     printf("the msg read from pipe is %s\n",buf); 
  }
}
void print(){
   printf("listptr->song_name=%s\n",listptr->song_name);
}
