#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define BUF_SIZE 512

int main(int argc, char *argv[]){

	if(argc < 2){
		printf("SYNTAX: %s [portno]", argv[0]);
		exit(1);
	}

	int sockfd, clientfd, portno, retval;
	struct sockaddr_in server_addr, client_addr;
	socklen_t clientlen;
	char buffer[BUF_SIZE];
	portno = atoi(argv[1]);//lib

// 1) Socket function
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Socket creation Error");
		exit(1);
	}
	bzero((char *)&server_addr, sizeof(server_addr)); // fills with zero

	// set the values in structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Structure inside sturcture (any local address)
	server_addr.sin_port = htons(portno);

// 2) Bind function
	if(bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
		perror("Binding Error");
		exit(1);
	}

// 3) Listen function
	listen(sockfd, 5);

// 4) Accept function
	clientlen = sizeof(client_addr);
	clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &clientlen);
	if(clientfd < 0){
		perror("Accepting Error");
		exit(1);
	}
	else{
		printf("\nConnected to the client: %d\n", clientfd);
	}


	while(1){
		bzero(buffer, BUF_SIZE);
		retval = read(clientfd, buffer, BUF_SIZE);

		if(retval < 0){
			perror("Reading Error");
			exit(1);
		}
		printf("Client: %sServer: ", buffer);
		bzero(buffer, BUF_SIZE);
		fgets(buffer, BUF_SIZE, stdin); // takes input only of the given size or less

		retval = write(clientfd, buffer, strlen(buffer));
		if(retval < 0){
			perror("Writing Error");
			exit(1);
		}

		if(strncmp(buffer, "exit", 4) == 0)
			break;
	}

	close(sockfd);
	close(clientfd);
	return 0;
}
