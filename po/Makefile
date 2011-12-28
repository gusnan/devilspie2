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

XG_ARGS = --keyword=_ --flag=_:1:pass-java-format -L Perl -w 80 --package-name=${NAME} --package-version=${VERSION} --msgid-bugs-address=ricardo@mones.org
LINGUAS = sv_SE

all: build

${NAME}.pot.0:
	xgettext ${XG_ARGS} -f ./POTFILES -d ${NAME} -o ./${NAME}.pot.0

# remove bogus entry win_title
update-pot: ${NAME}.pot.0
	head -17 ./${NAME}.pot.0 > ./${NAME}.pot
	tail -$(shell expr $(shell wc -l < ./${NAME}.pot.0) - 22 ) ./${NAME}.pot.0 >> ./${NAME}.pot
	rm -f ./${NAME}.pot.0

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

