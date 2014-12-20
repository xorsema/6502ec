SHELL	     = /bin/sh
CC	     = gcc
FLAGS	     = -ansi -Iinclude/
CFLAGS	     = -Wall 
DEBUGFLAGS   = -O0 -g
RELEASEFLAGS = -O2
TARGET	     = 6502ec
SOURCES	     = $(shell echo src/*.c)
HEADERS	     = $(shell echo include/*.h)
OBJECTS	     = $(SOURCES:.c=.o)
LIBCFLAGS    = 
LIBS	     = 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

release: $(SOURCES) $(HEADERS)
	$(CC) $(FLAGS) $(CFLAGS) $(RELEASEFLAGS) $(LIBCFLAGS) $(LIBS) -o $(TARGET) $(SOURCES)

clean:
	-rm -f $(OBJECTS)

distclean: clean
	-rm -f $(TARGET)

.SECONDEXPANSION:

$(foreach OBJ,$(OBJECTS),$(eval $(OBJ)_DEPS = $(shell gcc -MM $(OBJ:.o=.c) | sed s/.*://)))
%.o: %.c $$($$@_DEPS)
	$(CC) $(FLAGS) $(CFLAGS) $(DEBUGFLAGS) $(LIBCFLAGS) -c -o $@ $<

.PHONY : all release \
  clean distclean
