.PHONY:clean build all test

CC = clang++

G++_OPTIONS = -Wall -std=c++20 -O0 -g -I $(INCLUDE_DIR) -DDEBUG -arch x86_64
LD_FLGS += -lgtest_main -lgtest -arch x86_64


SOURCE_DIR         = ../../src/
INCLUDE_DIR        = ../../include/
DETAIL_INCLUDE_DIR = ../../include/detail/

HEADERS        = $(wildcard $(INCLUDE_DIR)*.h)
DETAIL_HEADERS = $(wildcard $(DETAIL_INCLUDE_DIR)*.h)

test_pic : test.o test_pic.o test_object.o
	$(CC) $(LD_FLGS) -o test_pic  $^ 

test.o : test.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_pic.o : $(SOURCE_DIR)Pic.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_object.o : $(SOURCE_DIR)Object.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

clean:
	rm *.o