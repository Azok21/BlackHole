/* *********************************************************************************************************************************** */

#include "../../Utils/Shared/Libs.h"
#include "../../Utils/Shared/Structs.h"

#include "../../Utils/Funct/Display.h"
#include "../../Utils/Funct/Network.h"

/* *********************************************************************************************************************************** */

void game(bool multi){

  // Mulitplayer Part. Only created if MULTI variable is used (ref. to PreDefs.h)
  int socketFD;
  if(multi){

    // Creation of the socket
    socketFD = Network__CreateSocket();
    
    // Creation of the Server adress struct
    struct sockaddr_in *servAddr = Network__CreateServAddr(serv_info.ip,serv_info.port);

    // Test of connection
    if(!connect(socketFD,(struct sockaddr*)servAddr,sizeof(*servAddr)))
      printf("Connected successfully\n");

    // Start The thread to receive data and send basic Infos (name, starting pos and life)
    Network__Thread(socketFD);
    Network__SendName(socketFD);
    Network__SendInfos(socketFD);
  }

  // Init Diplay of game window. Only ncurses used not so interesting to describe more here
  erase();
  refresh();

  Display__Ship(ship1);
  Display__Ship(ship2);

  Display__Life(ship1);
  Display__Life(ship2);
  
  // Core of the game
  int ch1;
  while((ch1=getch())!=KEY_BACKSPACE){
    
    // Movement part : use Arrows. 
    if      ((ch1==KEY_LEFT  ) && ship1.x>1                    ) { ship1.x--; }
    else if ((ch1==KEY_RIGHT ) && ship1.x<COLS-ship1.dim_cols-1) { ship1.x++; }

    // Refresh Display
    Display__Ship(ship1);
    Display__Life(ship1);

    // Add some damage to show the successfull transmission of live data 
    if(ship1.x == 2 || ship1.x == COLS-ship1.dim_cols-2){ ship1.life.life--;}

    // If MULTI, send the new information to the other client
    if (multi) {
      Network__SendInfos(socketFD);
    }
  }

  // Cleaning process
  Network__SendEnd(socketFD);
  close(socketFD);
}

/* *********************************************************************************************************************************** */
