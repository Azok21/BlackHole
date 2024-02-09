/* *********************************************************************************************************************************** */

#include "Main.h"

/* *********************************************************************************************************************************** */

// Creation of the Global variables, shared for all files (extern defined in Struct.h file)
SHIP ship1;
SHIP ship2;

SERVER serv_info;

/* *********************************************************************************************************************************** */

int main(int argc, char *argv[]){

  // Definition of the default value for server ip and port
  serv_info.ip = "172.24.192.114";
  serv_info.port = 2000;

  // Definition of the name by accepting paramters in the call of the exe. Normaly you can change it in teh settings menu but i got some problem with that.
  // When you open the settings menu for some dark reason the function connect() doesn't work anymore after making the game crash.
  if(argc==2){ ship1.name = argv[1]; }
  else{ ship1.name = "PLAYER 1"; }
  ship2.name = "PLAYER 2";

  // Init of ncurses and struct (ref. Init.c)
  Init();
  Init_Structs();
  
  // Refresh everything 
  refresh();

  // Send directly to the menu (ref. Menu.c)
  Menu();

  // End of the game. final return.
  return 0;
}

/* *********************************************************************************************************************************** */
