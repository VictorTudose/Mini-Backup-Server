#ifndef MY_SH 
#define MY_SH 1

#include "list.h"
#include "helpers.h"

void serv_sock(struct sockaddr_in* target,int PORT);
void download(int sockfd,char* filename,list messages);
list upload(int sockfd,char* filename,list messages);
void list_files(int sockfd,list messages);

#endif