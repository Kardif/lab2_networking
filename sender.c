/* server for basic rdt 3.0
Writer: Josh Brown and Jared Headings
Cse 3461 networking

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>

int sendFile(FILE*, int, int, struct sockaddr_in, socklen_t);
void sendHeader(int, int, int, struct sockaddr_in, socklen_t);
int waitforACK();

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])//takes in port number to run on. 
{

    int sockfd, port, n, size;
    struct stat st;
    struct sockaddr_in serv_addr, recv_addr;
	socklen_t addrsize, recvsize;
    FILE* theFile;
    char buffer[256];


    if (argc < 2) { //Error message if no port was passed, exit with program failure.
         fprintf(stderr,"ERROR, no port provided\n");
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); //Create socket with ipv4 and tcp.
    if (sockfd < 0){
        error((char*)"ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    port = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
	addrsize = sizeof(serv_addr);
    
    //binds socket to inPort and an ip.
    if (bind(sockfd, (struct sockaddr*) &serv_addr, addrsize) < 0){
              error((char*)"ERROR on binding");
    }
    
	//set up receive address
	bzero((char *) &serv_addr, sizeof(serv_addr));
    port = atoi(argv[1]+1);
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = INADDR_ANY;
    recv_addr.sin_port = htons(port);
    recvsize = sizeof(recv_addr);
	bzero(buffer, 256);
    recvfrom (sockfd, buffer, 255, 0,(struct sockaddr*) &recv_addr, &recvsize);
    //recieve filename
	printf("opening the file\n");
	printf("filename is %s\n", buffer);
    theFile = fopen(buffer, "rb");
    //open file in binary mode
    stat(buffer, &st);
    size = st.st_size;
	 printf("size = %d\n", size);
    if(size%512==0){
        size = size/512;
    }
    else{
        size = (size/512)+1;
    }
    //calculate total number of 512 byte parts
	printf("calling sendfile\n");
    n = sendFile(theFile, size, sockfd, recv_addr, recvsize);
    printf("file sent!");
	
	
	//close file
    fclose(theFile);
    printf("file closed!");
    return 0;
}

int sendFile(FILE* theFile, int total, int sockdesc, struct sockaddr_in serv_addr, socklen_t addrsize){
	 int n,i=0;
	 n=0;
	 char buffer[513];//128 4 4byte chars = 512 bytes
	printf("before the while loop in sendfile\n");
	 while(i<=total){
		sendHeader(i, total, sockdesc, serv_addr, addrsize);
		bzero(buffer, 513);
		fread(buffer, 4, 128, theFile);
		
		n = sendto(sockdesc,buffer,strlen(buffer), 0, (struct sockaddr*) &serv_addr, addrsize);
        printf("%s\n",buffer);
		
		
		waitforACK();
		i++;
	 }
	printf("after the while loop in sendfile\n");
    return 1;
}
void sendHeader(int x, int total, int sockdesc, struct sockaddr_in serv_addr, socklen_t addrsize){
	char buffer[128];
	//sleep(1);
	int n=0;
	bzero(buffer,128);
	sprintf(buffer, "SEQ_NUM %d \r\n", x);
	n = sendto(sockdesc,buffer,strlen(buffer),0,(struct sockaddr*) &serv_addr, addrsize);
	printf("n = %d\n",n);
	bzero(buffer,128);
	sprintf(buffer, "SEQ_TOTAL %d \r\n", total);
	n = sendto(sockdesc,buffer,strlen(buffer),0,(struct sockaddr*) &serv_addr, addrsize);
	printf("n = %d\n",n);
	bzero(buffer,128);
	sprintf(buffer, "\r\n");
	n = sendto(sockdesc,buffer,strlen(buffer),0,(struct sockaddr*) &serv_addr, addrsize);
	printf("n = %d\n",n);
	//sleep(1);
	
}
int waitforACK(){
	int i =0;
	while(i<1000){

	i++;

	}	
	return 0;
}

