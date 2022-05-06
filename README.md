## File Crypto GTK

GTK+3 version of [jaq-the-cat/file-crypto](https://github.com/jaq-the-cat/file-crypto).
Lets you encrypt a file with a 32-byte key (literally just 32 bytes inside a file, no cool funny formats) or you can leave
out the key and let the program generate it for you in your Documents folder.

### Installation
First, clone the repository
`git clone https://github.com/jaq-the-cat/file-crypto-gtk.git`
Then cd into it
`cd file-crypto-gtk`
And install the program
`sudo make install`

### Running the program
To run the program, type in `file-crypto-gtk` into the CLI and it will open up for you. For running it through an App Finder, you will need to create a .desktop file, like this one:
```
[Desktop Entry]
Name=FileCrypto
Exec=/usr/bin/file-crypto-gtk
Icon=
Terminal=false
Type=Application
Categories=Cryptography;
```
