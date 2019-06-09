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
unsigned int file_idx_entries = 0, file_max_size = 0;
unsigned int information[13];
static BYTE *bitmap;


#define SecMBR 0

int boot2()
{
	int i;
	unsigned int first_sector = 0;
	unsigned int second_sector = 0;
	unsigned int partition_count = 0, partition_table = 0;
	unsigned int sector_bytes = 0, i_byte_adrs = 0;

	BYTE *bitmap_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
	BYTE *info_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
	BYTE *mbr = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);

	if(read_sector(SecMBR, mbr) != 0)
		return -1;

	sector_bytes = (mbr[3] << 8 | mbr[2]);
	partition_table = (mbr[5] << 8 | mbr[4]);
	partition_count = (mbr[7] << 8 | mbr[6]);

	if(sector_bytes != SECTOR_SIZE)
		return -2;

	unsigned int start[partition_count];

	i_byte_adrs = partition_table;
	for(i = 0; i < partition_count; i++)
	{

		start[i] = (mbr[i_byte_adrs+3] << 24 | 
			        mbr[i_byte_adrs+2] << 16 | 
			        mbr[i_byte_adrs+1] << 8  | 
			        mbr[i_byte_adrs]);
		i_byte_adrs += 32;
	}

	first_sector = start[partition];
	second_sector = start[partition] + 1;

	if(read_sector(first_sector, bitmap_sector) != 0)
		return -2;

	if(read_sector(second_sector, info_sector) != 0)
		return -3;


	i_byte_adrs = 0;
	bitmap_start = (bitmap_sector[i_byte_adrs] << 24 | 
			        bitmap_sector[i_byte_adrs+1] << 16 | 
			        bitmap_sector[i_byte_adrs+2] << 8  | 
			        bitmap_sector[i_byte_adrs+3]);
	i_byte_adrs += 4;

	bitmap_end =   (bitmap_sector[i_byte_adrs] << 24 | 
			        bitmap_sector[i_byte_adrs+1] << 16 | 
			        bitmap_sector[i_byte_adrs+2] << 8  | 
			        bitmap_sector[i_byte_adrs+3]);
	i_byte_adrs += 4;



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
	for(i = 0; i < 13; i++)
	{
		information[i] = (info_sector[i_byte_adrs] << 24 | 
				          info_sector[i_byte_adrs+1] << 16 | 
				          info_sector[i_byte_adrs+2] << 8  | 
			    	      info_sector[i_byte_adrs+3]);	
		i_byte_adrs += 4;
	}

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