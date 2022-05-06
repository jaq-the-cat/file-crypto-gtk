#pragma once
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

int encrypt_file(char *filename, char *extension, char *key_filename);
int decrypt_file(char *filename, char *extension, char *key_filename);
