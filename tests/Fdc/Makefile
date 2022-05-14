.PHONY:clean build all test

CC = clang++

G++_OPTIONS = -Wall -std=c++14 -O0 -g -I $(INCLUDE_DIR) -DDEBUG
#LD_FLGS += -lSDL2


SOURCE_DIR         = ../../src/
INCLUDE_DIR        = ../../include/
DETAIL_INCLUDE_DIR = ../../include/detail/

HEADERS        = $(wildcard $(INCLUDE_DIR)*.h)
DETAIL_HEADERS = $(wildcard $(DETAIL_INCLUDE_DIR)*.h)

test_fdc : test.o test_fdc.o object.o
	$(CC) $(LD_FLGS) -o test_fdc  $^ 

test.o : test.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

object.o : $(SOURCE_DIR)Object.cpp 
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_fdc.o : $(SOURCE_DIR)Fdc.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

clean :
	rm *.o