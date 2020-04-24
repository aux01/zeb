BINARIES = altpp

all: $(BINARIES)

altpp: altpp.c
	cc -Wall -Wextra -o $@ $<

clean:
	rm -f $(BINARIES)

.PHONY: all clean
