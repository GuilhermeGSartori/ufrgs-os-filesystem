# Build the Test Application (TestApp).

LIBRARY_DIR =../lib
BINARY_DIR  =../bin
MAIN_INCLUDE_DIR =../include
MAIN_SOURCE_DIR  =../src
TEST_INCLUDE_DIR =./include
TEST_SOURCE_DIR  =./src
TEST_CASES_DIR   =./tests

all: t2fs test

t2fs:
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/chdir2.c -o $(BINARY_DIR)/chdir2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/close2.c -o $(BINARY_DIR)/close2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/closedir2.c -o $(BINARY_DIR)/closedir2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/create2.c -o $(BINARY_DIR)/create2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/delete2.c -o $(BINARY_DIR)/delete2.o
	gcc -Wall -c $(MAIN_SOURCE_DIR)/format2.c -o $(BINARY_DIR)/format2.o
	gcc -Wall -c $(MAIN_SOURCE_DIR)/getcwd2.c -o $(BINARY_DIR)/getcwd2.o
	gcc -Wall -c $(MAIN_SOURCE_DIR)/identify2.c -o $(BINARY_DIR)/identify2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/ln2.c -o $(BINARY_DIR)/ln2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/mkdir2.c -o $(BINARY_DIR)/mkdir2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/open2.c -o $(BINARY_DIR)/open2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/opendir2.c -o $(BINARY_DIR)/opendir2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/read2.c -o $(BINARY_DIR)/read2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/readdir2.c -o $(BINARY_DIR)/readdir2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/rmdir2.c -o $(BINARY_DIR)/rmdir2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/seek2.c -o $(BINARY_DIR)/seek2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/truncate2.c -o $(BINARY_DIR)/truncate2.o
#	gcc -Wall -c $(MAIN_SOURCE_DIR)/write2.c -o $(BINARY_DIR)/write2.o
	gcc -Wall -c $(MAIN_SOURCE_DIR)/support.c -o $(BINARY_DIR)/support.o
	gcc -Wall -c $(MAIN_SOURCE_DIR)/global_variables.c -o $(BINARY_DIR)/global_variables.o
	ar crs $(LIBRARY_DIR)/libt2fs.a $(LIBRARY_DIR)/apidisk.o $(BINARY_DIR)/*.o

test:
	rm -rf $(BINARY_DIR)/*.o
	gcc -Wall $(TEST_INCLUDE_DIR)/*.h $(TEST_SOURCE_DIR)/*.c $(TEST_CASES_DIR)/*.c $(LIBRARY_DIR)/libt2fs.a -o $(BINARY_DIR)/TestApp.exe

clean:
	rm -rf $(LIBRARY_DIR)/*.a $(BINARY_DIR)/*.o
