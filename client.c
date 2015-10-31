
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <netdb.h> 

//function Definitions
int timer();
int recieveFile(FILE*);
char* processHeader();
int sendACK(int);
int checkSum(char*);


static int hostPort = 7777;
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])//takes in "sender hostname(ip), sender port no, filename"
{
    int sockfd, recvfd; 
    int sendPort, n;
    struct sockaddr_in serv_addr, host_addr;
	FILE* requestedFile;
	
    

    char buffer[256];
    if (argc < 4) { //make sure enough args got passed.
       fprintf(stderr,"not enough args passed");
       exit(0);
    }

    sendPort = atoi(argv[2]); //get port from cmd line.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // new socket, tcp, ipv4
    if (sockfd < 0) 
        error("ERROR creating socket");

    bzero(&host_addr, sizeof host_addr);//set up host out port
    host_addr.sin_family=AF_INET;  
    host_addr.sin_port=htons(hostPort);
    inet_pton(AF_INET,"127.0.0.1",&(host_addr.sin_addr));

    n = bind(sockfd,(struct sockaddr *)&host_addr,sizeof(host_addr));
    if(n<0)
	error("ERROR binding Socket");
    
	bzero(&serv_addr,sizeof serv_addr);// set up serv address
    serv_addr.sin_family=AF_INET;  
	serv_addr.sin_port=htons(sendPort);
    inet_pton(AF_INET,argv[1],&(serv_addr.sin_addr));
	n = bind(recvfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(n<0)
	error("ERROR binding Socket");
	
    requestedFile = Fopen("%s_2",argv[3],"w");
    bzero(buffer,256);
	buffer = ("GET %s", argv[3])
    n = sendto(sockfd,buffer,strlen(buffer),(struct sockaddr *)&serv_addr,sizeof(serv_addr)); //write to the socket
	if (n<0) { error("error sending to socket");}
	n=recieveFile(requestedFile, recvfd, (struct sockaddr *)&serv_addr);
	if (n<0) { error("error recieving file");}
	
	
    fclose(requestedFile);
    close(sockfd); //close socket
    
    return 0;
}


/*
 * Process Header looks through the header supplied by the client and returns a stirng of the form
 * X/Y where x is the current part number and Y is the total.
 * 
 * */
char* processHeader(int recvFD, struct sockaddr* serv_addr){
	char* result, header[256], seq, X, Y;
	
	int i =0;
	while (strcmp(header, "\r\n")!=0){
		recvfrom (recvFD, header, 255, 0,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
		seq = strtok(header, " ,.-\r\n");
		if(strcmp(seq, "SEQ_NUMBER")==0){
			X = strtok(null, " ,.-\r\n");
		}
		else if(0==strcmp(seq, "SEQ_TOTAL")){
			Y = strtok(null, " ,.-\r\n");
		}
	}
	result = ("%s/%s", X,Y);
	return result;
	
}

/*
 * Recieve File recieves all of the file pieces from the sender 
 * 
* */
 
int recieveFile(File* theFile, int recvFD, struct sockaddr* serv_addr){

	char* xy, buffer[513];
	int total, current,check;
	
	xy = processHeader(recvFD, serv_addr);
	current = atoi(strtok(xy, "/"));
	total = atoi(strtok(null, "/"));
	while(current<=total){
		recvfrom (recvFD, buffer, 512, 0,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
		check = checkSUM(buffer);
		if(check==0){
			sendACK(current);
			fprintf(theFile, buffer);//write checked bytes to file
		}
		else{
			//do something if checksum fails? 
		}
		if(current!=total){
			xy = processHeader(recvFD, serv_addr);//read new header
			current = atoi(strtok(xy, "/"));
		}
		else{
			current++;
		}
	}
	
}
/*
 * Returns 0 if it checks out, -1 otherwise.
 * */
int checkSUM(char* bits){
	return 0;
}
/*
 * sends the ack for successfully recieved pieces.
 * */
int sendACK(int ackNum){
	return 0;
}