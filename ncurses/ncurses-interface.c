#include "ncurses-interface.h"
extern int start_field(void);

OptionBox options;

static void on_enter(const char *entry) {
  move(20, 0);
  clrtoeol();
  attron(COLOR_PAIR(2));

  if (strstr(entry, ENTRY_EXIT)) {
    stopprogram = true;
    mvprintw(LINES - 1, 0, "BYE, BYE, BYE", entry);
    exit(0);
  }
  ELIF(strstr(entry, ENTRY_HIGHMODE)) {
    mvprintw(LINES - 1, 0, "Enter to highhead mode?(enter/control+C)");
    options.engine = HighHead;
  }
  ELIF(strstr(entry, ENTRY_REGEXPMODE)) {
    mvprintw(LINES - 1, 0, "Enter to RegExp mode");
    options.engine = RegExp;
    //	 on_regexpmode_selected();
  }
  ELIF(strstr(entry, ENTRY_SEARCHBYTEXTMODE)) {
    mvprintw(LINES - 1, 0, "Enter to Search by text mode");
    options.engine = SearchByText;
  }

  // mvprintw(LINES - 1,0 , ITEMSELECTEDTEXT" %s", entry);
  attroff(COLOR_PAIR(2));
  refresh();
  if (options.engine != HighHead)
    start_field();
  stopprogram = true;
}

void start_menu(void) {
  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  int n_choices, i;

  /* Initialize curses */
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_YELLOW, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);

  /* Create items */
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for (i = 0; i < n_choices; ++i) {
    my_items[i] = new_item(choices[i], choices[i]);
    set_item_userptr(my_items[i], (void (*)(char *))on_enter);
  }
  /* Crate menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Set menu option not to show the description */
  menu_opts_off(my_menu, O_SHOWDESC);

  /* Create the window to be associated with the menu */
  my_menu_win = newwin(10, 70, 4, 4);
  keypad(my_menu_win, TRUE);

  /* Set main window and sub window */
  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 68, 3, 1));
  // set_menu_format(my_menu, 5, 3);
  set_menu_mark(my_menu, " * ");

  /* Print a border around the main window and print a title */
  box(my_menu_win, 1, 0);
  box(my_menu_win, -1, 0);

  attron(COLOR_PAIR(1));
  mvprintw(LINES - 3, 0, "Use PageUp and PageDown to scroll");
  mvprintw(LINES - 2, 0, "Use Arrow Keys to navigate (F1 to Exit)");
  attroff(COLOR_PAIR(1));
  refresh();

  /* Post the menu */
  post_menu(my_menu);
  wrefresh(my_menu_win);

  while ((c = wgetch(my_menu_win)) != KEY_F(1) && !stopprogram) {
    switch (c) {
    case KEY_DOWN:
      menu_driver(my_menu, REQ_DOWN_ITEM);
      break;
    case KEY_UP:
      menu_driver(my_menu, REQ_UP_ITEM);
      break;
    case KEY_LEFT:
      menu_driver(my_menu, REQ_LEFT_ITEM);
      break;
    case KEY_RIGHT:
      menu_driver(my_menu, REQ_RIGHT_ITEM);
      break;
    case KEY_NPAGE:
      menu_driver(my_menu, REQ_SCR_DPAGE);
      break;
    case KEY_PPAGE:
      menu_driver(my_menu, REQ_SCR_UPAGE);
      break;
    case 10: /* Enter */
    {
      ITEM *cur;
      void (*p)(char *);

      cur = current_item(my_menu);
      p = item_userptr((const ITEM *)cur);
      p((char *)item_name(cur));
      pos_menu_cursor(my_menu);
      break;
    }
    }
    wrefresh(my_menu_win);
  }

  /* Unpost and free all the memory taken up */
  unpost_menu(my_menu);
  free_menu(my_menu);
  for (i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  endwin();
}

OptionBox getOption(void) {
  start_menu();
  return options;
}
