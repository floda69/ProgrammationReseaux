# http://www.lab.dit.upm.es/~cdatlab/exs/rpc/test/
# rpcgen example Makefile
# Internetworking with TCP/IP, Volume III
#
CC=gcc -ansi  -D_GNU_SOURCE
PROGS = rdict rdictd

DEFS = 
CFLAGS = -g ${DEFS} ${INCLUDE}

SRC = rdict.c rdict.x rdict_cif.c rdict_server.c rdict_srp.c
GFILES = rdict.h rdict_clnt.c rdict_svc.c rdict_xdr.c

RDICT_OBJ = rdict_clnt.o rdict_cif.o rdict.o
RDICTD_OBJ = rdict_svc.o rdict_server.o rdict_srp.o

all: ${PROGS}

rdict: ${RDICT_OBJ} rdict_xdr.o
	${CC} ${CFLAGS} -lnsl -o $@ ${RDICT_OBJ} rdict_xdr.o

rdictd: ${RDICTD_OBJ} rdict_xdr.o
	${CC} ${CFLAGS} -lnsl -o $@ ${RDICTD_OBJ} rdict_xdr.o

rdict.h rdict_clnt.c rdict_svc.c rdict_xdr.c: rdict.x
	rpcgen -C -N rdict.x

clean: FRC
	rm -f Makefile.bak a.out tags core make.out ${PROGS} ${GFILES} *.o

install: all FRC
	@echo nothing to install.

print: Makefile ${SRC} FRC
	lpr -J"${PROG} source" Makefile ${SRC}

tags: ${SRC}
	ctags ${SRC}

FRC:
	
rdict: rdict.c ${RDICT_OBJ} ${GFILES} rdict.x
rdictd: ${RDICTD_OBJ} ${GFILES} rdict.x

