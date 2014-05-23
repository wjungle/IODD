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
	 //listptr�O�@�ӥu��Songstruct ���c������(�ŧi�b�W���A�ڧ⥦�ŧi�������ܼ�)�A�Ӹӵ��cSongstruct �O�ڦۤv�w�q���A�bstsong.h��
  listptr=lookdir("/music/");//lookdir�ѼƬ��n�j�M�ؿ������|�A�ھڦۤv���l�q���Ҧb���ؿ��M�w
  printf("----------------%s\n",listptr->song_name);  //test �o�̯ॿ�T�L�X�Ӹӥؿ����ɮפ��e�A�N���C���s�@���\
  printf("----------------%s\n",listptr->nextPtr->song_name);  //test �o�̯ॿ�T�L�X�Ӹӥؿ����ɮפ��e�A�N���C���s�@���\
  printf("----------------%s\n",listptr->nextPtr->nextPtr->song_name);  //test �o�̯ॿ�T�L�X�Ӹӥؿ����ɮפ��e�A�N���C���s�@���\
  char buf[100];
   printf("*******************buf=%p,,,listptr=%p\n",buf,listptr);
  while(1)
  {
   printf("thread1\n");
   printf("please input your cmd:");
   //if(listptr->song_name==listptr->song_name){printf("**********111   listptr->song_name==%s\n",listptr->song_name);}
   fflush(stdout);

   printf("\nbefore fgets********* listprt->song_name=%s\n",listptr->song_name);//test�A�M���Xlistptr->song_name
   fgets(buf,sizeof(buf),stdin);

   if(listptr==NULL){printf("*************listptr==NULL\n");}  //test�Aı�ofgets�����D�A�˹�listptr�o�ӫ����٦b���b�A���Glistptr�٦s�b�A�ҥH���|�i�o��if�A�|��else
   else if(listptr->song_name=="chsongarm"){printf("**********   listptr->song_name==chsongarm\n");}  //test�A�o�̤]���|����
   else{printf("************after fgets*********listptr->song_name=%s\n",listptr->song_name);}  //test �A�|�i�J�o�� �M���Xlistptr->song_name
   
   buf[strlen(buf)]='\0';

   printf("buf[0]=%c\n",buf[0]);
   
   if(buf[0]=='6'){printf("%s\n",listptr->song_name);}  //test�A���βz�o
   
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
