CC=gcc
SRC=src
OBJ=obj
BIN=bin

ifdef DEBUG
	STD_CFLAGS=-c -Wall -g3 -ggdb
else
	STD_CFLAGS=-c -Wall
endif

ifdef CHECK_DEPRECATED
DEPRECATED=-DGDK_PIXBUF_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
endif

LIB_CFLAGS=`pkg-config --cflags gtk+-2.0 lua5.1 libwnck-1.0`
STD_LDFLAGS= `pkg-config --libs gtk+-2.0 lua5.1 libwnck-1.0`

CFLAGS=$(STD_CFLAGS) $(LIB_CFLAGS) $(DEPRECATED)
LDFLAGS=$(STD_LDFLAGS)

all: devilspie2

$(OBJ)/devilspie2.o: $(SRC)/devilspie2.c
	$(CC) $(CFLAGS) $(SRC)/devilspie2.c -o $(OBJ)/devilspie2.o

$(OBJ)/xutils.o: $(SRC)/xutils.c
	$(CC) $(CFLAGS) $(SRC)/xutils.c -o $(OBJ)/xutils.o

$(OBJ)/script.o: $(SRC)/script.c
	$(CC) $(CFLAGS) $(SRC)/script.c -o $(OBJ)/script.o

$(OBJ)/script_functions.o: $(SRC)/script_functions.c
	$(CC) $(CFLAGS) $(SRC)/script_functions.c -o $(OBJ)/script_functions.o

devilspie2: $(OBJ)/devilspie2.o $(OBJ)/xutils.o $(OBJ)/script.o $(OBJ)/script_functions.o
	$(CC) $(LDFLAGS) $(OBJ)/devilspie2.o $(OBJ)/script.o $(OBJ)/script_functions.o $(OBJ)/xutils.o -o $(BIN)/devilspie2

clean:
	rm -rf $(OBJ)/*.o $(BIN)/devilspie2
