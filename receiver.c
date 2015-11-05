/*
 * client for basic rdt 3.0
 * Written by: Josh Brown and Jared Headings
 * Cse 3461 Networking
 * 
 *
 */

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
void processHeader(int, struct sockaddr_in*, char*);
int sendACK(int);


void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])//takes in "sender hostname(ip), sender port no, filename"
{
    int sockfd, servPort, n;
    struct sockaddr_in serv_addr, send_addr;
	socklen_t addrsize;
	FILE* requestedFile;
    char *filename = argv[3];
    
    if (argc < 4) { //make sure enough args got passed.
       fprintf(stderr,"not enough args passed");
       exit(0);
    }

    servPort = atoi(argv[2]); //get port from cmd line.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // new socket, tcp, ipv4
    printf("%d\n",sockfd);
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
    
    if (n<0){
        error((char*)"ERROR binding Socket");
    }
    
	
   // bzero(buffer,256);
	//strcpy(buffer, ("GET %s", argv[3]));
   	
	addrsize = sizeof(serv_addr);
    n = sendto(sockfd, filename, strlen(filename), 0, (struct sockaddr*)&serv_addr, addrsize); //write to the socket
	if (n<0) { error((char*)"error sending to socket");}

	strcat(filename, "2");
    requestedFile = fopen(filename,"w");	

	n=receiveFile(requestedFile, sockfd, &serv_addr);
	if (n<0) { error((char*)"error recieving file");}
	
    fclose(requestedFile);
    close(sockfd); //close socket
    
    printf("exiting...");
    return 1;
}


/*
 * Process Header looks through the header supplied by the client and returns a stirng of the form
 * X/Y where x is the current part number and Y is the total.
 * 
 * */
void processHeader(int sendFD, struct sockaddr_in* serv_addr, char* result){
	
	char* seq;
	int X;
	int Y;
	int i = 0;
	char header[256];
	socklen_t addrsize;
        bzero(header, 256);
    
    
    printf("about to recieve file\n");
	addrsize = sizeof(serv_addr);
    recvfrom (sendFD, header, 255, 0,(struct sockaddr*)serv_addr, &addrsize);  //problem line
    printf("recieved file\n");
		
   	printf("Entering process header while-loop.\n");
    	printf("%s\n",header);
	while (strncmp(header,"\r\n", 2) !=0&& i<20){
        printf("%s\n",header);
        
		if(strlen(header)>=2){
		seq = strtok(header," ,.-\r\n");
		//printf("seq = %s\n", seq);
		if(strncmp(seq, "SEQ_NUM",7)==0){
			X = atoi(strtok(NULL, " ,.-\r\n"));
			printf("x = %d\n", X);
		}
		else if(0==strncmp(seq, "SEQ_TOTAL",9)){
			Y = atoi(strtok(NULL, " ,.-\r\n"));
			printf("y = %d\n", Y);
		}
		}
		recvfrom (sendFD, header, 255, 0,(struct sockaddr*)serv_addr, &addrsize);
		i++;
	}
	sprintf(result, "%d/%d", X, Y);
	
    printf("%s", result);
	//return result;
	
}

/*
 * Recieve File recieves all of the file pieces from the sender 
 * 
* */
 
int receiveFile(FILE* theFile, int recvFD, struct sockaddr_in* serv_addr){

	char xy[256], buffer[128];
	int total, current;
	int i = 0;
	
    printf("Entered process header function.\n");
	processHeader(recvFD, serv_addr, xy);
    printf("Exited process header function.\n");
    
	current = atoi(strtok(xy, "/"));
	total = atoi(strtok(NULL, "/"));
	socklen_t addrsize;
	addrsize = sizeof(serv_addr);
	while(current<=total&& i<20){
		recvfrom (recvFD, buffer, 128, 0,(struct sockaddr*)&serv_addr, &addrsize);
		
		sendACK(current);
		fprintf(theFile, "%s", buffer);//write bytes to file
		
		if(current!=total){
			processHeader(recvFD, serv_addr,xy);//read new header
			
			current = atoi(strtok(xy, "/"));
			
			printf("current = %d\n", current);
		}
		else{
			current++;
		}
        printf("Current: %d  Total: %d\n",current,total);
	i++;
    }
    return 1;
}

/*
 * sends the ack for successfully recieved pieces.
 * */
int sendACK(int ackNum){
	return 0;
}
