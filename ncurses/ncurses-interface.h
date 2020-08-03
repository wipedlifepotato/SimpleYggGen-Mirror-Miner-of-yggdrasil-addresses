#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
#include <form.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define ITEMSELECTEDTEXT "Item selected is :" 
#define ELIF else if
#define MAXBUF 1024

static const char ENTRY_HIGHMODE[]="Start HighHeadMode";
static const char ENTRY_REGEXPMODE[]="Start RegExpMode";
static const char ENTRY_SEARCHBYTEXTMODE[]="Start SearchByText";
static const char  ENTRY_EXIT[]="Exit";


static bool stopprogram=false;
static const char *choices[] = {
                        ENTRY_HIGHMODE, ENTRY_REGEXPMODE, ENTRY_SEARCHBYTEXTMODE,
                        ENTRY_EXIT,
                        (char *)NULL,
                  };
static void on_enter(const char * entry);
void start_menu(void);

static void on_highmode_selected(void);
static void on_regexpmode_selected(void);
static void on_searchbytext_selected(void);
