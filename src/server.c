#include "helpers.h"
#include "server_help.h"
#include "list.h"

void usage()
{
	fprintf(stderr, "Usage: %s server_port\n", __FILE__);
	exit(0);
}

#define PORT (atoi(argv[1]))

int main(int argc, char *argv[])
{

	if (argc < 2) {
		usage();
	}

	int listenfd, newsockfd;
	char buffer[BUFLEN];

	struct sockaddr_in serv_addr = {0} ;
	serv_sock(& serv_addr,PORT);
	
	struct sockaddr_in cli_addr;

	int n, i, ret, j;
	socklen_t clilen;

	fd_set read_fds;
	fd_set tmp_fds;
	int fdmax;

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	FD_SET(STDIN_FILENO, &read_fds);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(listenfd < 0, "socket");

	ret = bind(listenfd, (SA *) &serv_addr, sizeof(SA));
	DIE(ret < 0, "bind");

	ret = listen(listenfd, MAX_CLIENTS);
	DIE(ret < 0, "listen");

	FD_SET(listenfd, &read_fds);
	fdmax = listenfd;

	list messages=NULL;

	while (1) {

		tmp_fds = read_fds; 
		ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		DIE(ret < 0, "select");

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				if(i==STDIN_FILENO)
				{
					read(STDIN_FILENO,buffer,BUFLEN-1);
					if (strncmp(buffer, "exit", 4) == 0) {

						strcpy(buffer,"exit");
						buffer[4]=0;

						for(j=4;j<=fdmax;j++)
						{

							if(i!=j){
								DIE( send(j,buffer,sizeof(buffer),0)<0,"sending exit message");
							}
						}

						close(listenfd);
						exit(0);
					}
					else
					{
						printf("The only command possible for server is exit\n");
					}

					continue;
				}
				if (i == listenfd) {
					
					clilen = sizeof(cli_addr);
					newsockfd = accept(listenfd, (SA *) &cli_addr, &clilen);
					DIE(newsockfd < 0, "accept");

					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fdmax) {
						fdmax = newsockfd;
					}

					printf("Noua conexiune de la %s, port %d, socket client %d\n",
							inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), newsockfd);
					sprintf(buffer,"%d",i);

					continue;

				}
					
				memset(buffer, 0, BUFLEN);
				n = recv(i, buffer, sizeof(buffer), 0);
				DIE(n < 0, "recv");

				if (n == 0) {

					printf("Socket-ul client %d a inchis conexiunea\n", i);
					close(i);	
					FD_CLR(i, &read_fds);

				} else {
					printf ("S-a primit de la clientul de pe socketul %d mesajul: %s\n", i, buffer);
					
					if(!strncmp(buffer,"UPLOAD",6)){
						printf("In UPLOAD\n");
						messages=upload(i, strchr(buffer,' ') + 1 ,messages);
					}

					if(!strncmp(buffer,"DOWNLOAD",6)){
						printf("In DOWNLOAD\n");
						download(i, strchr(buffer,' ') + 1 ,messages);
					}

					if(!strncmp(buffer,"LIST",4)){
						printf("In list\n");
						list_files(i, messages);
					}

				}
				
			}
		}
	}

	close(listenfd);

	return 0;
}
