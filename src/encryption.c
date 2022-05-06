#include "encryption.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

typedef unsigned char byte;

typedef struct {
  byte *fbytes;
  size_t length;
} file_wsize;

// 32*8 = 256bit

int aes_init_en(byte *key_data, int key_data_len, EVP_CIPHER_CTX *e_ctx) {
  int i, nrounds = 5;
  byte key[32], iv[32];

  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), NULL, key_data, key_data_len, nrounds, key, iv);
  if (i != 32) {
    fprintf(stderr, "! Key size is %d bits - should be 256 bits\n", i);
    return -1;
  }

  EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}

int aes_init_de(byte *key_data, int key_data_len, EVP_CIPHER_CTX *d_ctx) {
  int i, nrounds = 5;
  byte key[32], iv[32];

  i = EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), NULL, key_data, key_data_len, nrounds, key, iv);
  if (i != 32) {
    fprintf(stderr, "! Key size is %d bits - should be 256 bits\n", i);
    return -1;
  }

  EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  return 0;
}

/*
 * Encrypt *len bytes of data
 * All data going in & out is considered binary (byte[]).
 */
byte* aes_encrypt(EVP_CIPHER_CTX *e, byte *plaintext, int *len) {
  /* Max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes. */
  int c_len = *len + AES_BLOCK_SIZE, f_len = 0;
  byte *ciphertext = malloc(c_len);

  /* Allows reusing of 'e' for multiple encryption cycles. */
  EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);

  /* Update ciphertext, c_len is filled with the length of ciphertext generated,
   * len is the size of plaintext in bytes. */
  EVP_EncryptUpdate(e, ciphertext, &c_len, plaintext, *len);

  /* Update ciphertext with the final remaining bytes. */
  EVP_EncryptFinal_ex(e, ciphertext+c_len, &f_len);

  *len = c_len + f_len;
  return ciphertext;
}

/*
 * Decrypt *len bytes of ciphertext
 */
byte *aes_decrypt(EVP_CIPHER_CTX *e, byte *ciphertext, int *len) {
  /* Plaintext will always be equal to or lesser than length of ciphertext */
  int p_len = *len, f_len = 0;
  byte *plaintext = malloc(p_len);
  
  EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
  EVP_DecryptUpdate(e, plaintext, &p_len, ciphertext, *len);
  EVP_DecryptFinal_ex(e, plaintext+p_len, &f_len);

  *len = p_len + f_len;
  return plaintext;
}

int write_to_file(char* filename, byte* fbytes, int len) {
  FILE* file = fopen(filename, "wb");
  if (!file) {
    fprintf(stderr, "! Error opening file `%s`\n", filename);
    return -1;
  }

  fwrite(fbytes, 1, len, file);
  fclose(file);

  return 0;
}

int generate_key_file(char* filename, byte key[32]) {
  RAND_bytes(key, 32);

  write_to_file(filename, key, 32);
  return 0;
}

file_wsize get_file_contents(char* filename) {
  FILE* file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "! Error opening file `%s`\n", filename);
    return (file_wsize) { NULL, 0 };
  }
  struct stat sb;
  if (stat(filename, &sb) == -1) {
    fprintf(stderr, "! `stat` error\n");
    return (file_wsize) { NULL, 0 };
  }

  byte* fbytes = malloc(sb.st_size);
  fread(fbytes, sb.st_size, 1, file);
  fclose(file);
  return (file_wsize) {
    fbytes, sb.st_size
  };
}

int read_key(char* filename, byte key[32]) {
  file_wsize key_file = get_file_contents(filename);
  if (key_file.fbytes == NULL)
    return -1;
  if (key_file.length != 32) {
    fprintf(stderr, "! Key file `%s` invalid\n", filename);
    return -1;
  }

  memcpy(key, key_file.fbytes, 32);
  return 0;
}

int encrypt_file(char *filename, char *extension, char *key_filename) {
    // setup output filename
    char *out_filename = malloc(strlen(filename) + strlen(extension));
    strcpy(out_filename, filename);
    strcat(out_filename, extension);

    // get file contents to encrypt
    file_wsize fdata_en = get_file_contents(filename);
    if (fdata_en.fbytes == NULL) return -1;
    int len_en = fdata_en.length;

    // generate key
    byte key_data[32];
    int key_data_len = 32;
    if (read_key(key_filename, key_data) != 0) {
      const char* homedir = getenv("HOME");
      char* path_to_key = malloc(strlen(homedir) + strlen("/Documents/key.key"));

      strcpy(path_to_key, homedir); // $HOME
      strcat(path_to_key, "/Documents/key.key"); // $HOME/Documents/key.key

      printf("`%s` not found, generating `%s`...\n", key_filename, path_to_key);

      generate_key_file(path_to_key, key_data);
    };

    EVP_CIPHER_CTX *en = EVP_CIPHER_CTX_new();

    // initialize AES
    if (aes_init_en(key_data, key_data_len, en)) {
      fprintf(stderr, "! Couldn't initialize AES cipher\n");
      return -1;
    }

    byte *encrypted = aes_encrypt(en, fdata_en.fbytes, &len_en);
    free(fdata_en.fbytes);
    if (write_to_file(out_filename, encrypted, len_en) != 0) {
      free(encrypted);
      free(out_filename);
      return -1;
    };
    free(encrypted);
    free(out_filename);


    return 0;
}

int decrypt_file(char *filename, char *extension, char *key_filename) {
    // setup output filename
    char *out_filename = malloc(strlen(filename) + strlen(extension));
    strcpy(out_filename, filename);
    strcat(out_filename, extension);

    // get file contents to decrypt
    file_wsize fdata_de = get_file_contents(filename);
    if (fdata_de.fbytes == NULL) return -1;
    int len_de = fdata_de.length;

    // generate key
    byte key_data[32];
    int key_data_len = 32;
    if (read_key(key_filename, key_data) != 0) {
      return -1;
    };

    EVP_CIPHER_CTX *de = EVP_CIPHER_CTX_new();

    // initialize AES
    if (aes_init_de(key_data, key_data_len, de)) {
      fprintf(stderr, "! Couldn't initialize AES cipher\n");
      return -1;
    }

    byte *decrypted = aes_decrypt(de, fdata_de.fbytes, &len_de);
    free(fdata_de.fbytes);
    if (write_to_file(out_filename, decrypted, len_de) != 0) {
      free(decrypted);
      free(out_filename);
      return -1;
    };

    free(decrypted);
    free(out_filename);

    return 0;
}
