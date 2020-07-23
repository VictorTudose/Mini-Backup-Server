#include "server_help.h"

void serv_sock(struct sockaddr_in* target,int PORT)
{
	target->sin_family = AF_INET;
	target->sin_port = htons(PORT);
	target->sin_addr.s_addr = INADDR_ANY;
}

void download(int sockfd,char* filename,list messages)
{

	char* final=calloc(BUFLEN,sizeof(char));
	sprintf(final,"cpy%s",filename);

	if(!match(messages,filename))
	{
		strncpy(final,"NO FILE",7);
		final[7]=0;
		send(sockfd,final,7,0);
		return;
	}

	if(send_file(sockfd,final)<0)
	{
		printf("Unsuccessful download\n");
		return;
	}
	printf("Successful download\n");
}

list upload(int sockfd,char* filename,list messages)
{

	char* final=calloc(BUFLEN,sizeof(char));
	sprintf(final,"cpy%s",filename);
	if(recv_file(sockfd,final)<0)
		return messages;

	char* buf=calloc(BUFLEN,sizeof(char));
	memset(buf,0,BUFLEN);
	strcpy(buf,filename);

	return cons(buf,messages);

}
void list_files(int sockfd,list messages)
{
	for(list l=messages;l;l=l->next)
	{
		printf("Sending %s\n", (char*)(l->info));
		DIE( send(sockfd, (char*)(l->info) ,strlen( (char*)(l->info) ),0 ) <0 , "send");
	}
}