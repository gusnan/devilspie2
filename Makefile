CC=gcc
SRC=src
OBJ=obj
BIN=bin

ifdef DEBUG
	STD_CFLAGS=-c -Wall -g3 -ggdb
else
	STD_CFLAGS=-c -Wall
endif

PROG=bin/devilspie2

ifndef DESTDIR
DESTDIR=/usr/local
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
STD_LDFLAGS= `pkg-config --libs $(PKG_GTK) $(PKG_WNCK) lua5.1`

LOCAL_CFLAGS=$(STD_CFLAGS) $(LIB_CFLAGS) $(DEPRECATED)
LOCAL_LDFLAGS=$(STD_LDFLAGS)

ifdef CHECK_GTK3
	LOCAL_CFLAGS+=-DGTK_DISABLE_SINGLE_INCLUDES
	LOCAL_CFLAGS+=-DGSEAL_ENABLE
	
	CHECK_DEPRECATED=1
endif

ifdef CHECK_DEPRECATED
	LOCAL_CFLAGS+=-DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
endif 

all: $(BIN)/devilspie2

$(OBJ)/devilspie2.o: $(SRC)/devilspie2.c $(SRC)/version.h $(SRC)/script.h $(SRC)/script_functions.h
	$(CC) $(LOCAL_CFLAGS) $(SRC)/devilspie2.c -o $(OBJ)/devilspie2.o

$(OBJ)/xutils.o: $(SRC)/xutils.c $(SRC)/xutils.h
	$(CC) $(LOCAL_CFLAGS) $(SRC)/xutils.c -o $(OBJ)/xutils.o

$(OBJ)/script.o: $(SRC)/script.c $(SRC)/script.h $(SRC)/script_functions.h
	$(CC) $(LOCAL_CFLAGS) $(SRC)/script.c -o $(OBJ)/script.o

$(OBJ)/script_functions.o: $(SRC)/script_functions.c $(SRC)/script.h $(SRC)/xutils.h
	$(CC) $(LOCAL_CFLAGS) $(SRC)/script_functions.c -o $(OBJ)/script_functions.o

$(BIN)/devilspie2: $(OBJ)/devilspie2.o $(OBJ)/xutils.o $(OBJ)/script.o $(OBJ)/script_functions.o
	$(CC) $(OBJ)/devilspie2.o $(OBJ)/script.o $(OBJ)/script_functions.o $(OBJ)/xutils.o $(LOCAL_LDFLAGS) -o $(PROG)

clean:
	rm -rf $(OBJ)/*.o $(PROG)

install:
	install -d $(DESTDIR)/bin
	install -m 755 $(PROG) $(DESTDIR)/bin

uninstall:
	rm -f $(DESTDIR)/$(PROG)
