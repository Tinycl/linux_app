DEFAULT:smbus

CC = gcc
CPP = g++

DATE = date +%y%m%d

OBJS = smbus.o

smbus: ${OBJS} 

smbus:
	${CPP} ${OBJS} -o smbus

.ccp.o:
	${CPP} -w -c $< -o $@
.c.o:
	${CPP} -w -c $< -o $@
.S.o:
	${CPP} -w -c $< -o $@

clean:
	rm -f *.o smbus
