## Makefile for libpidontrol

## Edit thise variables to configure installation path
# root to install to for `make install'
PREFIX := /usr/local
# if you use stow, root of your stow package directory
STOWBASE := /usr/local/stow
# if you use stow, a name for this stow package

## Mabe you care about these
CC := gcc
CXXFLAGS := -g -fPIC -I./include -Wall -Werror
OCXXFLAGS := -O3 -funroll-loops -I./include


## You probably don't care aboute these
VERSION := 0.0.1
PROJECT := libjs

STOWDIR := $(PROJECT)-$(VERSION)
STOWPREFIX := $(STOWBASE)/$(STOWDIR)

DISTPATH := $(HOME)/prism/tarballs
DOXPATH := $(HOME)/prism/public_html/dox




HEADERS := $(addprefix include/js/,js.h)



.PHONY: doc default clean stow


default: libjs.so

.c.o:
	$(CC) $(CXXFLAGS) -c $<

pid_test: pid_test.c
	$(CC) $(CXXFLAGS) -o $@ $< -ljs

libjs.so: js.o $(HEADERS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $<

clean:
	rm -fv *.o *.so pid_test

distclean: clean
	rm -rf doc

doc:
	doxygen

docul: doc
	cp -Tr doc/html $(DOXPATH)/$(PROJECT)

dist: distclean
	cd .. &&               \
	tar --exclude=.svn --lzma -cvf $(DISTPATH)/$(PROJECT)-$(VERSION).tar.lzma $(PROJECT)

stow: libjs.so
	mkdir -p $(STOWPREFIX)/include/ssdmu
	mkdir -p $(STOWPREFIX)/lib/
	install --mode 755 libjs.so $(STOWPREFIX)/lib
	install --mode 644 include/js/*.h $(STOWPREFIX)/include/js
	cd $(STOWBASE) && stow $(STOWDIR)


install: libjs.so
	mkdir -p $(PREFIX)/include/js
	mkdir -p $(PREFIX)/lib/
	install --mode 755 libjs.so $(PREFIX)/lib
	install --mode 644 include/js/*.h $(PREFIX)/include/js
