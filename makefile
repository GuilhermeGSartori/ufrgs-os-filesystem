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

all:
	$(CC) -Wall -c $(SRC_DIR)/support.c -o $(LIB_DIR)/support.o
	$(CC) -Wall -c $(SRC_DIR)/mkdir2.c -o $(LIB_DIR)/mkdir2.o
	$(CC) -Wall -c $(SRC_DIR)/format2.c -o $(LIB_DIR)/format2.o
	$(CC) -Wall -o ./code_test $(LIB_DIR)/apidisk.o $(LIB_DIR)/format2.o $(LIB_DIR)/support.o $(LIB_DIR)/mkdir2.o

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~


