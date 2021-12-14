.PHONY:clean build all

EMULATOR = x86

CC = clang++

#G++_OPTIONS = -std=c++11 -O2 -I $(INCLUDE_DIR)
#G++_OPTIONS = -std=c++11 -O3 -mtune=native -march=native -I $(INCLUDE_DIR)
G++_OPTIONS = -Wall -std=c++11 -O0 -g -I $(INCLUDE_DIR)
LD_FLGS += -lSDL2


SOURCE_DIR      = ./src/
INCLUDE_DIR     = ./include/

HEADERS = $(wildcard $(INCLUDE_DIR)*.h)
SOURCES = $(wildcard $(SOURCE_DIR)*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:$(EMULATOR)

%.o : %.cpp $(HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

$(EMULATOR) : $(OBJECTS) 
	$(CC) $(LD_FLGS) -o $(OUTPUTS_DIR)$(EMULATOR) $^

clean :
	rm $(OUTPUTS_DIR)$(EMULATOR)
	rm $(SOURCE_DIR)*.o
