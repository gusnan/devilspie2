CC=gcc
SRC=src
OBJ=obj
BIN=bin

ifdef DEBUG
	STD_CFLAGS=-Wall -g3 -ggdb -D_DEBUG
else
	STD_CFLAGS=-Wall -Wformat -Wno-format-extra-args -Wformat-security -Wformat-nonliteral -Wformat=2
endif

DEPEND=Makefile.dep

OBJECTS=$(OBJ)/config.o $(OBJ)/devilspie2.o $(OBJ)/xutils.o $(OBJ)/script.o $(OBJ)/script_functions.o $(OBJ)/error_strings.o

ifndef PREFIX
	ifdef INSTALL_PREFIX
		PREFIX=$(INSTALL_PREFIX)
	else
		PREFIX=/usr/local
	endif
endif

NAME = devilspie2
PROG=$(BIN)/$(NAME)
VERSION = $(shell cat ./VERSION)
DATADIR = ${DESTDIR}${PREFIX}/share
LOCALEDIR = ${DATADIR}/locale

ifdef GTK2
	PKG_GTK=gtk+-2.0
	PKG_WNCK=libwnck-1.0

	CHECK_GTK3=1
else
	PKG_GTK=gtk+-3.0
	PKG_WNCK=libwnck-3.0
endif

LIB_CFLAGS=$(shell pkg-config --cflags --silence-errors $(PKG_GTK) $(PKG_WNCK) lua5.1 || pkg-config --cflags $(PKG_GTK) $(PKG_WNCK) lua)
STD_LDFLAGS=
LIBS=-lX11 $(shell pkg-config --libs --silence-errors $(PKG_GTK) $(PKG_WNCK) lua5.1 || pkg-config --libs $(PKG_GTK) $(PKG_WNCK) lua)

LOCAL_CFLAGS=$(STD_CFLAGS) $(DEPRECATED) $(CFLAGS) $(LIB_CFLAGS)
LOCAL_LDFLAGS=$(LDFLAGS) $(STD_LDFLAGS)

LOCAL_CPPFLAGS=$(CPPFLAGS)

ifdef CHECK_GTK3
	LOCAL_CFLAGS+=-DGTK_DISABLE_SINGLE_INCLUDES
	LOCAL_CFLAGS+=-DGSEAL_ENABLE

	CHECK_DEPRECATED=1
endif

ifdef CHECK_DEPRECATED
	LOCAL_CFLAGS+=-DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
endif

LOCAL_CFLAGS+=-DLOCALEDIR=\"$(LOCALEDIR)\" -DPACKAGE=\"$(NAME)\" -DDEVILSPIE2_VERSION=\"$(VERSION)\"

.PHONY: all
all: $(BIN)/devilspie2
	${MAKE} -C po -j1 all

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_CPPFLAGS) -c $< -o $@

$(BIN)/$(NAME): $(OBJECTS)
	$(CC) $(LOCAL_CFLAGS) $(LOCAL_LDFLAGS) $(OBJECTS) -o $(PROG) $(LIBS)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(PROG) $(DEPEND)
	${MAKE} -C po clean

install:
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(PROG) $(DESTDIR)$(PREFIX)/bin
	${MAKE} -C po install

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/$(PROG)
	${MAKE} -C po uninstall

$(DEPEND):
	$(CC) -MM $(SRC)/*.c | sed -e "s/\([A-Za-z0-9+-0._&+-]*:\)/\$(OBJ)\/\1/g" > $(DEPEND)

-include $(DEPEND)
