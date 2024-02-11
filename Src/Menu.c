/* *********************************************************************************************************************************** */

#include "../Utils/Shared/Libs.h"
#include "../Utils/Shared/PreDefs.h"
#include "../Utils/Shared/Structs.h"

#include "../Utils/Funct/Display.h"

#include "Settings.h"
#include "Game.h"

/* *********************************************************************************************************************************** */

int Menu__Driver(int ch,MENU *menu,ITEM **items){
  
  switch (ch) {

    // Move between items according with the arrow key
    case KEY_UP:   menu_driver(menu,REQ_UP_ITEM); break;
    case KEY_DOWN: menu_driver(menu,REQ_DOWN_ITEM); break;

    // if ENTER pressed start the selected menu
    case 10: 
      const char *name = item_name(current_item(menu));
      
      if      (strcmp(name,"   SOLO_AI   ") == 0) { game(SOLO); }  // solo game versus AI
      else if (strcmp(name," MULTIPLAYER ") == 0) { game(MULTI);}  // multiplayer game connecting to server
      else if (strcmp(name,"   SETTING   ") == 0) { Settings(); }  // settings page to set new server ip and port or new name 

      break;
  }

  return 0;
}

/* *********************************************************************************************************************************** */

int Menu(){

  // Create the windows needed and ncurses variable
  WINDOW *win_menu,*inner;

  MENU *menu;
  ITEM **items;

  // Set a preset item num and names
  int nbr_select = 4;
  char select[][14] = {"   SOLO_AI   "," MULTIPLAYER "," TOURNAMENTS ","   SETTING   "};

  // Defining the windows (inner is a derivated win of win_menu, also called a child win)
  win_menu = newwin(LINES,COLS,0,0);
  inner = derwin(win_menu,4,13,(LINES/6)*5,(COLS/2)-6);

  // Define the new items of the menu
  items = calloc(nbr_select,sizeof(ITEM *));
  for (int i=0;i<nbr_select;i++) {
    items[i] = new_item(select[i],"");
  }

  // Create the menu
  menu = new_menu(items);
  
  // Set all windows correctly
  set_menu_win(menu,win_menu); // set the main win used
  set_menu_sub(menu,inner);    // set the subwin of where the menu will really take place
  set_menu_mark(menu,NULL);
  set_menu_format(menu,5,1);   // set the menu format: 5 lines, 1 cols.

  // Display the menu (ref. Display.c)
  Display__Menu(win_menu,menu);

  // Core of the menu
  int ch;
  while ((ch=getch())!=KEY_BACKSPACE) { 
    Menu__Driver(ch,menu,items);            // Go look if the action is in the driver
    Display__Menu(win_menu,menu);           // Refresh Display
    wrefresh(win_menu);                     // Refresh the window
  }

  // Cleaning process
  unpost_menu(menu);
  free_menu(menu);
  
  for (int i =0;i<4;i++)
    free_item(items[i]);
  free(items);
 
  delwin(inner);
  delwin(win_menu);






  
  endwin();

  return 1;
}

/* *********************************************************************************************************************************** */
