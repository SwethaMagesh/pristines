#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <string.h>

#include <arpa/inet.h>

#define Buffer_size 200
int main() {

  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketfd < 0) {
    perror("\n Socket error");
    exit(1);
  }
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = 9999;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int bval = bind(socketfd, (struct sockaddr * ) & server_addr, sizeof(server_addr));
  if (bval < 0) {
    perror("\nBinding error");
    exit(1);
  }
  printf("\n Bind successful");
  struct sockaddr_in client_addr;
  socklen_t clientlen = sizeof(client_addr);
  char BUFFER[Buffer_size];
   printf("\n Trying to receive from client ..");

    while (1) {
	     bzero(BUFFER, Buffer_size);

	    int rval;
 rval=recvfrom(socketfd,BUFFER, Buffer_size,0, (struct sockaddr * ) & client_addr, & clientlen);
    if(strncmp(BUFFER,"exit",4)==0){
	    break;
    }
			
    printf("Client wants to know the IP for  %s", BUFFER);
    char request[100],response[100];
    request="";
    response="";
    puts(request);
    puts(response);

     FILE* fp=fopen("dnsMap","r");
    while(!feof(fp)){
	char temp[100];
	fgets(fp,temp);
	if(strcmp(temp,request)==0){
		fgets(fp,response);
	        printf("%s",response);	
	}


    } 
     int wval = sendto(socketfd, response, strlen(response),0,(struct sockaddr*)&client_addr,sizeof(client_addr));
        
    if (wval < 0) {
      perror("Write unsuccessful");
    }
  }
  printf("Client: BYE\n");
  close(socketfd);

  return 0;

}
