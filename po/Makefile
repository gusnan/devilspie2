#
# Devilspie2 po makefile
# $Id$
#


ifndef prefix
	ifdef INSTALL_PREFIX
		prefix=$(INSTALL_PREFIX)
	else
		prefix=/usr/local
	endif
endif

NAME = devilspie2
VERSION = $(shell cat ../VERSION)
DATADIR = ${DESTDIR}${prefix}/share
LOCALEDIR = ${DATADIR}/locale

XG_ARGS = --keyword=_ --keyword=N_ -w 80 --package-name=${NAME} --package-version=${VERSION} --msgid-bugs-address=gusnan@gusnan.se
LINGUAS = sv_SE

all: build

${NAME}.pot:
	xgettext ${XG_ARGS} --directory=../src/ devilspie2.c script.c script_functions.c xutils.c -d ${NAME} -o ./${NAME}.pot

update-pot: ${NAME}.pot

%.pox: %.po
	msgmerge -o $@ --previous $< ${NAME}.pot 

build:
	for po in ${LINGUAS}; \
	do msgfmt -v --statistics -c -o t-$${po} $${po}.po && mv -f t-$${po} $${po}.mo; \
	done
	

install: all install-dirs
	for po in ${LINGUAS}; \
	do install -m 0644 $${po}.mo ${LOCALEDIR}/$${po}/LC_MESSAGES/${NAME}.mo; \
	done

install-dirs:
	for po in ${LINGUAS}; \
	do install -d ${LOCALEDIR}/$${po}/LC_MESSAGES; \
	done

uninstall:
	for po in ${LINGUAS}; \
	do rm -f  ${LOCALEDIR}/$${po}/LC_MESSAGES/${NAME}.mo; \
	done

clean-build:
	rm -f *.mo

clean-pox:
	rm -f *.pox

clean: clean-build
	rm -f *~

.PHONY: all build install install-dirs uninstall clean clean-pox clean-build update-pot

