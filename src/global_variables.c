#include "../include/t2fs.h"

/**
 * Definition of global variables.
 * 
 * To use them in a file, declare as:
 *  - extern <type> <variable_name>;
 * 
 * @author Guilherme Sartori
 * @author Marlize Ramos
 * @author Renan Kummer
 */

// Holds the current working directory path.
char working_dir_path[10000];
WORD working_dir_block;
_Bool __boot_init = 0;
_Bool __root_done = 1;//aplica o mkdir pra cada partição depois deixa isso 1...
int partition = 0;
unsigned int bitmap_start = 0, bitmap_end = 0, superblock_start = 0, number_of_blocks = 0;
unsigned int sectors_per_block = 0, root_sector = 0, partition_end = 0, root_block = 0, entry_p_dir_blck = 0, dir_idx_adrs_bytes = 0;
unsigned int dir_idx_hash_bytes = 0, dir_idx_adrs_number = 0, dir_files_max = 0, hash_size = 0, dir_idx_leftovers = 0;
unsigned int file_idx_entries = 0, file_max_size = 0, block_size = 0;
unsigned int information[16];
BYTE *bitmap;
