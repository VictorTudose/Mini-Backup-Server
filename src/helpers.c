#include "helpers.h"

int send_file(int sockfd,char* filename)
{
	int fd=open(filename,O_RDONLY);

	printf("Sending file %s\n", filename);
	
	if(fd<0){
		char final[10];
		strncpy(final,"NO FILE",7);
		final[7]=0;
		printf("Sending %s\n", final);
		send(sockfd,final,7,0);
		printf("DID ret -1 at fileopen\n" );
		return -1;
	}

	char buf[BUFLEN];
	memset(buf,0,BUFLEN);
	int status=1;

	while(status)
	{
		memset(buf,0,BUFLEN);
		status=read(fd,buf,BUFLEN);

		if(status==-1){
			printf("DID ret -1\n");
			return -1;
		}

		if(strlen(buf)){
			printf("Sending %s\n",buf );
		}
		else
		{
			printf("End sending %s\n",buf);
			break;
		}
		send(sockfd, buf, status, 0);
	}

	close(fd);
	
	memset(buf,0,BUFLEN);
	buf[0]=';';
	printf("Sending %s\n",buf );
	send(sockfd, buf, 1, 0);
	
	printf("End for good %s\n",buf);
	
	return 1;

}

int recv_file(int sockfd,char* filename)
{
	int fd;
	char buf[BUFLEN];
	memset(buf,0,BUFLEN);
	int status=1;

	DIE((fd=open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");

	while(status )
	{
		status=recv( sockfd, buf, BUFLEN ,0 );
		
		if(strstr(buf,"NO FILE"))
		{
			printf("%d: %s\n",status,buf);
			return -1;
		}

		if(strstr(buf,"O FILE"))
		{
			printf("%d: %s\n",status,buf);
			return -1;
		}

		if(status==-1){
			printf("ERROR in recv_file\n");
			return -1;
		}
		
		if(status==0){
			break;
		}


		buf[status]=0;

		printf("%d: %s\n",status,buf);

		if(buf[status-1]==';')
		{
			buf[status-1]=0;
			write(fd,buf,status-1);
			break;
		}

		write(fd,buf,status);

	}
	
	printf("Done recv_file\n");

	close(fd);
	return 1;
}