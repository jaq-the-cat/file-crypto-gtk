#include <gtk-3.0/gtk/gtk.h>
#include <stdbool.h>
#include "types.h"
#include "gtkm.h"
#include "encryption.h"

extern GtkWidget *encrypt_btn;
extern GtkWidget *decrypt_btn;
extern global g;

void make_sensitive_if_cool() {
  if (g.selected_filename[0] != 0) { // if file selected...
    // let user use encrypt button
    gtk_widget_set_sensitive(encrypt_btn, true);
    if (g.key_filename[0] != 0) { // if file and key selected...
      // let user use decrypt button
      gtk_widget_set_sensitive(decrypt_btn, true);
    }
  }
}

void key_selected(GtkFileChooser *self, gpointer user_data) {
  // set the global key filename to be equal to the filename selected by the user
  strcpy(g.key_filename, gtk_file_chooser_get_filename(self));
  make_sensitive_if_cool();
}

void file_selected(GtkFileChooser *self, gpointer user_data) {
  // set global filename to encrypt/decrypt to be equal to the filename selected by the user
  strcpy(g.selected_filename, gtk_file_chooser_get_filename(self));
  make_sensitive_if_cool();
}

void ext_changed(GtkEntry *self, gpointer user_data) {
  // update encryption extension for use with encryption
  GtkEntryBuffer *buf = gtk_entry_get_buffer(self);
  char* text = (char*) gtk_entry_buffer_get_text(buf);

  strcpy(g.ext, text);
}

void encrypt(GtkButton *_) {
  if (encrypt_file(g.selected_filename, g.ext, g.key_filename) == 0) {
    printf("SUCCESS!\n");
  };
}

void decrypt(GtkButton *_) {
  if (decrypt_file(g.selected_filename, g.ext, g.key_filename) == 0) {
    printf("SUCCESS!\n");
  };
}
