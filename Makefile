CC = gcc
HEADERDIR = inc
CFLAGS = -I./$(HEADERDIR) -Wall -Wextra -Wno-sequence-point #-Werror 
DEBUGFLAGS = -ggdb3 -O0

ODIR = obj
SOURCEDIR = src
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

# Get list of object files, with paths
OBJECTS := $(subst $(SOURCEDIR),$(ODIR),$(SOURCES:%.c=%.o))
BINARY = clox

$(shell mkdir -p obj)

$(BINARY): $(OBJECTS)
	$(CC) $(DEBUGFLAGS) $(CFLAGS) $(OBJECTS) -o $(BINARY)

$(ODIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(DEBUGFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf obj
	@rm -f clox
