/* *********************************************************************************************************************************** */

#include "../Shared/Libs.h"
#include "../Shared/Structs.h"
#include "../Shared/PreDefs.h"

#include "../Funct/Display.h"

/* *********************************************************************************************************************************** */

// return a socket
int Network__CreateSocket(){
  return socket(TCP,IPV4,0); 
}

/* *********************************************************************************************************************************** */

// return a struct for the server adsress, filled in
struct sockaddr_in* Network__CreateServAddr(char *ip,int port){
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

// Threaded function
void Network__Recv(int socketFD){

  char *buffer = calloc(1024, sizeof(char));
  char *type,*token;

  // Starting an infinite loop of reception
  while (true){
    ssize_t  amountReceived = recv(socketFD,buffer,1024,0);

    // Starting an infinite loop of reception
    if(amountReceived>0){
      buffer[amountReceived] = 0;

      type = strsep(&buffer,":");
      if(strcmp(type,"name")==0){
        token = strsep(&buffer,";");
        ship2.name = token;
      }
      else if (strcmp(type,"infos")==0) {
        token = strsep(&buffer,";");
        ship2.x = atoi(token);
        token = strsep(&buffer,".");
        ship2.life.life = atoi(token);

        Display__Ship(ship2);
        Display__Life(ship2);
      }
      else if(strcmp(type,"end")==0){
        break;
      }
    }

    if(amountReceived==0)
      break;
  }
  
  close(socketFD);
}

/* *********************************************************************************************************************************** */

// Creation of the Thread Function
void Network__Thread(int Fd){
  pthread_t id;
  pthread_create(&id,NULL,(void*)Network__Recv,(void*)(intptr_t)Fd);
}

/* *********************************************************************************************************************************** */

// Network function that send the preseted name
void Network__SendName(int Fd){
  char buffer[1024];
 
  sprintf(buffer, "%s:%s;","name",ship1.name);

  send(Fd,buffer,strlen(buffer),0);
}

/* *********************************************************************************************************************************** */

// Network function that send the current infos of the client
void Network__SendInfos(int Fd){
  char buffer[1024];
 
  sprintf(buffer, "%s:%d;%d.","infos",ship1.x,ship1.life.life);

  send(Fd,buffer,strlen(buffer),0);
}

/* *********************************************************************************************************************************** */

// Network function that send a end signal to the server
void Network__SendEnd(int Fd){
  char buffer[1024];

  sprintf(buffer, "%s:","end");

  send(Fd,buffer,strlen(buffer),0);
}

