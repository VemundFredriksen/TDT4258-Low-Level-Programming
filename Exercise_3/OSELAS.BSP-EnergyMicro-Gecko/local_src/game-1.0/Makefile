#
# Simple development makefile
#

#
# add more flags here,
# but don't remove the "+="
#
CFLAGS += -O2 -g -Wall#-Wsign-compare -Wfloat-equal -Wformat-security #-Werror
CPPFLAGS +=
LDFLAGS +=

OBJECTS = snake.o graphics.o

all: game

game: ${OBJECTS}
	${CC} ${CFLAGS} game.c ${OBJECTS}  -o game ${LDFLAGS}

clean:
	-rm -f game

install:


.PHONY: all install clean

# end of development makefile
