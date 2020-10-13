CC = gcc
HEADERDIR = inc
CFLAGS = -I./$(HEADERDIR) -Wall -Wextra -Wno-sequence-point #-Werror 

ODIR = obj
SOURCEDIR = src
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')

# Get list of object files, with paths
OBJECTS := $(subst $(SOURCEDIR),$(ODIR),$(SOURCES:%.c=%.o))
BINARY = clox

$(shell mkdir -p obj)

all: CFLAGS += -O3 -DNAN_BOXING
all: $(BINARY)

debug: CFLAGS += -ggdb3 -O0 -DDEBUG_PRINT_CODE -DDEBUG_TRACE_EXECUTION -DDEBUG_STRESS_GC -DDEBUG_LOG_GC
debug: $(BINARY)

profile: CFLAGS += -g -O3
profile: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BINARY)

$(ODIR)/%.o: $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf obj
	@rm -f clox
