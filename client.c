
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <netdb.h> 

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd; 
    int outPort, n;
    struct sockaddr_in serv_addr, host_addr;
    

    char buffer[256];
    if (argc < 4) { //make sure enough args got passed.
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    outPort = atoi(argv[2]); //get port from cmd line.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // new socket, tcp, ipv4
    if (sockfd < 0) 
        error("ERROR creating socket");

    bzero(&host_addr,sizeof host_addr);//set up host out port
    host_addr.sin_family=AF_INET;  
    host_addr.sin_port=htons(outPort);
    inet_pton(AF_INET,argv[1],&(host_addr.sin_addr));

    n = bind(sockfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
    if(n<0)
	error("ERROR binding Socket");
    
	bzero(&serv_addr,sizeof serv_addr);// set up serv address
    serv_addr.sin_family=AF_INET;  
    inet_pton(AF_INET,argv[2],&(serv_addr.sin_addr));
	
    /*printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
	
    n = sendto(sockfd,buffer,strlen(buffer)); //write to the socket
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,100);
    n = read(sockfd,buffer,99); //read from the socket
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    */
    close(sockfd); //close socket
    
    return 0;
}
