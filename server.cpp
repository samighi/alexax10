/* A simple server in the internet domain using TCP.
myServer.c
D. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Raspberry as follows:
   
    g++ -o myServer myServer.c
    ./myServer

The server waits for a connection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/stat.h>

void error( char *msg ) {
  perror(  msg );
  exit(1);
}

int func( int a ) {
   return 2 * a;
}

int doesFileExist(const char *filename) {
    struct stat st;
    int result = stat(filename, &st);
    return result == 0;
}

void sendData( int sockfd, int x ) {
  int n;

  char buffer[32];
  sprintf( buffer, "%d\n", x );
  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
    error( const_cast<char *>( "ERROR writing to socket") );
  buffer[n] = '\0';
}

void sendDataStr( int sockfd, char *buffer ) {
  int n;

  if ( (n = write( sockfd, buffer, strlen(buffer) ) ) < 0 )
    error( const_cast<char *>( "ERROR writing to socket") );
  buffer[n] = '\0';
  printf("Sent Data");
}


int getData( int sockfd ) {
  char buffer[32];
  int n;

  if ( (n = read(sockfd,buffer,31) ) < 0 )
    error( const_cast<char *>( "ERROR reading from socket") );
  buffer[n] = '\0';
  return atoi( buffer );
}

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno = 51717, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int data;
     FILE *fp;

    portno = atoi(argv[1]);

     printf( "using port #%d\n", portno );
   
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
         error( const_cast<char *>("ERROR opening socket") );
     bzero((char *) &serv_addr, sizeof(serv_addr));

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons( portno );
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
       error( const_cast<char *>( "ERROR on binding" ) );
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
 
     //--- infinite wait on a connection ---
     while ( 1 ) {
        printf( "waiting for new client...\n" );
        if ( ( newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen) ) < 0 )
            error( const_cast<char *>("ERROR on accept") );
        printf( "opened new communication with client\n" );
        while ( 1 ) {
             //---- wait for a number from client ---
           //  data = getData( newsockfd );
           //  printf( "got %d\n", data );
           //  if ( data < 0 )
           //     break;
               
           //  data = func( data );

             //--- send new data back ---
        //     printf( "sending back %d\n", data );
            // sendData( newsockfd, data );
            int rc = doesFileExist("/echo/alexa.cmd");
            if (rc > 0) {
                fp = fopen("/echo/alexa.cmd", "r"); 
            // strcpy(buffer,"a7 off");

            for (int i=0;i<255;i++) buffer[i]=0;
             printf("Reading file\n");
             size_t newLen = fread(buffer, sizeof(char),255 , fp);
             printf("Read file\n");
              if (newLen == 0) {
                   fputs("Error reading file", stderr);
                } else {
                 buffer[++newLen] = '\0'; /* Just to be safe. */
                  printf("Got from Alexa %s",buffer);
                }
             sendDataStr( newsockfd, buffer );
               system("rm /echo/alexa.cmd");
             }
           else {
             printf("Sleeping");
           sleep(1);
            }
        }
        close( newsockfd );

        //--- if -2 sent by client, we can quit ---
        if ( data == -2 )
          break;
     }
     return 0;
}


