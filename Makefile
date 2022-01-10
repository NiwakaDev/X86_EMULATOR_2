.PHONY:clean build all

EMULATOR = x86

CC = clang++

G++_OPTIONS = -std=c++11 -O2 -I $(INCLUDE_DIR)
#G++_OPTIONS = -std=c++11 -O3 -I $(INCLUDE_DIR)
#G++_OPTIONS = -Wall -std=c++11 -O0 -g -I $(INCLUDE_DIR) -DDEBUG
#G++_OPTIONS = -Wall -std=c++11 -O0 -g -I $(INCLUDE_DIR)
LD_FLGS += -lSDL2 -lSDL2_mixer

SOURCE_DIR         = ./src/
INCLUDE_DIR        = ./include/
DETAIL_INCLUDE_DIR = ./include/detail/

HEADERS        = $(wildcard $(INCLUDE_DIR)*.h)
DETAIL_HEADERS = $(wildcard $(DETAIL_INCLUDE_DIR)*.h)

SOURCES = $(wildcard $(SOURCE_DIR)*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:$(EMULATOR)

%.o : %.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

$(EMULATOR) : $(OBJECTS) 
	$(CC) $(LD_FLGS) -o $(OUTPUTS_DIR)$(EMULATOR) $^ 

clean :
	rm $(OUTPUTS_DIR)$(EMULATOR)
	rm $(SOURCE_DIR)*.o
