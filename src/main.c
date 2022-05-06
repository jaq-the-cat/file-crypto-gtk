#include <stdio.h>
#include <stdlib.h>
#include "gtkm.h"
#include <gtk-3.0/gtk/gtk.h>

WIDGET_S(window, "main_window");
WIDGET_S(encrypt_btn, "encrypt_btn");
WIDGET_S(decrypt_btn, "decrypt_btn");
GtkBuilder *builder;

int main(int argc, char* argv[]) {
  // initialize gtk
  gtk_init(&argc, &argv);
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "file-crypto-gui.glade", NULL);

  // initialize global widgets
  init_window(builder);
  init_encrypt_btn(builder);
  init_decrypt_btn(builder);

  // essential for GTK program
  gtk_builder_connect_signals(builder, NULL);
  g_object_unref(builder);

  // start program
  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

void exit_app() {
  gtk_main_quit();
}
