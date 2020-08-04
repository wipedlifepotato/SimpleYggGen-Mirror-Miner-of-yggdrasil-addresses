/*https://gist.github.com/alan-mushi/c8a6f34d1df18574f643
 * Simple ncurses form example with fields that actually behaves like fields.
 *
 * How to run:
 *	gcc -Wall -Werror -g -pedantic -o test fields_magic.c -lform -lncurses
 */
#include "ncurses-interface.h"
#include <assert.h>
#include <ctype.h>
#include <form.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
extern OptionBox options;
static bool selected = false;
static FORM *form;
static FIELD *fields[5];
static WINDOW *win_body, *win_form;

/*
 * This is useful because ncurses fill fields blanks with spaces.
 */
static char *trim_whitespaces(char *str) {
  char *end;

  // trim leading space
  while (isspace(*str))
    str++;

  if (*str == 0) // all spaces?
    return str;

  // trim trailing space
  end = str + strnlen(str, 128) - 1;

  while (end > str && isspace(*end))
    end--;

  // write new null terminator
  *(end + 1) = '\0';

  return str;
}

static void driver(int ch) {
  int i;
  const char *searchtext;
  switch (ch) {
  case 10: /*enter*/
    // Or the current field buffer won't be sync with what is displayed
    form_driver(form, REQ_NEXT_FIELD);
    form_driver(form, REQ_PREV_FIELD);

    move(LINES - 3, 2);

    for (i = 0; fields[i]; i++) {
      printw("%s", trim_whitespaces(field_buffer(fields[i], 0)));

      if (field_opts(fields[i]) & O_ACTIVE)
        printw("\"\t");
      else
        printw(": \"");
    }

    refresh();
    pos_form_cursor(form);
    searchtext = field_buffer(fields[1], 0);
    memcpy(options.searchtext, searchtext, strlen(searchtext));
    options.countOfThreads = atoi(field_buffer(fields[1], 0));
    selected = true;
    //			printf("Count of threads: %d;
    //				searchtext:%s\n",options.countOfThreads,options.searchtext);

    break;

  case KEY_DOWN:
    form_driver(form, REQ_NEXT_FIELD);
    form_driver(form, REQ_END_LINE);
    break;

  case KEY_UP:
    form_driver(form, REQ_PREV_FIELD);
    form_driver(form, REQ_END_LINE);
    break;

  case KEY_LEFT:
    form_driver(form, REQ_PREV_CHAR);
    break;

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

  default:
    form_driver(form, ch);
    break;
  }

  wrefresh(win_form);
}

int start_field() {
  int ch;

  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);

  win_body = newwin(24, 80, 0, 0);
  assert(win_body != NULL);
  box(win_body, 0, 0);
  win_form = derwin(win_body, 20, 78, 3, 1);
  assert(win_form != NULL);
  box(win_form, 0, 0);
  mvwprintw(win_body, 1, 2,
            "Press F1 to quit and ENTER to start(three times, of control+C to "
            "abort)");

  fields[0] = new_field(1, 10, 0, 0, 0, 0);
  fields[1] = new_field(1, 40, 0, 15, 0, 0);
  fields[2] = new_field(1, 10, 2, 0, 0, 0);
  fields[3] = new_field(1, 40, 2, 15, 0, 0);
  fields[4] = NULL;
  assert(fields[0] != NULL && fields[1] != NULL && fields[2] != NULL &&
         fields[3] != NULL);

  set_field_buffer(fields[0], 0, "SearchText");
  set_field_buffer(fields[1], 0, "200");
  set_field_buffer(fields[2], 0, "Count of threads");
  set_field_buffer(fields[3], 0, "3");

  set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
  set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
  set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
  set_field_opts(fields[3], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

  set_field_back(fields[1], A_UNDERLINE);
  set_field_back(fields[3], A_UNDERLINE);

  form = new_form(fields);
  assert(form != NULL);
  set_form_win(form, win_form);
  set_form_sub(form, derwin(win_form, 18, 76, 1, 1));
  post_form(form);

  refresh();
  wrefresh(win_body);
  wrefresh(win_form);

  while ((ch = getch()) != KEY_F(1) && !selected)
    driver(ch);

  unpost_form(form);
  free_form(form);
  free_field(fields[0]);
  free_field(fields[1]);
  free_field(fields[2]);
  free_field(fields[3]);
  delwin(win_form);
  delwin(win_body);
  endwin();

  return 0;
}
