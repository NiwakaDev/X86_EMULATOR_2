.PHONY:clean build all test

CC = clang++

G++_OPTIONS = -Wall -std=c++14 -O0 -g -I $(INCLUDE_DIR) -DDEBUG -arch x86_64
LD_FLGS += -lSDL2 -lgtest_main -lgtest -arch x86_64


SOURCE_DIR         = ../../src/
INCLUDE_DIR        = ../../include/
DETAIL_INCLUDE_DIR = ../../include/detail/

HEADERS        = $(wildcard $(INCLUDE_DIR)*.h)
DETAIL_HEADERS = $(wildcard $(DETAIL_INCLUDE_DIR)*.h)

test_sib : test.o test_sib.o
	$(CC) $(LD_FLGS) -o test_sib  $^ 

test.o : test.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_sib.o : $(SOURCE_DIR)Sib.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

clean:
	rm *.o