#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
# 

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: t2fs t2fslib

t2fs:
	$(CC) -Wall -c $(SRC_DIR)/write2.c -o $(BIN_DIR)/write2.o
	$(CC) -Wall -c $(SRC_DIR)/truncate2.c -o $(BIN_DIR)/truncate2.o
	$(CC) -Wall -c $(SRC_DIR)/seek2.c -o $(BIN_DIR)/seek2.o
	$(CC) -Wall -c $(SRC_DIR)/rmdir2.c -o $(BIN_DIR)/rmdir2.o
	$(CC) -Wall -c $(SRC_DIR)/read2.c -o $(BIN_DIR)/read2.o
	$(CC) -Wall -c $(SRC_DIR)/open2.c -o $(BIN_DIR)/open2.o
	$(CC) -Wall -c $(SRC_DIR)/ln2.c -o $(BIN_DIR)/ln2.o
	$(CC) -Wall -c $(SRC_DIR)/identify2.c -o $(BIN_DIR)/identify2.o
	$(CC) -Wall -c $(SRC_DIR)/delete2.c -o $(BIN_DIR)/delete2.o
	$(CC) -Wall -c $(SRC_DIR)/create2.c -o $(BIN_DIR)/create2.o
	$(CC) -Wall -c $(SRC_DIR)/close2.c -o $(BIN_DIR)/close2.o
	$(CC) -Wall -c $(SRC_DIR)/closedir2.c -o $(BIN_DIR)/closedir2.o
	$(CC) -Wall -c $(SRC_DIR)/readdir2.c -o $(BIN_DIR)/readdir2.o
	$(CC) -Wall -c $(SRC_DIR)/opendir2.c -o $(BIN_DIR)/opendir2.o
	$(CC) -Wall -c $(SRC_DIR)/getcwd2.c -o $(BIN_DIR)/getcwd2.o
	$(CC) -Wall -c $(SRC_DIR)/chdir2.c -o $(BIN_DIR)/chdir2.o
	$(CC) -Wall -c $(SRC_DIR)/global_variables.c -o $(BIN_DIR)/global_variables.o
	$(CC) -Wall -c $(SRC_DIR)/support.c -o $(BIN_DIR)/support.o
	$(CC) -Wall -c $(SRC_DIR)/mkdir2.c -o $(BIN_DIR)/mkdir2.o
	$(CC) -Wall -c $(SRC_DIR)/format2.c -o $(BIN_DIR)/format2.o

t2fslib:
	ar crs $(LIB_DIR)/libt2fs.a $(LIB_DIR)/apidisk.o $(BIN_DIR)/format2.o $(BIN_DIR)/support.o $(BIN_DIR)/mkdir2.o $(BIN_DIR)/global_variables.o $(BIN_DIR)/chdir2.o $(BIN_DIR)/getcwd2.o $(BIN_DIR)/opendir2.o $(BIN_DIR)/readdir2.o $(BIN_DIR)/closedir2.o $(BIN_DIR)/close2.o $(BIN_DIR)/create2.o $(BIN_DIR)/delete2.o $(BIN_DIR)/identify2.o $(BIN_DIR)/ln2.o $(BIN_DIR)/open2.o $(BIN_DIR)/read2.o $(BIN_DIR)/rmdir2.o $(BIN_DIR)/seek2.o $(BIN_DIR)/truncate2.o $(BIN_DIR)/write2.o

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/format2.o $(BIN_DIR)/support.o $(BIN_DIR)/mkdir2.o $(BIN_DIR)/global_variables.o $(BIN_DIR)/chdir2.o $(BIN_DIR)/getcwd2.o $(BIN_DIR)/opendir2.o $(BIN_DIR)/readdir2.o $(BIN_DIR)/closedir2.o


