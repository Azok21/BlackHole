/* *********************************************************************************************************************************** */

#include "../Utils/Shared/Libs.h"
#include "../Utils/Shared/PreDefs.h"
#include "../Utils/Shared/Structs.h"

/* *********************************************************************************************************************************** */

// Init of color Pair. Ncurses use only.
void Init_Colors(){
        init_pair(WHITE  , COLOR_WHITE  , COLOR_WHITE  );   // (COLOR NAME , FG COLOR(text) , BG COLOR) : color name ref. PreDefs.h   
        init_pair(BLUE   , COLOR_BLUE   , COLOR_BLUE   );
        init_pair(CYAN   , COLOR_CYAN   , COLOR_CYAN   );
        init_pair(RED    , COLOR_RED    , COLOR_RED    );
        init_pair(YELLOW , COLOR_YELLOW , COLOR_YELLOW );
        init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(GREEN  , COLOR_GREEN  , COLOR_GREEN  );

        init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
        init_pair(RED_BLACK  , COLOR_RED  , COLOR_BLACK);
}

/* *********************************************************************************************************************************** */

// Init of the screan and different settings of ncurses. Ncurses use only
void Init(){
  initscr();            // Init a new screen in the terminal

  start_color();        // Start the color system in the terminal
  Init_Colors();        // Init the color pair we need 
  
  cbreak();
  noecho();             // Doesn't return the key pressed 

  curs_set(false);      // Hidde the cursor
  keypad(stdscr,true);  // Allow the use of the numpad and arrows
}

/* *********************************************************************************************************************************** */


void Init_Structs(){

  // Creation of the common game window
  ship1.win = ship2.win = newwin(LINES,COLS,0,0);
  
  // Definition of the ship size
  ship1.dim_rows = ship2.dim_rows = 6;
  ship1.dim_cols = ship2.dim_cols = 9;

  // Allocation and Definition of the ship shape
  ship1.shape = ship2.shape = calloc(ship1.dim_rows,sizeof(char*));
  for (int i=0;i<ship1.dim_rows;i++) {
    ship1.shape[i] = ship2.shape[i] = calloc(ship1.dim_cols,sizeof(char));
    switch (i) {
      case 0: ship1.shape[i] = ship2.shape[i] = "    @    "; break;
      case 1: ship1.shape[i] = ship2.shape[i] = "|  @@@  |"; break;
      case 2: ship1.shape[i] = ship2.shape[i] = "| @#@#@ |"; break;
      case 3: ship1.shape[i] = ship2.shape[i] = "|@@#@#@@|"; break; 
      case 4: ship1.shape[i] = ship2.shape[i] = "|  @@@  |"; break;
      case 5: ship1.shape[i] = ship2.shape[i] = "    #    "; break;
    }
  }

  // Definition of the ships colors
  ship1.color1 = BLUE;
  ship1.color2 = GREEN;
  ship2.color1 = RED;
  ship2.color2 = YELLOW;

  // Definition of the starting coordinate
  ship1.x = ship2.x = COLS/2-4;
  ship1.y = LINES-10;
  ship2.y = 4;

  // Definition of the ship pos on the screen
  ship1.pos = DOWN;
  ship2.pos = UP;

  // Definition of max bullet possible based on screen size and allocation of space accordingly
  ship1.maxBullet = ship2.maxBullet = 10;
  ship1.nbrBullet = ship2.nbrBullet = 0;
  ship1.BulletX = ship2.BulletX = calloc(ship1.maxBullet,sizeof(int));
  ship1.BulletY = ship2.BulletY = calloc(ship1.maxBullet,sizeof(int));
 

  // Definition of new life window
  ship1.life.win = newwin(3,52,LINES-3,COLS-53);
  ship2.life.win = newwin(3,52,0,1);

  // Definition of starting life points
  ship1.life.life = ship2.life.life = 50;

  // Definition of life color
  ship1.life.color = BLUE;
  ship2.life.color = RED;
}

/* *********************************************************************************************************************************** */
