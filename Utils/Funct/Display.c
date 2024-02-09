/* *********************************************************************************************************************************** */

#include "../Shared/Libs.h"
#include "../Shared/Structs.h"
#include "../Shared/PreDefs.h"

/* *********************************************************************************************************************************** */

// Display function of ships based on their screen position. Ncurses purpose only.
void Display__Ship(SHIP ship){
  for (int i=0;i<ship.dim_rows;i++) {
    
    // Move on each rows of the ship shape and clean everything on the line before doing anything
    wmove(ship.win,ship.y+i,0);
    wclrtoeol(ship.win);

    int row;

    // Set the orientation of the ship based on screen pos
    if (ship.pos == DOWN) row = i;
    else row = ship.dim_rows-1-i;
    
    // Print the right color based on the character in the ship shape
    for (int j=0;j<ship.dim_cols;j++) {
      char c = ship.shape[row][j];

      if (c=='@') {
        wattron(ship.win,COLOR_PAIR(ship.color1));
        mvwprintw(ship.win,ship.y+i,ship.x+j,"@");
        wattroff(ship.win,COLOR_PAIR(ship.color1));
      } 
      else if (c=='#') {
        wattron(ship.win,COLOR_PAIR(ship.color2));
        mvwprintw(ship.win,ship.y+i,ship.x+j,"#");
        wattroff(ship.win,COLOR_PAIR(ship.color2));
      } 
      else if (c=='|') {
        wattron(ship.win,COLOR_PAIR(WHITE));
        mvwprintw(ship.win,ship.y+i,ship.x+j,"|");
        wattroff(ship.win,COLOR_PAIR(WHITE));
      }
    }
  }

  // Refresh to display the ships
  wrefresh(ship.win);
}

/* *********************************************************************************************************************************** */

// Display function of the life based on the player screen pos
void Display__Life(SHIP ship){

  // Clean all the windows before doing anything else
  werase(ship.life.win);
  box(ship.life.win,0,0);

  // Print the life in right to left direction if on top of the screen
  if(ship.pos){
    mvwprintw(ship.life.win,2,2," %s ",ship.name);

    wattron(ship.life.win,COLOR_PAIR(ship.life.color));
    for (int i=1;i<=ship.life.life;i++) {
      mvwprintw(ship.life.win,1,i,"O");
    }
    wattroff(ship.life.win,COLOR_PAIR(ship.life.color));
  } 

  // Print it from left to right if on the bottom of the screen
  else {
    mvwprintw(ship.life.win,0,50-strlen(ship.name)-2," %s ",ship.name);
    
    wattron(ship.life.win,COLOR_PAIR(ship.life.color));
    for (int i=50;i>50-ship.life.life;i--) {
      mvwprintw(ship.life.win,1,i,"O");
    }
    wattroff(ship.life.win,COLOR_PAIR(ship.life.color));
  }

  // refresh the window
  wrefresh(ship.life.win);
}

/* *********************************************************************************************************************************** */

// not functionnal yet got some problem with the display doesn't work properly ^^'
void Display__Bullet(SHIP ship){
  
  if(ship.nbrBullet == 0){
    ship.BulletX[0] = COLS/2-4;
    ship.BulletY[0] = LINES-10;

    //ship.nbrBullet++;
  }
  else if(ship.BulletX[ship.nbrBullet-1] == ship.y-3 && ship.nbrBullet<ship.maxBullet){
    ship.BulletX[ship.nbrBullet] = ship.x+4;
    ship.BulletY[ship.nbrBullet] = ship.y-1;

    ship.nbrBullet+=1;
  }
  
  if (ship.nbrBullet==0){
    //for (int i=0;i<ship.nbrBullet;i++) {
      move(ship.BulletX[0],ship.BulletY[0]);
      clrtoeol();

      printf("%d\n",ship.nbrBullet);

      mvwprintw(ship.win,ship.BulletY[0],ship.BulletX[0],"o");
      ship.BulletY[0]++;
    //}
  }
  wrefresh(ship.win);
}

/* *********************************************************************************************************************************** */

// Display function of the menu. Ncurses purpose only.
void Display__Menu(WINDOW* win_menu,MENU* menu){

  // Erase everything before anything else
  erase(); 
  refresh(); 

  // Reprint the box and repost(reprint) the menu
  box(win_menu,0,0);

  post_menu(menu);

  // Print Title with colors
  char tittle[][100] = {"          SSSSSS  SS      SSSSSSS  SSSSSSS  SS   SS      SS   SS  SSSSSSS  SS       SSSSSSS         ",
                        "         SS   SS  SS      SS   SS  SS       SS  SS       SS   SS  SS   SS  SS       SS              ",
                        "BBBBBBB  SSSSSSS  SS      SSSSSSS  SS       SSSSS        SSSSSSS  SS   SS  SS       SSSSS    BBBBBBB",
                        "         SS   SS  SS      SS   SS  SS       SS  SS       SS   SS  SS   SS  SS       SS              ",
                        "         SSSSSS   SSSSSSS SS   SS  SSSSSSS  SS   SS      SS   SS  SSSSSSS  SSSSSSS  SSSSSSS         "};

  for (int i=0;i<5;i++) {
    for (int j=0;j<100;j++) {

      char c = tittle[i][j];
      
      if (c=='S') {
        wattron(win_menu,COLOR_PAIR(MAGENTA));
        mvwprintw(win_menu,3+i,(COLS/2)-50+j,"S");
        wattroff(win_menu,COLOR_PAIR(MAGENTA));
      }
      
      else if(c=='B'){
        wattron(win_menu,COLOR_PAIR(GREEN));
        mvwprintw(win_menu,3+i,(COLS/2)-50+j,"B");
        wattroff(win_menu,COLOR_PAIR(GREEN));
      }
    }
  }

  // Print subTitle
  wattron(win_menu,COLOR_PAIR(GREEN_BLACK));
  mvwprintw(win_menu,9,(COLS/2)-18,"BATTLE FOR THE STARS , AND BEYOND ...");
  wattroff(win_menu,COLOR_PAIR(GREEN_BLACK));

  // Refresh the window
  wrefresh(win_menu);
}

/* *********************************************************************************************************************************** */

// Display function of settings. Ncurses purpose only.
void Display__Settings(WINDOW *win_settings,WINDOW *win_form,FORM *form){

  // Erase everything before anything else
  erase(); 
  refresh();

  // Repost the form and print back the box
  post_form(form);
  
  box(win_form,0,0);

  // Print decoration with colors
  char *wrench[]    = {"    WWWWWW  WWWWWW    ",
                       "  WWWW  WW  WW  WWWW  ",
                       "WWWW    WW  WW    WWWW",
                       "WW      WWWWWW      WW",
                       "WW        WW        WW",
                       "WW                  WW",
                       "WWWW               WWW",
                       "  WWWW          WWWW  ",
                       "    WW          WW    ",
                       "    WW  WWWWWW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WW  WW  WW    ",
                       "    WW  WWWWWW  WW    ",
                       "    WW          WW    ",
                       "  WWWW          WWWW  ",
                       "WWWW              WWWW",
                       "WW                  WW",
                       "WW        WW        WW",
                       "WW      WWWWWW      WW",
                       "WWWW    WW  WW    WWWW",
                       "  WWWW  WW  WW  WWWW  ",
                       "    WWWWWW  WWWWWW    "};


  for (int i=0;i<28;i++) {
    for (int j=0;j<22;j++) {
      char c = wrench[i][j];
      if(c=='W'){
        wattron(win_settings,COLOR_PAIR(WHITE));
        mvwprintw(win_settings,2+i,5+j,"W");
        wattroff(win_settings,COLOR_PAIR(WHITE));
      }
    }
  }

  // Print control information
  wattron(win_settings,COLOR_PAIR(RED_BLACK));
  mvwprintw(win_settings,31,33,"Press ENTER to validate field /// Press F2 to save modifications");
  wattroff(win_settings,COLOR_PAIR(RED_BLACK));

  // Refresh the windows
  refresh();
  wrefresh(win_settings);
  wrefresh(win_form);
}

