#ifndef MYLIST__
#define MYLIST__ 1

#include "helpers.h"

#define list struct genCell*
#define debug() printf("%s %d\n",__FILE__,__LINE__)


struct genCell
{
	void* info;
	struct genCell* next;
};

list cons(void* info,list node);
void removeL(list* al,list cell);

list match(list l,char* info);

#endif