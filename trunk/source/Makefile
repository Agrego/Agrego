#
# This is the Agrego! system Makefile
#

# definitions

CC=gcc
LDFLAGS=-ldl /usr/lib/libssl.so -lrt
LDFLAGS_CUNIT=-ldl /usr/lib/libcunit.so
CFLAGS=-O2 -W -Wall -Werror -pedantic # -ansi -D_POSIX_SOURCE  // Commented for now, awaiting understanding of the problems it causes

# -lrt for timer functions

#-D_POSIX_SOURCE
# else I have :
# implicit declaration of function ‘kill’
# if I drop '-ansi' it works too

#--pedantic-errors
# We can't use --pedantic-errors because there is a bug a the file
# /usr/include/bits/resource.h:161: error: comma at end of enumerator list
# the error doesn't appears without --pedantic-errors

EXEC=agrego-local agrego-distant
EXEC_DIR=bin/
EXEC_TEST=cunit-local cunit-distant

SRC=$(wildcard *.c) $(wildcard iniparser/*.c)

SRC_LOCAL=$(SRC) $(wildcard local/*.c)
OBJ_LOCAL=$(SRC_LOCAL:.c=.o)

SRC_DISTANT=$(SRC) $(wildcard distant/*.c)
OBJ_DISTANT=$(SRC_DISTANT:.c=.o)

SRC_TEST=$(wildcard cunit/*.c)

SRC_TEST_LOCAL=$(SRC_TEST) $(filter-out main.c, $(SRC_LOCAL))
OBJ_TEST_LOCAL=$(SRC_TEST_LOCAL:.c=.o)

SRC_TEST_DISTANT=$(SRC_TEST) $(filter-out main.c, $(SRC_DISTANT))
OBJ_TEST_DISTANT=$(SRC_TEST_DISTANT:.c=.o)

# main target

all: $(EXEC)

cunit: $(EXEC_TEST)
	$(EXEC_DIR)cunit-distant
	$(EXEC_DIR)cunit-local

## final target

agrego-distant: $(OBJ_DISTANT)
	$(CC) -o $(EXEC_DIR)$@ $^ $(LDFLAGS)

agrego-local: $(OBJ_LOCAL)
	$(CC) -o $(EXEC_DIR)$@ $^ $(LDFLAGS)

## Unit test rules

cunit-distant: $(OBJ_TEST_DISTANT)
	$(CC) -o $(EXEC_DIR)$@ $^ $(LDFLAGS) $(LDFLAGS_CUNIT)

cunit-local: $(OBJ_TEST_LOCAL)
	$(CC) -o $(EXEC_DIR)$@ $^ $(LDFLAGS) $(LDFLAGS_CUNIT)

## devel documentation

doc:
	doxygen agrego-Doxyfile

## Debian package creation rules

local-deb: agrego-local
	bash scripts/makedeb.sh local .

distant-deb: agrego-distant
	bash scripts/makedeb.sh distant .

deb: local-deb distant-deb

# generic target

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	find . -name "*.o" -exec rm {} \;

fclean: clean
	cd $(EXEC_DIR); rm -f $(EXEC_TEST) $(EXEC)

clean-deb:
	rm -f scripts/*.deb

re: fclean all
