.PHONY:clean build all

EMULATOR = x86

CC = g++
G++_OPTIONS = -Wall -std=c++11 -O2 -I ${INCLUDE_DIR}
#G++_OPTIONS = -Wall -std=c++11 -O0 -g -I ${INCLUDE_DIR}
G++_OPTIONS += -lSDL2

SOURCE_DIR      = ./src/
INCLUDE_DIR     = ./include/

vpath %.h ${INCLUDE_DIR}
vpath %.cpp ${SOURCE_DIR}

HEADERS = $(wildcard ${INCLUDE_DIR}*.h)
SOURCES = $(wildcard ${SOURCE_DIR}*.cpp)
OBJECTS = ${SOURCES:.cpp=.o}

all:${EMULATOR}

%.o : %.cpp ${HEADERS}
	${CC} ${G++_OPTIONS} -o $@ -c $<

${EMULATOR} : ${OBJECTS} 
	${CC} ${LD_FLGS} ${G++_OPTIONS} -o ${OUTPUTS_DIR}${EMULATOR} $^

clean :
	rm ${OUTPUTS_DIR}${EMULATOR}
	rm ${SOURCE_DIR}*.o
