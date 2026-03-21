CC := cc
CFLAGS := -Wall -Wextra
LIBS := -lncurses

PROG_NAME := TWo
SRC_DIR := src
INSTALL_PATH := /usr/local/bin

# Find all .c files recursively
SRC := $(shell find $(SRC_DIR) -name "*.c" ! -name "create_stages.c")

# Check if create_stages.c exists
ifneq (,$(wildcard $(SRC_DIR)/create_stages.c))
    HAS_CREATE_STAGES := yes
    STAGE_CREATOR := create_stages
    STAGE_SRC := $(SRC_DIR)/create_stages.c
endif

all: $(PROG_NAME) $(if $(HAS_CREATE_STAGES),create-stages,)

# Main game executable
$(PROG_NAME): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Stage creation utility (only if file exists)
create-stages: $(STAGE_SRC)
	$(CC) $(CFLAGS) -DTEST_CREATE $^ -o $@

# Create stages directory and generate stage files
stages: create-stages
	mkdir -p stages
	./create-stages

install: $(PROG_NAME)
	sudo install -m755 $(PROG_NAME) $(INSTALL_PATH)

clean:
	rm -f $(PROG_NAME) create-stages
	rm -rf stages

uninstall:
	sudo rm -f $(INSTALL_PATH)/$(PROG_NAME)

.PHONY: all install uninstall clean stages create-stages
