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

static int mes_len = 100; //max message length to be allowed to echo.
static int lis_len =30; // time allowed to listen for communcation

int main(int argc, char *argv[])
{

     char str[mes_len];
     int sockfd, inPort, cli_fd, cli_len, n;
     struct sockaddr_in serv_addr, cli_addr;


     if (argc < 2) { //Error message if no port was passed, exit with program failure.
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0); //Create socket with ipv4 and tcp. 
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     iPort = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(inPort);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,//binds socket to inPort and an ip.
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd, lis_len); //listen for communication

     clilen = sizeof(cli_addr);
     cli_fd = accept(sockfd, (struct sockaddr *)  &cli_addr, &clilen);// accept communication

	if (cli_fd < 0) 
             error("ERROR on accept");
	while(true){
	     bzero(str, mes_len); //erase str, read in up to mes_len chars print it, and send it back
	     n = read(cli_fd, str, mes_len);
	     if (n < 0) error("ERROR reading from socket");
	     printf("Echo - %s",str);
	     n = write(cli_fd, str, strlen(str)+1);
	     if (n < 0) error("ERROR writing to socket");
	}//end while loop

    return 0;
}
