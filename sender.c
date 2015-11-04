/* server for basic rdt 3.0
Writer: Josh Brown and Jared Headings
Cse 3461 networking

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>



int sendFile(FILE*, int, int, struct);
int sendHeader(int, int, int, struct);


int main(int argc, char *argv[])//takes in port number to run on. 
{

     int sockfd, port, n, size, i;
	 struct stat st;
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
	 File = fopen(buffer, "rb");
	 //open file in binary mode
	 stat(filename, &st);
	 size = st.st_size;
	 
	 if(size%512==0){
		 size = size/512;
	 }
	 else{
		 size = (size/512)+1;
	 }
	 //calculate total number of 512 byte parts
	
	 n = sendFile(theFile, size, sockfd, serv_addr);
	
	
	//close file
    return 0;
}

int sendFile(FILE* theFile, int total, int sockdesc, struct sockaddr_in serv_addr){
	 i=0;
	 char buffer(128);//128 4 4byte chars = 512 bytes
	 while(i<total){
		sendheader(i, total, sockdesc, serv_addr);
		fread(buffer, 4, 128, theFile);
		n = sendto(sockdesc,buffer,strlen(buffer),(struct sockaddr *)&serv_addr,sizeof(serv_addr));
		i++;
	 }
}
int sendHeader(int x, int total, int sockdesc, struct sockaddr_in serv_addr){
	char* buffer;
	buffer = ("SEQ_NUM %d /r/n", x);
	n = sendto(sockdesc,buffer,strlen(buffer),(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	buffer = ("SEQ_TOTAL %d /r/n", total);
	n = sendto(sockdesc,buffer,strlen(buffer),(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	buffer = ("/r/n");
	n = sendto(sockdesc,buffer,strlen(buffer),(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	
	
}