/* *********************************************************************************************************************************** */

#include "../Utils/Shared/Libs.h"
#include "../Utils/Shared/Structs.h"

#include "../Utils/Funct/Display.h"

/* *********************************************************************************************************************************** */

char* trim_whitespaces(char *str)
{
	char *end;

	// trim leading space
	while(isspace(*str))
		str++;

	if(*str == 0) // all spaces?
		return str;

	// trim trailing space
	end = str + strnlen(str, 128) - 1;

	while(end > str && isspace(*end))
		end--;

	// write new null terminator
	*(end+1) = '\0';

	return str;
}

/* *********************************************************************************************************************************** */

int driver(int ch,FORM *form,FIELD **fields){

	switch (ch) {	

    // Move the cursor down on the next field (back to first if at the end)
		case KEY_DOWN:
			form_driver(form, REQ_NEXT_FIELD);
			form_driver(form, REQ_END_LINE);
			break;

    // Move the cursor up on the previous field (back to last if at the start)
		case KEY_UP:
			form_driver(form, REQ_PREV_FIELD);
			form_driver(form, REQ_END_LINE);
			break;

    // Move the cursor left in the curent field (go to next field if at the end) 
		case KEY_LEFT:
			form_driver(form, REQ_PREV_CHAR);
			break;

    // Move the cursor right in the current field (go to previous field if at the start)
		case KEY_RIGHT:
			form_driver(form, REQ_NEXT_CHAR);
			break;

		// Delete the char before cursor
		case KEY_BACKSPACE:
		case 127:
			form_driver(form, REQ_DEL_PREV);
			break;

		// Delete the char under the cursor
		case KEY_DC:
			form_driver(form, REQ_DEL_CHAR);
			break;

    // Take new field_buffer value and save them in apropriate struct part (ships and serv_info)
    case KEY_F(2):
      char *ip   = trim_whitespaces(field_buffer(fields[1],0));
      int port   = atoi(trim_whitespaces(field_buffer(fields[3],0)));
      char *name = trim_whitespaces(field_buffer(fields[5],0));
      
      move(LINES-6,2);
      clrtoeol();
      move(LINES-5,2);
      clrtoeol();
      move(LINES-4,2);
      clrtoeol();

      if(strcmp(ip,serv_info.ip) != 0){
        serv_info.ip = ip;
        mvprintw(LINES-6,2,"Setting saved ==> IP : %s",serv_info.ip);
      }
       
      if(port != serv_info.port){
        serv_info.port = port;
        mvprintw(LINES-5,2,"Setting saved ==> PORT : %d",serv_info.port);
      }

      if(strcmp(name,ship1.name) != 0){
        ship1.name = trim_whitespaces(field_buffer(fields[5],0));
        mvprintw(LINES-4,2,"Setting saved ==> NAME : %s",ship1.name);
      }

      pos_form_cursor(form);
      break;
    
    // Go to the next field (go back to first if at end). for simplification of modification process only.
    // (a field is not considered modified if the cursor is still in, use enter to simply move out of the field)
    case 10:
      form_driver(form,REQ_NEXT_FIELD);
      form_driver(form,REQ_END_LINE);
      break;

    // For all other key pressed simply write them in the current field.
		default:
			form_driver(form, ch);
			break;
	}	
}

/* *********************************************************************************************************************************** */

int Settings() {

  // Set the cursor visible. for display purpose only.
  curs_set(true);

  // Declaration of all needed variables.
  WINDOW *win_settings,*win_form,*inner; 

  FORM *form;
  FIELD **fields;

  int nbr_select = 6; 
  
  char port_str[50]; 
  sprintf(port_str, "%d",serv_info.port);

  
  // Creation of needed windows. Display is as follow (...: no box ; ---: box ; DDD: some decorations ; FFF: some fields).
  //  
  //  win_settings
  //  +......................+
  //  .         win_form     .
  //  .   DD    +-----------+.
  //  .  DDDD   |           |.
  //  .  DDDD   | FF        |.
  //  .  DDDD   | FF        |.
  //  .  DDDD   |           |.
  //  .   DD    +-----------+.
  //  +......................+
  //
  win_settings = newwin(33,100,LINES/2-16,COLS/2-50);
  win_form = derwin(win_settings,30,68,1,31);

  // Creation of all the fields; new_field(nbr_rows, nbr_cols, posx, posy, nbr_off-screen_rows, nbr_additional_buffer)
  fields = calloc(nbr_select, sizeof(FIELD*));    
  fields[0]=new_field(1,10,2,6,0,0);
  fields[1]=new_field(1,40,2,21,0,0);
  fields[2]=new_field(1,10,4,6,0,0);
  fields[3]=new_field(1,40,4,21,0,0);
  fields[4]=new_field(1,10,8,6,0,0);
  fields[5]=new_field(1,40,8,21,0,0);
  fields[nbr_select] = NULL;

  // Setting fields options |==> O_VISIBLE : make field visible.
  //                        |==> O_PUBLIC  : make field data public. 
  //                        |==> O_AUTOSKIP: skip to the next field when this one is filled.
  //                        |==> O_EDIT    : allow edition.
  //                        |==> O_ACTIVE  : can be accessed by navigation key.
  //
  set_field_opts(fields[0],O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
  set_field_opts(fields[1],O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
  set_field_opts(fields[2],O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
  set_field_opts(fields[3],O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
  set_field_opts(fields[4],O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
  set_field_opts(fields[5],O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

  // Put underline under editable fields. display purpose only.
  set_field_back(fields[1],A_UNDERLINE);
  set_field_back(fields[3],A_UNDERLINE);
  set_field_back(fields[5],A_UNDERLINE);

  // Set the fields buffer (data if editable or name if not)
  set_field_buffer(fields[0],0,"HOST IP   : ");
  set_field_buffer(fields[1],0,serv_info.ip);
  set_field_buffer(fields[2],0,"HOST PORT : ");
  set_field_buffer(fields[3],0,port_str);
  set_field_buffer(fields[4],0,"PLAY NAME :");
  set_field_buffer(fields[5],0,ship1.name);
  
  // Create the form with the created fields.
  form = new_form(fields);
  
  // Distribution of the different windows based on the display example
  set_form_win(form,win_settings);
  set_form_sub(form,win_form);

  // General diplay function of settings window and the form (ref in Display.c)
  Display__Settings(win_settings,win_form,form);
  
  // Working core of the settings, call to the driver to make actions and refresh.
  int ch;
  while ((ch=getch())!=27) {
    driver(ch,form,fields);
    wrefresh(win_form);
  }

  // cleaning process
  unpost_form(form);
  free_form(form);

  for (int i=0;i<=nbr_select;i++) {
    free_field(fields[i]);
  }
  free(fields);

  delwin(win_settings);
  delwin(win_form);

  // Set the cursor back to hidden. display purpose only.
  curs_set(false);

  endwin();
  return 0;
}
