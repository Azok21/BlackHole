#ifndef HEADER_FILE_NETWORK
#define HEADER_FILE_NETWORK 

int Network__CreateSocket();

struct sockaddr_in* Network__CreateServAddr(char*,int);

void Network__Thread(int);
void Network__Recv(int);
void Network__SendName(int);
void Network__SendInfos(int);
void Network__SendEnd(int);

#endif
