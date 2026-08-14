CC := cc
CFLAGS := -Wall -Wextra
LIBS := -lncurses -lsqlite3

PROG_NAME := TWo
SRC_DIR := src
INSTALL_PATH := /usr/local/bin

# Find all .c files recursively
SRC := $(shell find $(SRC_DIR) -name "*.c")

all: $(PROG_NAME)

# Main game executable
$(PROG_NAME): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

install: $(PROG_NAME)
	sudo install -m755 $(PROG_NAME) $(INSTALL_PATH)

clean:
	rm -f $(PROG_NAME)

uninstall:
	sudo rm -f $(INSTALL_PATH)/$(PROG_NAME)

.PHONY: all install uninstall clean stages create-stages
