# zeb makefile
.POSIX:

CFLAGS = -std=c99 -pipe $(CWARN) $(INCFLAGS)
CWARN = -pedantic -Wall -Wextra -Wconversion -Wshadow -Wpointer-arith \
        -Wstrict-prototypes -Wmissing-prototypes


all: zeb

zeb: zeb.c
	cc $(CFLAGS) -o $@ $<

README.html: README.md
	markdown --unsafe <$< >$@

clean:
	rm -f zeb

.PHONY: all clean
