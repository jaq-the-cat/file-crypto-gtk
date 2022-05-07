## File Crypto GTK

GTK+3 version of [jaq-the-cat/file-crypto](https://github.com/jaq-the-cat/file-crypto).
Lets you encrypt a file with a 32-byte key (literally just 32 bytes inside a file, no cool funny formats) or you can leave
out the key and let the program generate it for you in your Documents folder.

### Dependencies
- the clang compiler
- GTK+3
- OpenSSL

### Install

First, clone the repository

`git clone https://github.com/jaq-the-cat/file-crypto-gtk.git`

Then cd into it

`cd file-crypto-gtk`

And install the program

`sudo make install`

### Uninstall

Simply run `sudo make uninstall` in the project folder. If you previously deleted it, clone the project again with `git clone https://github.com/jaq-the-cat/file-crypto-gtk.git`

### Running the program
To run the program, type in `file-crypto-gtk` into the CLI and it will open up for you.
You could also use it through an App Finder, where its name is FileCrypto

### How to Use
- AES256 (32 byte) Key

  Select the key file from the filesystem.

  On encryption, if it is left empty, a file named `key.key` will be generated in your Documents folder (`$HOME/Documents/key.key`) for later decryption.
  
  Must not be left empty when decrypting a file.

- File to encrypt/decrypt

  Select the file to encrypt/decrypt from the filesystem.

  Must not be left empty when encrypting or decrypting (what file would you even encrypt or decrypt?).

- Output extension

  Extension for the output file, leave blank to replace the selected file with the encrypted or decrypted version.

  For example, if you are encrypting a file named `$HOME/file.txt`, and the output extension is set to `.encrypted`, the program will generate an encrypted file `$HOME/file.txt.encrypted` and leave `$HOME/file.txt` unchanged.

  If you leave it blank, `$HOME/file.txt` will be replaced with an encrypted version of itself, and you will lose the unencrypted contents. **If you delete the key used for encryption, the file will be unrecoverable**.
