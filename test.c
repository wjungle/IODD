#include <stdio.h>
#include <stdlib.h>
#include "lookdir.h"

int main()
{
	Songstruct *test = NULL;
	test = lookdir("/home/jungle/music");
	//printf("test address:%p\n", test);	

	while(test->song_name != NULL){
		printf("%s\n", test->song_name);
		test = test->nextPtr;
	}

}
