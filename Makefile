LIB = libechos.a
EXE = echos echoc

RM = rm -f
CFLAGS = -g -O2 -Wall \
		 -std=c99 \
		 -D_XOPEN_SOURCE=600 \
		 -D_DEFAULT_SOURCE \
		 -pthread

all : ${EXE}

OBJECTS=$(patsubst %.c,%.o,$(wildcard *.c))

${LIB} : ${OBJECTS}
	${RM} ${LIB}
	${AR} rs ${LIB} *.o

clean :
	${RM} *.o ${LIB} ${EXE}

${EXE} : ${LIB}
