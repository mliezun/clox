CC = gcc
HEADERDIR = headers
CFLAGS = -I./$(HEADERDIR) -Wall

ODIR = obj
SOURCEDIR = src
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

# Get list of object files, with paths
OBJECTS := $(subst $(SOURCEDIR),$(ODIR),$(SOURCES:%.c=%.o))
BINARY = clox

$(shell mkdir -p obj)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BINARY)

$(ODIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf obj
	@rm -f clox
