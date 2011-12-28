CC=gcc
SRC=src
OBJ=obj
BIN=bin

ifdef DEBUG
	STD_CFLAGS=-c -Wall -g3 -ggdb -D_DEBUG
else
	STD_CFLAGS=-c -Wall
endif

PROG=$(BIN)/devilspie2
DEPEND=Makefile.dep

OBJECTS=$(OBJ)/devilspie2.o $(OBJ)/xutils.o $(OBJ)/script.o $(OBJ)/script_functions.o

ifndef prefix
	ifdef INSTALL_PREFIX
		prefix=$(INSTALL_PREFIX)
	else
		prefix=/usr/local
	endif
endif

ifdef GTK2
	PKG_GTK=gtk+-2.0
	PKG_WNCK=libwnck-1.0

	CHECK_GTK3=1
else
	PKG_GTK=gtk+-3.0
	PKG_WNCK=libwnck-3.0
endif 

LIB_CFLAGS=`pkg-config --cflags $(PKG_GTK) $(PKG_WNCK) lua5.1`
STD_LDFLAGS=
LIBS=`pkg-config --libs $(PKG_GTK) $(PKG_WNCK) lua5.1`

LOCAL_CFLAGS=$(STD_CFLAGS) $(DEPRECATED) $(CFLAGS) $(LIB_CFLAGS)
LOCAL_LDFLAGS=$(LDFLAGS) $(STD_LDFLAGS)

ifdef CHECK_GTK3
	LOCAL_CFLAGS+=-DGTK_DISABLE_SINGLE_INCLUDES
	LOCAL_CFLAGS+=-DGSEAL_ENABLE

	CHECK_DEPRECATED=1
endif

ifdef CHECK_DEPRECATED
	LOCAL_CFLAGS+=-DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
endif 

.PHONY: all
all: $(BIN)/devilspie2
	${MAKE} -C po build

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(LOCAL_CFLAGS) -c $< -o $@

$(BIN)/devilspie2: $(OBJECTS)
	$(CC) $(LOCAL_LDFLAGS) $(OBJECTS) -o $(PROG) $(LIBS)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(PROG) $(DEPEND)
	${MAKE} -C po clean

install:
	install -d $(DESTDIR)$(prefix)/bin
	install -m 755 $(PROG) $(DESTDIR)$(prefix)/bin
	${MAKE} -C po install

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(prefix)/$(PROG)
	${MAKE} -C po uninstall

$(DEPEND):
	$(CC) -MM $(SRC)/*.c | sed -e "s/\([A-Za-z0-9+-0._&+-]*:\)/\$(OBJ)\/\1/g" > $(DEPEND)

-include $(DEPEND)
