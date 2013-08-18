CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib
OBJ_DIR=./obj/
LIBS=-L./ -lcgic


all: libcgic.a  capture upload.cgi

install: libcgic.a
	cp libcgic.a /usr/local/lib
	cp cgic.h /usr/local/include
	@echo libcgic.a is in /usr/local/lib. cgic.h is in /usr/local/include.

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o
	$(RANLIB) libcgic.a

#mingw32 and cygwin users: replace .cgi with .exe

upload.cgi: upload.o libcgic.a
	$(CC) upload.o -o upload.cgi ${LIBS}
capture: capture.o libcgic.a
	$(CC) capture.o -o capture ${LIBS}
run_shell.cgi: run_shell.o libcgic.a
	$(CC) run_shell.o -o run_shell.cgi ${LIBS}
other.cgi: other.o
	$(CC) other.o -o other.cgi
mv: 
	mv *.o *.a *.cgi capture ${OBJ_DIR} 2>/dev/null #2->null需要设置MAKE结果->null how?
clean:
	cd ${OBJ_dir} ; rm -f *.o *.a  capture *.cgi ; cd ..

