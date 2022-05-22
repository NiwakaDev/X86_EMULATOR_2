.PHONY:clean build all test

CC = clang++

G++_OPTIONS = -Wall -std=c++14 -O0 -g -I $(INCLUDE_DIR) -DDEBUG

SOURCE_DIR         = ../../src/
INCLUDE_DIR        = ../../include/
DETAIL_INCLUDE_DIR = ../../include/detail/

HEADERS        = $(wildcard $(INCLUDE_DIR)*.h)
DETAIL_HEADERS = $(wildcard $(DETAIL_INCLUDE_DIR)*.h)

test_cpu : test.o test_cpu.o object.o SegmentRegister.o Memory.o Bios.o Gdtr.o Idtr.o TaskRegister.o Instruction.o InstructionFactory.o
	$(CC) $(LD_FLGS) -o test_cpu  $^ 

test.o : test.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

object.o : $(SOURCE_DIR)Object.cpp 
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_cpu.o : $(SOURCE_DIR)Cpu.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_segment_register.o : $(SOURCE_DIR)test_segment_register.o.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_memory.o : $(SOURCE_DIR)test_memory.o.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_bios.o : $(SOURCE_DIR)test_bios.o.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

test_gdtr.o : $(SOURCE_DIR)test_gdtr.o.cpp $(HEADERS) $(DETAIL_HEADERS)
	$(CC) $(G++_OPTIONS) -o $@ -c $<

clean :
	rm *.o