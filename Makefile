.PHONY:clean build all test

EMULATOR            = x86
EMULATOR_TEST       = x86_test

CC = clang++

G++_OPTIONS = -std=c++14 -O3 -I $(INCLUDE_DIR)
G++_OPTIONS_DEBUG = -Wall -std=c++14 -O0 -g -I $(INCLUDE_DIR) -DDEBUG
LD_FLGS += -lSDL2
#LD_FLGS += -lSDL2 -lSDL2_mixer


SOURCE_DIR         = ./src/
INCLUDE_DIR        = ./include/
DETAIL_INCLUDE_DIR = ./include/detail/

HEADERS        = $(wildcard $(INCLUDE_DIR)*.h)
DETAIL_HEADERS = $(wildcard $(DETAIL_INCLUDE_DIR)*.h)

SOURCES       = $(wildcard $(SOURCE_DIR)*.cpp)
OBJECTS       = $(SOURCES:.cpp=.o)
OBJECTS_DEBUG = $(SOURCES:.cpp=.test.o)

all:$(EMULATOR)

%.o : %.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<


$(EMULATOR) : $(OBJECTS) 
	$(CC) $(LD_FLGS) -o $(OUTPUTS_DIR)$(EMULATOR) $^ 

%.test.o : %.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS_DEBUG) -o $@ -c $<

x86_test : $(OBJECTS_DEBUG)
	$(CC) $(LD_FLGS) -o $(EMULATOR_TEST) $^ 

clean :
	rm -f $(OUTPUTS_DIR)$(EMULATOR)
	rm -f $(SOURCE_DIR)*.o
