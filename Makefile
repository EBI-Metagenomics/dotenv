.POSIX:

VERSION := 1.0.2
TAP_VERSION := 0.1.0
TAP_URL := https://github.com/zorgnax/libtap/archive/refs/tags/$(TAP_VERSION).tar.gz

CC ?= gcc
CFLAGS := $(CFLAGS) -std=c99 -Wall -Wextra

SRC := dotenv.c
OBJ := $(SRC:.c=.o)
HDR := dotenv.h

all: check

libtap-$(TAP_VERSION).tar.gz:
	curl -Ls $(TAP_URL) --output libtap-$(TAP_VERSION).tar.gz

libtap.a tap.h: libtap-$(TAP_VERSION).tar.gz
	tar xzf $<
	cd libtap-$(TAP_VERSION) && make PREFIX=$(PWD)/libtap-$(TAP_VERSION) install
	mv libtap-$(TAP_VERSION)/lib/libtap.a .
	mv libtap-$(TAP_VERSION)/include/tap.h .
	rm -rf libtap-$(TAP_VERSION)

tapview:
	curl -OLs https://gitlab.com/esr/tapview/-/raw/master/tapview
	chmod +x tapview

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test_dotenv.o: libtap.a | tap.h

test_dotenv: test_dotenv.o $(OBJ)
	$(CC) $(CFLAGS) $^ libtap.a -o $@

test check: test_dotenv tapview assets/test.valid.env assets/.env
	./test_dotenv | ./tapview

dist: clean
	mkdir -p dotenv-$(VERSION)
	cp -R README.md LICENSE dotenv.h dotenv.c dotenv-$(VERSION)
	tar -cf - dotenv-$(VERSION) | gzip > dotenv-$(VERSION).tar.gz
	rm -rf dotenv-$(VERSION)

distclean:
	rm -f dotenv-$(VERSION).tar.gz

clean: distclean
	rm -f test_dotenv *.o libtap-$(TAP_VERSION).tar.gz libtap.a tap.h tapview
	rm -rf libtap-$(TAP_VERSION)

.PHONY: all check test dist distclean
