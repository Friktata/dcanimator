# Compiler
CC = gcc
CFLAGS = -Iinclude -std=c11 -g -Wall -Wextra
LDFLAGS = -lnotcurses -lnotcurses-core


TARGET = dcanimator
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
SHAREDIR = /usr/share/$(TARGET)

SRCS = $(wildcard src/*.c) dcanimator.c
OBJS = $(SRCS:.c=.o)
OBJS_ROOT = $(notdir $(OBJS))

all: $(TARGET)
	@rm -f $(OBJS_ROOT)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS_ROOT) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(notdir $@)

keep: $(TARGET)

install: $(TARGET)
	@echo "Preparing to install $(TARGET)..."
	@if [ "$$(id -u)" -eq 0 ]; then SUDO=""; else SUDO="sudo"; fi; \
	echo "Using: $$SUDO"; \
	$$SUDO install -d "$(BINDIR)"; \
	$$SUDO install -m 755 "$(TARGET)" "$(BINDIR)/$(TARGET)"; \
	echo "Installing shared data to $(SHAREDIR) (old copy will be removed if present)"; \
	$$SUDO rm -rf "$(SHAREDIR)"; \
	$$SUDO mkdir -p "$(SHAREDIR)"; \
	$$SUDO cp -r "$(CURDIR)/share/." "$(SHAREDIR)/"; \
	echo "Installation complete."

uninstall:
	@echo "Preparing to uninstall $(TARGET)..."
	@if [ "$$(id -u)" -eq 0 ]; then SUDO=""; else SUDO="sudo"; fi; \
	echo "Using: $$SUDO"; \
	$$SUDO rm -f "$(BINDIR)/$(TARGET)"; \
	$$SUDO rm -rf "$(SHAREDIR)"; \
	echo "Uninstall complete."

clean:
	rm -f $(TARGET) *.o

.PHONY: all keep clean install uninstall
