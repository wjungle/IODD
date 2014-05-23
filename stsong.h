#include <stdlib.h>
struct songstruct{
  char *song_name;
  struct songstruct *nextPtr;
};
typedef struct songstruct Songstruct;

Songstruct *insert(Songstruct *sPtr,char *data)
{
  Songstruct *newptr;
  Songstruct *previousptr;
  Songstruct *currentptr;
  newptr=malloc(sizeof(Songstruct));
  if(newptr!=NULL)
  {
    newptr->song_name=data;
    newptr->nextPtr=NULL;
    previousptr=NULL;
    currentptr=sPtr;
    while(currentptr!=NULL)
    {
      previousptr=currentptr;
      currentptr=currentptr->nextPtr;
    }
    if(previousptr==NULL)
    {
      newptr->nextPtr=sPtr;
      sPtr=newptr;
    }else{
      previousptr->nextPtr=newptr;
      newptr->nextPtr=currentptr;
    }
  }else{
    printf("not insert.No memory size");
  }
  return sPtr;
}
