/* Mini lab socket server
Writer: Josh Brown
Cse 3461 networking
extra(non class provided) sources "https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/"
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<string.h>

int sendFile();
int sendHeader();


int main(int argc, char *argv[])//takes in port number to run on. 
{

     int sockfd, port, n;
     struct sockaddr_in serv_addr, cli_addr;
	 FILE* theFile;
	 char* buffer[256];


     if (argc < 2) { //Error message if no port was passed, exit with program failure.
         fprintf(stderr,"ERROR, no port provided\n");
     }

     sockfd = socket(AF_INET, SOCK_DGRAM, 0); //Create socket with ipv4 and tcp. 
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     port = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,//binds socket to inPort and an ip.
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
	recvfrom (sockFD, buffer, 255, 0,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
     //recieve filename
	 //open file in binary mode
	 //calculate total number of 512 byte parts
	 //loop
	 //send header
	 //send part
	 //close
	
	

    return 0;
}
