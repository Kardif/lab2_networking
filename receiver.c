/*
 * client for basic rdt 3.0
 * Written by: Josh Brown and Jared Headings
 * Cse 3461 Networking
 * 
 * */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h> 
#include <stddef.h>
#include <arpa/inet.h>
#include <unistd.h>

//function Definitions
int timer();
int receiveFile(FILE*, int, struct sockaddr_in*);
char* processHeader(int, struct sockaddr_in);
int sendACK(int);


void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])//takes in "sender hostname(ip), sender port no, filename"
{
    int sockfd; 
    int servPort, n;
    struct sockaddr_in serv_addr, send_addr;
	socklen_t addrsize, sendsize;
	FILE* requestedFile;
    char *filename = argv[3];
    

    char buffer[256];
    if (argc < 4) { //make sure enough args got passed.
       fprintf(stderr,"not enough args passed");
       exit(0);
    }

    servPort = atoi(argv[2]); //get port from cmd line.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // new socket, tcp, ipv4
    if (sockfd < 0) 
        error((char*)"ERROR creating socket");

    
	bzero(&serv_addr,sizeof serv_addr);// set up serv address
    serv_addr.sin_family=AF_INET;  
	serv_addr.sin_port=htons(servPort);
    inet_pton(AF_INET,argv[1],&(serv_addr.sin_addr));
	bzero(&send_addr,sizeof send_addr);// set up send address
    send_addr.sin_family=AF_INET;  
	send_addr.sin_port=htons(servPort+1);
    inet_pton(AF_INET,argv[1],&(send_addr.sin_addr));
	n = bind(sockfd, (struct sockaddr*)&send_addr, sizeof(send_addr));
    if(n<0)
	error((char*)"ERROR binding Socket");
    
	strcat(filename, "2");
    requestedFile = fopen(filename,"w");
    bzero(buffer,256);
	strcpy(buffer, ("GET %s", argv[3]));
   	
	addrsize = sizeof(serv_addr);
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, addrsize); //write to the socket
	if (n<0) { error((char*)"error sending to socket");}
	n=receiveFile(requestedFile, sockfd, &serv_addr);
	if (n<0) { error((char*)"error recieving file");}
	
	
    fclose(requestedFile);
    close(sockfd); //close socket
    
    return 0;
}


/*
 * Process Header looks through the header supplied by the client and returns a stirng of the form
 * X/Y where x is the current part number and Y is the total.
 * 
 * */
char* processHeader(int sendFD, struct sockaddr_in serv_addr){
	char* result, header[256], seq, X, Y;
	socklen_t addrsize;
	addrsize = sizeof(serv_addr);
	while (strcmp(header, "\r\n")!=0){
		recvfrom (sendFD, header, 255, 0,(struct sockaddr*)&serv_addr, &addrsize);
		seq = *strtok(header, " ,.-\r\n");
		if(strcmp(&seq, "SEQ_NUMBER")==0){
			X = *strtok(NULL, " ,.-\r\n");
		}
		else if(0==strcmp(&seq, "SEQ_TOTAL")){
			Y = *strtok(NULL, " ,.-\r\n");
		}
	}
	result = ("%s/%s", &X, &Y);
	return result;
	
}

/*
 * Recieve File recieves all of the file pieces from the sender 
 * 
* */
 
int receiveFile(FILE* theFile, int recvFD, struct sockaddr_in* serv_addr){

	char* xy, buffer[128];
	int total, current;
	
	xy = processHeader(recvFD, *serv_addr);
	current = atoi(strtok(xy, "/"));
	total = atoi(strtok(NULL, "/"));
	socklen_t addrsize;
	addrsize = sizeof(serv_addr);
	while(current<=total){
		recvfrom (recvFD, buffer, 128, 0,(struct sockaddr*)&serv_addr, &addrsize);
		
		sendACK(current);
		fprintf(theFile, "%s", buffer);//write bytes to file
		
		if(current!=total){
			xy = processHeader(recvFD, *serv_addr);//read new header
			current = atoi(strtok(xy, "/"));
		}
		else{
			current++;
		}
    }
    return 1;
}

/*
 * sends the ack for successfully recieved pieces.
 * */
int sendACK(int ackNum){
	return 0;
}
