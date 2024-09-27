# compiler setup
CC=gcc
CFLAGS=-Wall -g

# define targets
TARGETS = biblioteca

build: $(TARGETS)

biblioteca: biblioteca.c
	$(CC) $(CFLAGS) biblioteca.c -o biblioteca

pack:
	zip -FSr 3XYCC_FirstnameLastname_Tema1.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
