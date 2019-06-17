/**
 * Format a virtual disk.
 * 
 * @author Guilherme Sartori
 */
//#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/support.h"
#include <stdio.h>
#include <stdlib.h>

#define SecMBR 0

int format2(int sectors_per_block)
{
	int i, j;
	unsigned int partition_count = 0, partition_table = 0;
	unsigned int version = 0, sector_bytes = 0; 
	unsigned int i_byte_adrs = 0;
	unsigned int block_size;
	extern int partition;
	extern _Bool __root_done, __boot_init;
	BYTE *mbr = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);

	if(read_sector(SecMBR, mbr) != 0)
		return -1;

	if(mbr_info(mbr, &version, &sector_bytes, &partition_table, &partition_count) != 0)
		return -2;
	
	block_size = sectors_per_block * SECTOR_SIZE;

	//char partition_names[partition_count][24];
	unsigned int start[partition_count], end[partition_count];
	unsigned int partition_size[partition_count];

	i_byte_adrs = partition_table;
	for(i = 0; i < partition_count; i++)
	{
		read_little_DWORD(mbr, &start[i], i_byte_adrs);
		i_byte_adrs += 4;

		read_little_DWORD(mbr, &end[i], i_byte_adrs);
		i_byte_adrs += 4;

		partition_size[i] = end[i] - start[i] - 1;//we use 2 fixed sectors for the superblock, the rest is then treated as 
                                                  //blocks
		partition_size[i] /= sectors_per_block;

		//for(j = 0; j < 24; j++)
		//	partition_names[i][j] = mbr[i_byte_adrs+j];

		i_byte_adrs += 24;
	}
	
	unsigned int bitmap_start;
	unsigned int bitmap_end;
	unsigned int root_block = 1;
	unsigned int entry_p_dir_blck, dir_idx_adrs_bytes, dir_idx_hash_bytes, dir_idx_adrs_number;
	unsigned int dir_files_max, hash_size, dir_idx_leftovers;
	unsigned int file_idx_entries, file_max_size;
	
	/*directory definitions*/
	entry_p_dir_blck = block_size/sizeof(DIRENT2);//the number of file entries that a dir. block can have
	dir_idx_adrs_bytes = block_size/(16*sectors_per_block);//the number of bytes that directory block entries can ocupy in the dir index block
	dir_idx_hash_bytes = (block_size*(16*sectors_per_block - 1)) /(16*sectors_per_block);//the number of bytes that a hash can ocupy
	dir_idx_adrs_number = dir_idx_adrs_bytes/(sizeof(WORD) + sizeof(WORD));//the number of dir. blocks that a directory index can have
	dir_files_max = dir_idx_adrs_number * entry_p_dir_blck;//the max. number of files that a directory can reference in the dir. blocks
	hash_size = dir_files_max * sizeof(HashTable);//the actual space that a hash table needs to use in the index block of a directory
	dir_idx_leftovers = block_size - hash_size;//the remaining bytes of a directory index block. They are always the same as the
	                                           //max. number of files in a directory, we can maybe use this to create a "bytemap"

	/*file definitions*/
	file_idx_entries = block_size/sizeof(DWORD);//the number of entries to data blocks that a index block of a regular file can have
	file_max_size = file_idx_entries * block_size;//the max. size that a regular file can have in bytes

	/*
      *Superblock Structure:
       *The superblock is made of 2 sectors
       *First Sector:
        *the first 4 bytes are used to store the start byte of the bitmap
        *the next 4 bytes are used to store the end byte of the bitmap
        *the rest of the sector is for the bitmap itself
       *Second Sector:
        *the first 4 bytes are used to store the sectors per block information
        *the next 4 bytes are used to store the start sector of the partition (root dir) 
        *the next 4 bytes are used to store the last sector of the partition
        *the next 4 bytes are used to store the block number of the root dir (1)
        *the next 4 bytes are used to store the number of entries to file index used in a directory block
        *the next 4 bytes are used to store the number of bytes used to store address to directory blocks in the directory index block
        *the next 4 bytes are used to store the number of bytes used to store the hash table (the first bytes in the index)
        *the next 4 bytes are used to store the number of address to directory blocks in the directory index
        *the next 4 bytes are used to store the max. number of files that can be stored in a single directory;
        *the next 4 bytes are used to store the actual size of the hashtable
        *the next 4 bytes are used to store the bytes leftovers in the directory index block
        *the next 4 bytes are used to store the number of entries in a regular file index block
        *the next 4 bytes are used to store the max. size in bytes of a regular file;
        *the next 4 bytes are used to store the block size in bytes
	*/

	for(i = 0; i < partition_count; i++)
	{
		bitmap_start = 8;
		bitmap_end = (partition_size[i]/8) + bitmap_start;//511/8 = 63 e alguns quebrados... esses alguns quebrados são desconsiderados...
		BYTE *first_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
		BYTE *second_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);	

		for(j = 0; j < sizeof(BYTE) * SECTOR_SIZE; j++)
		{
			first_sector[j] = 0;
			second_sector[j] = 0;
		}

		j = 0;
		four_bytes_to_sector_array(first_sector, bitmap_start, &j);
		four_bytes_to_sector_array(first_sector, bitmap_end, &j);

		for(j = (int) bitmap_start; j < (int) bitmap_end; j++)	
		{
			if(j == bitmap_start)
				first_sector[j] = (BYTE) 1; //sets the first bit of the bitmap to 1 (byte 1 is 0000 0001 = 1...), since
			                         //the first block is used for the root itselt. The superblock space is not treated as a block
									 //1 byte (8 numbers in b) to hexa is 2. 0000 0001 becomes 01

									//colocar igual a 3? já pega o primeiro bloco de entry
			else
				first_sector[j] = (BYTE) 0; //to actually visualize the bitmap area, set this to 1, this confirms the
			                         //theory that the bitmap is correctly mapped to the number of blocks.
		}
		
		j = 0;
		four_bytes_to_sector_array(second_sector, sectors_per_block, &j);
		four_bytes_to_sector_array(second_sector, start[i]+2, &j);//start[i]+2 is the root dir first sector
		four_bytes_to_sector_array(second_sector, end[i], &j);
		four_bytes_to_sector_array(second_sector, root_block, &j);
		four_bytes_to_sector_array(second_sector, entry_p_dir_blck, &j);
		four_bytes_to_sector_array(second_sector, dir_idx_adrs_bytes, &j);
		four_bytes_to_sector_array(second_sector, dir_idx_hash_bytes, &j);
		four_bytes_to_sector_array(second_sector, dir_idx_adrs_number, &j);
		four_bytes_to_sector_array(second_sector, dir_files_max, &j);
		four_bytes_to_sector_array(second_sector, hash_size, &j);
		four_bytes_to_sector_array(second_sector, dir_idx_leftovers, &j);

		if(file_idx_entries <= partition_size[i])
		{ 
			four_bytes_to_sector_array(second_sector, file_idx_entries, &j);
			four_bytes_to_sector_array(second_sector, file_max_size, &j);
		}

		else //if the possible number of entries in a file is greater than the number of blocks in the disk, the entries 
		{    //number is set to the number of blocks in the disk

			four_bytes_to_sector_array(second_sector, partition_size[i]-1, &j);
			four_bytes_to_sector_array(second_sector, (partition_size[i]-1)*block_size, &j);
		}

		four_bytes_to_sector_array(second_sector, block_size, &j);
		
		write_sector(start[i], first_sector);//start is the sector number of the first sector of the superblock
		write_sector(start[i]+1, second_sector);

		free(first_sector);
		free(second_sector);

		printf("\n\nStarting mkdir2 for roots, partition %d\n", partition);
		__root_done = 0;
		mkdir2("root");
		printf("root done!\n");
		__boot_init = 0;
		partition++;

	}
	__root_done = 1;
	partition = 0;
	boot2();
	//open2(root)



    //debugging:
    /*HashTable ht[dir_files_max];
   	for(i = 0; i < dir_files_max; i++)
       	ht[j].name[0] = '\0';
	printf("version: %x\n", version);
	printf("number of partitions: %d\n", partition_count);
	printf("partition 1 with name %s size in blocks: %d\n", partition_names[0], (partition_size[0]/8)*8);
	printf("partition 2 with name %s size in blocks: %d\n", partition_names[1], (partition_size[1]/8)*8);
	printf("partition 3 with name %s size in blocks: %d\n", partition_names[2], (partition_size[2]/8)*8);
	printf("partition 4 with name %s size in blocks: %d\n", partition_names[3], (partition_size[3]/8)*8);
	printf("entry per block of directories: %d\n", entry_p_dir_blck);
	printf("bytes for entry blocks in the directory index: %d\n", dir_idx_adrs_bytes);
	printf("bytes for the hash table in the directory index: %d\n", dir_idx_hash_bytes);
	printf("number of entries for the DIRENT2 blocks in the index: %d\n", dir_idx_adrs_number);
	printf("max number of files in a directory: %d\n", dir_files_max);
	printf("space used for the hash table: %d\n", hash_size);
	printf("remaining bytes of a dir index: %d\n", dir_idx_leftovers);
    printf("hash size official: %d\n", sizeof(ht));
    printf("the number of entries in a reg. file index (also the max number of blocks) in theory: %d\n", file_idx_entries);
    printf("the max. size in bytes of a file in theory: %d\n", file_max_size);
    printf("A single block size: %d\n", block_size);
	printf("start: %d\n", start[0]);
	printf("end: %d\n", end[0]);    
	printf("start: %d\n", start[1]);
	printf("end: %d\n", end[1]);  
	printf("start: %d\n", start[2]);
	printf("end: %d\n", end[2]);  
	printf("start: %d\n", start[3]);
	printf("end: %d\n", end[3]);  
	printf("partition size: %d\n", partition_size[0]);   
	printf("partition size: %d\n", partition_size[1]);   
	printf("partition size: %d\n", partition_size[2]);   
	printf("partition size: %d\n", partition_size[3]);
	printf("partition 0 bitmap size: %d\n", (partition_size[0]/8));   
	printf("partition 0 bitmap end: %d\n", (partition_size[0]/8) + bitmap_start);
	*/

    return 0;
}

/* COMMENTING TO NOT AFFECT OTHER TESTS!
int main()
{
	format2(2);
	boot2();

	return 0;
}
*/