INSTALL_PATH=/usr/local
VERSION=1_01

BIN=shascii
CC=gcc
CFLAGS=-c -Wall -pedantic -ansi
BIN_CURR=$(BIN)_$(VERSION)

all: $(BIN)

shascii_$(VERSION): $(BIN).o
	$(CC) $(BIN).o -o $(BIN)

shascii.o: shascii.c
	$(CC) $(CFLAGS) $(BIN).c -o $(BIN).o

clean: 
	rm -f *.o
	rm -rf $(BIN)
	rm -rf $(BIN_CURR)

install: $(BIN_CURR)
	@chmod +x install.sh
	@./install.sh $(VERSION) $(INSTALL_PATH) 0

force-install: $(BIN_CURR)
	@chmod +x install.sh
	@./install.sh $(VERSION) $(INSTALL_PATH) 1