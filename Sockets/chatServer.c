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

  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
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
  printf("\n Listening for clients....");
  listen(socketfd, 5);
  struct sockaddr_in client_addr;
  socklen_t clientlen = sizeof(client_addr);
  int clientfd = accept(socketfd, (struct sockaddr * ) & client_addr, & clientlen);
  printf("\n Connection from client accepted..");
  char BUFFER[Buffer_size];
  while (1) {
    read(clientfd, BUFFER, Buffer_size);
    printf("\n Client: %s", BUFFER);

    bzero(BUFFER, Buffer_size);
    printf("\n Server : Type your response....");
    fgets(BUFFER, Buffer_size, stdin);
    if (strncmp(BUFFER, "exit",4) == 0) {
      break;
    }
    int wval = write(clientfd, BUFFER, Buffer_size);
    if (wval < 0) {
      perror("Write unsuccessful");
    }
  }
  printf("Client: BYE");
  close(socketfd);

  return 0;

}