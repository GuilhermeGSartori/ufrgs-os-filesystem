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
	gcc -Wall -c $(MAIN_SOURCE_DIR)/t2fs.c -o $(BINARY_DIR)/t2fs.o
	ar crs $(LIBRARY_DIR)/libt2fs.a $(LIBRARY_DIR)/apidisk.o $(BINARY_DIR)/*.o

test:
	rm -rf $(BINARY_DIR)/*.o
	gcc -Wall $(TEST_INCLUDE_DIR)/*.h $(TEST_SOURCE_DIR)/*.c $(TEST_CASES_DIR)/*.c $(LIBRARY_DIR)/libt2fs.a -o $(BINARY_DIR)/TestApp.exe

clean:
	rm -rf $(LIBRARY_DIR)/*.a $(BINARY_DIR)/*.o
