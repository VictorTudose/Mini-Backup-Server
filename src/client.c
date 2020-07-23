#include "helpers.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, ret;
	struct sockaddr_in serv_addr={0};
	char buffer[BUFLEN];
	int fdmax;

	fd_set read_fds;
	fd_set tmp_fds;

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	if (argc < 3) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	ret = inet_aton(argv[1], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (SA*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	FD_SET(STDIN_FILENO, &read_fds);
	FD_SET(sockfd, &read_fds);
	fdmax = sockfd;

	while (1) {

		tmp_fds = read_fds; 
		
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");
		memset(buffer, 0, BUFLEN);

		if(FD_ISSET(sockfd,&tmp_fds)){
			recv(sockfd,buffer, BUFLEN - 1,0);
			printf("Received %s\n",buffer);
			if(strlen(buffer)==0){
				close(sockfd);
				exit(0);
			}
			if(strncmp(buffer,"exit",4)==0){
				close(sockfd);
				exit(0);
			}
		}
		else
		{
			read(STDIN_FILENO,buffer,BUFLEN-1);
			buffer[strlen(buffer)-1]=0;
			
			if (strncmp(buffer, "exit", 4) == 0) {
				break;
			}
			send(sockfd,buffer,BUFLEN-1,0);
		
			if(!strncmp(buffer,"UPLOAD",6))
			{
				send_file(sockfd,strchr(buffer,' ')+1);
			}
			if(!strncmp(buffer,"DOWNLOAD",6))
			{
				printf("Receving %s\n",strchr(buffer,' ')+1);
				char* final=calloc(BUFLEN,sizeof(char));
				sprintf(final,"cli%s",strchr(buffer,' ')+1);
				recv_file(sockfd,final);
			}

		}
	}

	close(sockfd);

	return 0;
}
