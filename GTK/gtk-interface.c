#include <gtk/gtk.h>
 
static GtkBuilder *builder;
static GtkWidget *window;

// static GtkSwitch * RegExpSwitch, * HighHeadSwitch;
char PrefixPatern[321];
#define COMANDBUF 100
#define PROGRAMNAME "SimpleYggGen"
static gboolean regexp, highmode;
void on_window_main_destroy(void) { gtk_main_quit(); }

void on_start_clicked(void) {
  puts("Start search");
  puts(PrefixPatern);
  if (regexp)
    puts("regexp true");
  if (highmode)
    puts("highmode true");
  char command[sizeof(PrefixPatern) + COMANDBUF];
  sprintf(command, "./%s %s %s %s", PROGRAMNAME, 
          regexp ? "-r" : "",PrefixPatern, highmode ? "-H" : "");
#ifdef __linux__
	#include <unistd.h> 
	#include <sys/types.h> 
	#include <sys/wait.h>
pid_t pid=fork();
if(pid==0){

	static char *arguments[4];
	arguments[0]=(regexp ? "-r" : "");
	arguments[1]=PrefixPatern;
	arguments[2]=(highmode ? "-H" : "");
	arguments[3]=NULL;
	execv("./"PROGRAMNAME,arguments);//TODO: /usr/bin/PROGRAMNAME?
	exit(127);
}else if(pid == getpid()){
	waitpid(pid,0,0);
}

#else
  system(command);
#endif
}

void on_high_mode_activated(GtkSwitch *data, gboolean state) {
  puts("High Mode changed");
  highmode = state;
}

void on_regexp_mode_changed(GtkSwitch *data, gboolean state) {
  puts("RegExp mode changed");
  regexp = state;
}

void on_ipv6_pater_changed(GtkWidget *data, gpointer p) {
  puts("IPv6 patter changed");
  // GtkEntry * ipv6_label=GTK_ENTRY(gtk_builder_get_object(builder,
  // "Ipv6Text"));
  GtkWidget *entry = (GtkWidget *)data;
  const gchar *ipv6 = gtk_entry_get_text(GTK_ENTRY(entry));
  puts(ipv6);
  memcpy(PrefixPatern, ipv6, strlen(ipv6));
  PrefixPatern[strlen(ipv6)] = '\0';
}

int main(int argc, char *argv[]) {

  memset((void *)PrefixPatern, '\0', sizeof(PrefixPatern));
  // bzero(PrefixPatern, sizeof(PrefixPatern));
  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "GTK.glade", NULL);

  window = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
  // RegExpSwitch = GTK_SWITCH(gtk_builder_get_object(builder, "highswitch"));
  // HighHeadSwitch = GTK_SWITCH(gtk_builder_get_object(builder,
  // "regexpswitch"));
  gtk_window_set_title(GTK_WINDOW(window), "Sys-Yggdrasil GTK interface");
  gtk_builder_connect_signals(builder, NULL);
  GtkWidget *paned = GTK_WIDGET(gtk_builder_get_object(builder, "Paned"));

  g_object_unref(builder);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}
