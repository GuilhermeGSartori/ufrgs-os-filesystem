/** support file:
// includes: Auxiliar functions
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/support.h"
#include "../include/t2fs.h"
#include "../include/apidisk.h"

/*
 * Global Variables Declaration
 * When used, they need to be declares in the files
 * that require them. Using "extern <type> <name>"
*/

_Bool __boot_init = 0;
int partition = 0;
unsigned int bitmap_start = 0, bitmap_end = 0;
unsigned int sectors_per_block = 0, root_sector = 0, partition_end = 0, root_block = 0, entry_p_dir_blck = 0, dir_idx_adrs_bytes = 0;
unsigned int dir_idx_hash_bytes = 0, dir_idx_adrs_number = 0, dir_files_max = 0, hash_size = 0, dir_idx_leftovers = 0;
unsigned int file_idx_entries = 0, file_max_size = 0, block_size = 0;
unsigned int information[14];
static BYTE *bitmap;


#define SecMBR 0

int boot2()
{
	int i;
	unsigned int first_sector = 0;
	unsigned int second_sector = 0;
	unsigned int partition_count = 0, partition_table = 0;
	unsigned int version = 0, sector_bytes = 0, i_byte_adrs = 0;

	BYTE *bitmap_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
	BYTE *info_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
	BYTE *mbr = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);

	if(read_sector(SecMBR, mbr) != 0)
		return -1;

	if(mbr_info(mbr, &version, &sector_bytes, &partition_table, &partition_count) != 0)
		return -2;

	unsigned int start[partition_count];

	i_byte_adrs = partition_table;
	for(i = 0; i < partition_count; i++)
	{
		read_little_DWORD(mbr, &start[i], i_byte_adrs);
		i_byte_adrs += 32;
	}

	first_sector = start[partition];
	second_sector = start[partition] + 1;

	if(read_sector(first_sector, bitmap_sector) != 0)
		return -2;

	if(read_sector(second_sector, info_sector) != 0)
		return -3;


	i_byte_adrs = 0;
	read_big_DWORD(bitmap_sector, &bitmap_start, i_byte_adrs);
	i_byte_adrs += 4;
	read_big_DWORD(bitmap_sector, &bitmap_end, i_byte_adrs);

	printf("READ -> Bitmap start: %d\n", bitmap_start);
	printf("READ -> Bitmap end: %d\n", bitmap_end);

	bitmap = (BYTE *) malloc(sizeof(BYTE )*(bitmap_end - bitmap_start));

	//printf("bitmap size: %d\n", sizeof(*bitmap));
	printf("READ -> Bitmap: \n");
	i_byte_adrs = bitmap_start;

	for(i = 0; i < bitmap_end - bitmap_start; i++)
	{
		bitmap[i] = bitmap_sector[i_byte_adrs];
		printf("	Bitmap value byte %d: %d \n",i, bitmap[i]);
		i_byte_adrs++;
	}

	i_byte_adrs = 0;
	for(i = 0; i < 14; i++)
	{
		read_big_DWORD(info_sector, &information[i], i_byte_adrs);
		i_byte_adrs += 4;
	}

	global_initialization(information);

	printf("READ -> sectors per block: %d\n", sectors_per_block);
	printf("READ -> root sector: %d\n", root_sector);
	printf("READ -> partition end: %d\n", partition_end);
	printf("READ -> root block: %d\n", root_block);
	printf("READ -> entry per block of directories: %d\n", entry_p_dir_blck);
	printf("READ -> bytes for entry blocks in the directory index: %d\n", dir_idx_adrs_bytes);
	printf("READ -> bytes for the hash table in the directory index: %d\n", dir_idx_hash_bytes);
	printf("READ -> number of entries for the DIRENT2 blocks in the index: %d\n", dir_idx_adrs_number);
	printf("READ -> max number of files in a directory: %d\n", dir_files_max);
	printf("READ -> space used for the hash table: %d\n", hash_size);
	printf("READ -> remaining bytes of a dir index: %d\n", dir_idx_leftovers);
    printf("READ -> the real number of entries in a reg. file index  (also the max number of blocks): %d\n", file_idx_entries);
    printf("READ -> the real max. size in bytes of a file: %d\n", file_max_size);
    printf("READ -> the size in bytes of a single block: %d\n", block_size);

	//começar a modular...

	/*
	BYTE bitmapper[bitmap_end - bitmap_start];
	printf("%d\n", bitmap_end - bitmap_start);
	bitmapper[70] = 20;
	printf("%d\n", bitmapper[70]);
	//pq isso é possível e não tá reclamando wtf?
	bitmap[0] = 1;
	bitmap[62] = 2;
	bitmap[70] = 3;
	printf("%d\n", bitmap[0]);
	printf("%d\n", bitmap[62]);
	printf("%d\n", bitmap[10240]);// isso não deveria dar certo...
	//printf("%d\n", bitmap[10240020]);// isso dá seg fault...
	printf("\n");
	*/
	__boot_init = 1;

	return 0;
}

/*
 *the information is stored in little endian fashion in the disk, because of this
 *we need manipulate the data stored in the mbr array, in order to that in a data
 *with 2 bytes, first we take the second byte and shift it 8 bits to the left, then
 *we use the "or" operator with the first byte, creating a value with 2 bytes of info
 *in the correct order.
 *The same logic is applied in information with more than 2 bytes
*/

int mbr_info(BYTE *mbr, unsigned int *version, unsigned int *sector_bytes, unsigned int *partition_table, unsigned int *partition_count)
{
	*version = (mbr[1] << 8 | mbr[0]);
	*sector_bytes = (mbr[3] << 8 | mbr[2]);
	
	if(*sector_bytes != SECTOR_SIZE)
		return -2;

	*partition_table = (mbr[5] << 8 | mbr[4]);
	*partition_count = (mbr[7] << 8 | mbr[6]);

	return 0;
}


void read_little_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator)
{
	*dest = (source[iterator+3] << 24 | 
		     source[iterator+2] << 16 | 
			 source[iterator+1] << 8  | 
			 source[iterator]);
}

void read_big_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator)
{
	*dest = (source[iterator] << 24   | 
			 source[iterator+1] << 16 | 
			 source[iterator+2] << 8  | 
			 source[iterator+3]);
}


void global_initialization(unsigned int *information)
{
	sectors_per_block = information[0];
	root_sector = information[1];
	partition_end = information[2];
	root_block = information[3];
	entry_p_dir_blck = information[4];
	dir_idx_adrs_bytes = information[5];
	dir_idx_hash_bytes = information[6];
	dir_idx_adrs_number = information[7];
	dir_files_max = information[8];
	hash_size = information[9];
	dir_idx_leftovers = information[10];
	file_idx_entries = information[11];
	file_max_size = information[12];
	block_size = information[13];
}

void four_bytes_to_sector_array(BYTE *sector, unsigned int info, int *iterator, int position)
{
	//the loops is used to store 4 bytes of unsigned ints to 4 single bytes (to save in the disk)
	//big endian storing
	for(*iterator = *iterator; *iterator < sizeof(unsigned int) * position; (*iterator)++)
			sector[*iterator] = (info) >> (24-(8*((*iterator) - sizeof(unsigned int)*(position-1)))) & 0xFF;
	//não colocar (BYTE) antes da info... por algum motivo dá overflow (?)
}


