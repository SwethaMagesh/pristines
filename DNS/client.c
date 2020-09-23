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
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  char BUFFER[Buffer_size];
  while (1) {
    bzero(BUFFER, Buffer_size);
    printf("Enter hostname (cannonical address), enter exit to stop\n ");
    fgets(BUFFER, Buffer_size, stdin);
int wval = sendto(socketfd, BUFFER, Buffer_size,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if (strncmp(BUFFER, "exit",4) == 0) {
      break;
    }
    
    
    if (wval < 0) {
      perror("Write unsuccessful");
    } 

      bzero(BUFFER, Buffer_size);
      int rval = recvfrom(socketfd, BUFFER, Buffer_size,0,NULL,NULL);
      if (rval < 0) {
        perror("Cannot read from server");
      } else if (strcmp(BUFFER,"")==0) {
	    printf("\n Try a more  popular hostname. This is not found in list");  
      }
	else {
        printf(" \nServer says IP as: %s", BUFFER);
      }
   if(strncmp(BUFFER,"exit",4)==0){
	   break;
   } 

  }
  printf("Server: BYE.. Nice speaking with you!\n");
  close(socketfd);

  return 0;

}
