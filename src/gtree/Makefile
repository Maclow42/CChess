CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -O3
LDFLAGS =
LDLIBS =

SRC = main.c gtree.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main