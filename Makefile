SRC     := $(wildcard src/*.c)
PKGS    := gtk+-3.0
CC      := clang
CFLAGS  := -Iheaders -rdynamic -Wall -O3 -std=c17 
LDFLAGS := -lcrypto
CFLAGS  += $(shell pkg-config --cflags $(PKGS))
LDFLAGS += $(shell pkg-config --libs $(PKGS))

.PHONY: dev clean compile cnr

dev:
	echo $(CFLAGS) | tr " " "\n" > compile_flags.txt
	echo $(LDFLAGS) | tr " " "\n" >> compile_flags.txt

clean:
	$(RM) *.o a.out

compile: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o a.out

install: $(SRC)
	sudo $(CC) -D__PRODUCTION=0 $(CFLAGS) $(LDFLAGS) $^ -o /usr/bin/file-crypto-gtk # compile program and add binary to system
	sudo mkdir /usr/share/file-crypto-gtk # create resources folder
	sudo cp res/file-crypto-gui.glade /usr/share/file-crypto-gtk/file-crypto-gui.glade # add UI resources (.glade)
	sudo cp res/file-crypto-gtk.desktop /usr/share/applications/file-crypto-gtk.desktop # add .desktop entry

uninstall: $(SRC)
	sudo rm /usr/bin/file-crypto-gtk # remove binary
	sudo rm -rf /usr/share/file-crypto-gtk # remove UI resources (.glade)
	sudo rm /usr/share/applications/file-crypto-gtk.desktop # remove .destop entry
