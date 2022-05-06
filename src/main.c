#include <stdio.h>
#include <stdlib.h>
#include "gtkm.h"
#include <gtk-3.0/gtk/gtk.h>

#ifdef __PRODUCTION
#define GLADE_LOCATION "/var/lib/file-crypto-gtk/file-crypto-gui.glade"
#else
#define GLADE_LOCATION "file-crypto-gui.glade"
#endif

WIDGET_S(window, "main_window");
WIDGET_S(encrypt_btn, "encrypt_btn");
WIDGET_S(decrypt_btn, "decrypt_btn");
WIDGET_S(success_dialog, "success_dialog");
WIDGET_S(success_msg, "success_msg");
WIDGET_S(failure_dialog, "failure_dialog");
WIDGET_S(failure_msg, "failure_msg");

GtkBuilder *builder;

int main(int argc, char* argv[]) {
  // initialize gtk
  gtk_init(&argc, &argv);
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, GLADE_LOCATION, NULL);

  // initialize global widgets
  init_window(builder);
  init_encrypt_btn(builder);
  init_decrypt_btn(builder);
  init_success_dialog(builder);
  init_success_msg(builder);
  init_failure_dialog(builder);
  init_failure_msg(builder);

  // essential for GTK program
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);

  // start program
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

void close_success() {
  gtk_widget_hide(success_dialog);
}

void close_failure() {
  gtk_widget_hide(failure_dialog);
}

void exit_app() {
  gtk_main_quit();
}
