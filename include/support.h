/**
 * Support functions for T2FS library.
 * 
 * @author Guilherme Sartori
 * @author Marlize Ramos
 * @author Renan Kummer
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/constants.h"

// ======================================================================================
//                                   TYPES & STRUCTURES
// ======================================================================================

// Alias for C strings.
typedef char* string;


typedef struct t2fs_openfile{
    DIRENT2 record;
    DWORD currentPointer; // Em bytes a partir do inicio do arquivo!
} OpenFile;


// ======================================================================================
//                                       FUNCTIONS
// ======================================================================================

int find_target_dir(string target);

void list_entries(BYTE *block);

int get_entry_number(string name, WORD number);

/*
 *Funtion used to read an entry from a block
 */
void read_entry(BYTE *block, DIRENT2 *entry, int *iterator);

/*
 *Funtion used to find the first available block in the bitmap
 */
int free_block_bit();

/*
 *Funtion used to verify if the parameter matches with the pattern used for
 *setting the working directory to the root directory
 */
int check_if_root(char *name);

/*
 *Funtion used to save a DIRENT2 structure (or any other structure) in a array of bytes
 *the iterator is the position of the first byte to be modified
 */
void dirent_to_bytes_array(BYTE *block, DIRENT2 entry, int *iterator);

/**
 * Reads the content of a block in the disk and saves it into an array of bytes.
 * This function is partition-exclusive.
 * 
 * @param block        The array to hold the content of the block.
 * @param block_number The number of the block to read.
 * @return T2FS_SUCCESS if block could be read,
 *         T2FS_NULL_POINTER if block parameter is null,
 *         T2FS_INVALID_BLOCK_NUMBER if block_number parameter is outside the range of blocks,
 *         T2FS_FAILURE otherwise.
 */
ReturnCode read_block(BYTE *block, WORD block_number);

/**
 *Function used to write the content of a array with the size in bytes of a block
 *in a block. The block to be written is referenced by its block number (partition exclusive)
 */
int write_block(BYTE *block, WORD block_number);

/**
 *Saves a hashtable structure in a array in the iterator position
 */
int ht_to_bytes_array(BYTE *array, HashTable *ht, int *iterator);

/**
 *Saves two bytes in a array of bytes in the iterator position
 */
void two_bytes_to_bytes_array(BYTE *array, short int info, int *iterator);

/**
 *Saves four bytes in a array of bytes in the iterator position
 */
void four_bytes_to_sector_array(BYTE *sector, unsigned int info, int *iterator);

/**
 *Gets the information in the mbr sector (i think...)
 */
int mbr_info(BYTE *mbr, unsigned int *version, unsigned int *sector_bytes, unsigned int *partition_table, unsigned int *partition_count);

/**
 *Initialize every global using the informationg gathered from the superblock
 */
void global_initialization(unsigned int *information);

/**
 *Reads a DWORD (32 bytes) and save it in a usigned int. The DWORD must be stored in little-endian fashion
 */
void read_little_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator);

/**
 *Reads a DWORD (32 bytes) and save it in a usigned int. The DWORD must be stored in big-endian fashion
 */
void read_big_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator);

/**
 *Boots the file system
 */
int boot2();

/**
 * Calculates the index of an entry in current working directory's hash table. 
 * 
 * @param entry_name The name of the entry.
 * @return The index for the entry if hash table has space, T2FS_DIRECTORY_IS_FULL otherwise.
 */
int calculate_hash_table_index(string entry_name);

/**
 * Breaks the path into individual entries.
 * 
 * Example: ./a/b/c.txt breaks into
 *   [0] = "."
 *   [1] = "a"
 *   [2] = "b"
 *   [3] = "c.txt"
 * 
 * @param path       The path to parse.
 * @param array_size The size of returning array.
 * @return Array of strings with each entry.
 */
string* parse_path(string path, int *array_size);

/**
 * Get entry name from path.
 * 
 * Example: ./a/b/c.txt returns
 *   "c.txt"
 * 
 * @param path The path to parse.
 * @return The entry name.
 */
string get_entry_name(string path);


/*************************************************************************************************
 * RASCUNHO
 * 
 * -> a cada 16 linhas no disco fecha um setor!
 * -> xxxx são 2 bytes em binário
 * -> alem disso tem que verificar se tem blocos suficientes para arquivo crescer quando 
 *    for adicionar dados e etc... 
 * -> diretório vai ser indexado na real, com hash no bloco de índice, mesma proporção
 * -> 8 (6, 2), só que os 6 vão ser ocupados com entradas de 32 ainda... vai ter uma 
 *    entrada de 4 bytes pra dizer quantos livres ainda tem também ou 12 entradas de 16... 
 *    mas o q elas tem? ponteiro pra bloco e quantidade de livres nele
 * -> posso fazer 24 entradas de 8 então... 2 unsigned int (ponteiro e quantos tem livre)
 * -> quando alocar aloca bloco de índice e logo já aloca "." e ".."
 * -> .. na raiz troca de partição (reboot)
 * -> bitmap em memória, mas sempre que fizer algo, so fazer um write com ele e gg
 * -> tipo de arquivo define se é 0 = dir, 1 = txt 2 = bin
 * -> se bin lê normal mas mete uns print em binário...
 *************************************************************************************************/ 
