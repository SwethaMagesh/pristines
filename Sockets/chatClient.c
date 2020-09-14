#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define Buffer_size 200
int main(){
	
	int socketfd=socket(AF_INET,SOCK_STREAM,0);
	if(socketfd<0){
 	perror("\n Socket error");
 	exit(1);
	}
 struct sockaddr_in server_addr;
 server_addr.sin_family=AF_INET;
 server_addr.sin_port=9999;
 server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
 int connectval=connect(socketfd,(struct sockaddr*)&server_addr,sizeof(server_addr));  
 if(connectval<0){
  perror("\n Connection error");
  exit(1);
 }
 else{
	printf("\n Connected with SERVER .. Press exit to stop chatting ");
 }
 char BUFFER[Buffer_size];
 while(1){
	bzero(BUFFER,Buffer_size);
	fgets(BUFFER,Buffer_size,stdin);
	if(strcmp(BUFFER,"exit")==0){
		break;
	}
	int wval=write(socketfd,BUFFER,Buffer_size);
	if(wval<0){
		perror("Write unsuccessful");
	}
	else{
	bzero(BUFFER,Buffer_size);
	int rval=read(socketfd,BUFFER,Buffer_size);
	if(rval<0){
		perror("Cannot read from server");
	}
	else{
		printf("\n Server: %s",BUFFER);
	}
	}


 }
 printf("Server: BYE");
 close(socketfd);



 return 0;


}
