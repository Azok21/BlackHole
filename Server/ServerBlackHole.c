/* *********************************************************************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

/* *********************************************************************************************************************************** */

// Definition of some variables to simplify understanding
#define TCP AF_INET
#define IPV4 SOCK_STREAM

/* *********************************************************************************************************************************** */

// Creation of a struct to simplify the automatisation of client addr creation and movements
struct ClientAddr{
    int ClientFD;
    int BoundFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

/* *********************************************************************************************************************************** */

// Global variables 
struct ClientAddr acceptedAddr[20];
int acceptedAddrCount = 0;

/* *********************************************************************************************************************************** */

// prototypes 
int Create__Socket();                                   // return a socket
struct sockaddr_in* Create__ServerAddr(char*,int);      // return a struct for the server adsress, filled in
struct ClientAddr*  Create__ClientAddr(int);            // return a struct for the client address, filled in

void OpenToClient(int);                                 // start the accpeting process and bound clients 2 by 2

void Transmission__Thread(struct ClientAddr*);          // Create a thread to receive the data from client and free the main thread for new accepting 
void Transmission__RecvSend(struct ClientAddr*);       // work on the data received and send it to the bounded client

/* *********************************************************************************************************************************** */

int main(){
  
  // Creation of the server socket and addr
  int serverSocketFD = Create__Socket();
  struct sockaddr_in *servAddr = Create__ServerAddr("",2000);

  // Test if the binding is done successfully
  if (!bind(serverSocketFD,(struct sockaddr*)servAddr,sizeof(*servAddr)))
    printf("socket binding ==> success\n");

  // Start to listen on port 2000 any ip that want to connect
  int listenResult = listen(serverSocketFD,10);

  // Start accpeting and binding clients 2 by 2
  OpenToClient(serverSocketFD);

  // Shutdown the socket once finished
  shutdown(serverSocketFD,SHUT_RDWR);

  return 0;
}

/* *********************************************************************************************************************************** */

int Create__Socket(){
  return socket(TCP,IPV4,0); 
}

/* *********************************************************************************************************************************** */

struct sockaddr_in* Create__ServerAddr(char *ip,int port){
  struct sockaddr_in *servAddr = malloc(sizeof(struct sockaddr_in));;

  // Setting the type of connection and the port that will be used
  servAddr->sin_family = TCP;
  servAddr->sin_port = htons(port);

  // Setting the ip that will be used (if ip="" ==> we listen for any ip possible else set the ip as given)
  if(strlen(ip) ==0)
    servAddr->sin_addr.s_addr = INADDR_ANY;
  else
    inet_aton(ip,&servAddr->sin_addr);

  return servAddr;
}

/* *********************************************************************************************************************************** */

struct ClientAddr * Create__ClientAddr(int serverSocketFD) {
  
  // Creating the different needed variables
  struct sockaddr_in  clientAddress ;
  
  int clientAddressSize = sizeof (struct sockaddr_in);
  int clientSocketFD = accept(serverSocketFD,(struct sockaddr*)&clientAddress,&clientAddressSize);
  
  struct ClientAddr* acceptedSocket = malloc(sizeof (struct ClientAddr));

  // Setting the struct variables (refer to ClientAddr struct prototype at the top oif this file)
  acceptedSocket->address = clientAddress;
  acceptedSocket->ClientFD = clientSocketFD;
  acceptedSocket->BoundFD = -10;
  acceptedSocket->acceptedSuccessfully = clientSocketFD>0;

  // Test if the acceptation was done successfully
  if(!acceptedSocket->acceptedSuccessfully)
    acceptedSocket->error = clientSocketFD;

  return acceptedSocket;
}

/* *********************************************************************************************************************************** */

void OpenToClient(int serverSocketFD) {

  while(true){
    // Get the client addr struct
    struct ClientAddr* clientSocket  = Create__ClientAddr(serverSocketFD);

    // Adding the new client to the list of connected client
    acceptedAddr[acceptedAddrCount] = *clientSocket;
    acceptedAddrCount++;

    // Bind two client together (last in with a free client in the connected client list)
    if (acceptedAddrCount<2) {
      printf("Waiting for second player\n");
    }
    else {
      int i; 
      for (i=0;i<acceptedAddrCount;i++) {
        // If their is a free client to bind with we share the client FD  of each client to the other one
        if(acceptedAddr[i].BoundFD==-10 && i!=acceptedAddrCount-1){
          acceptedAddr[i].BoundFD = acceptedAddr[acceptedAddrCount-1].ClientFD;
          acceptedAddr[acceptedAddrCount-1].BoundFD = acceptedAddr[i].ClientFD;

          printf("Two Player connected\n");
          
          // Then we start to Thread to receive and send back the two recieved data
          Transmission__Thread(&acceptedAddr[i]);
          Transmission__Thread(&acceptedAddr[acceptedAddrCount-1]);
        }
      }
    }
  }
}

/* *********************************************************************************************************************************** */

// Creation of the Thread Function
void Transmission__Thread(struct ClientAddr *cAddr) {
  pthread_t id;
  pthread_create(&id,NULL,(void*)Transmission__RecvSend,(void*)cAddr);
}

/* *********************************************************************************************************************************** */

// Threaded function
void Transmission__RecvSend(struct ClientAddr *cAddr){

  char *buffer = calloc(1024, sizeof(char));
  char *dupper;
  char *type,*token,*name;
  int posx,life;

  // Starting an infinite loop of reception
  while (true){
    ssize_t  amountReceived = recv(cAddr->ClientFD,buffer,1024,0);

    // Duplication of the received data for treatment
    dupper = strdup(buffer);

    // Separation of each received token in the data String based on the separators
    if(amountReceived>0){
      buffer[amountReceived] = 0;

      type = strsep(&dupper,":");
      if(strcmp(type,"name")==0){
        token = strsep(&dupper,";");
        name = token;
      }
      else if (strcmp(type,"infos")==0) {
        token = strsep(&dupper,";");
        posx = atoi(token);
        token = strsep(&dupper,".");
        life = atoi(token);
      }
      else if(strcmp(type,"end")==0){
        break;
      }

      // Printing some information on the server side. debugging purpose only.
      printf("message from %s ==> %d ; %d\n",name,posx,life);

      // Sending the initial buffer to the bound client by File Descriptor
      send(cAddr->BoundFD,buffer, strlen(buffer),0);
    }

    if(amountReceived==0)
      break;
  }

  // Cleaning Process
  free(buffer);
  close(cAddr->ClientFD);
}

/* *********************************************************************************************************************************** */

// Initialy to send to multiple client but not used yet
/*
void Transmission__Send(char *buffer,int socketFD) {

    for(int i = 0 ; i<acceptedAddrCount ; i++)
        if(acceptedAddr[i].acceptedSocketFD !=socketFD)
        {
          send(acceptedAddr[i].acceptedSocketFD,buffer, strlen(buffer),0);
          printf("sended : %s\n",buffer);
        }

}
*/
