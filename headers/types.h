#pragma once

typedef unsigned char byte;

typedef struct {
  char ext[16];
  char selected_filename[1024];
  char key_filename[1024];
} global;
