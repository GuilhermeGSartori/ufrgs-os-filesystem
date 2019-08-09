/**
 * Create a new directory.
 * 
 * @author Guilherme Sartori
 */
#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/support.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mkdir2(char *pathname)
{
	int i, j, k;
	extern _Bool __boot_init; 
	extern _Bool __root_done;
	extern unsigned int root_block;
	extern unsigned int dir_idx_adrs_bytes, entry_p_dir_blck;
	extern unsigned int block_size, dir_files_max;
	extern WORD working_dir_block, temp_dir_block;
	WORD entries_used = 2;
	DIRENT2 direc_self, direc_father; 
	DIRENT2 direc_entry;
	WORD free_blocks[2], index_entry, extra_block;
	int path_size, entry_number;


	if(__boot_init == 0)
		boot2();

	BYTE *index_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	BYTE *entries_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	BYTE *father_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	BYTE *dir_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	HashTable ht[dir_files_max];

	if(__root_done == 1)
	{
		for(k = 0; k < 2; k++)
		{
			free_blocks[k] = free_block_bit();
			if(free_blocks[k] == -1)
				return -3;
	
		}


		string *path = parse_path(pathname, &path_size);
		if(path[path_size-2][0] == '.')
		{
			direc_father.indexBlock = working_dir_block;//ver do pai
		}
		else
		{
			temp_dir_block = working_dir_block;
			for(i = 1; i < path_size-1; i++)
			{
				if(find_target_dir(path[i]) != 0)
				{
					printf("path invalido '%s' nao existe!\n", path[i]);
					return -1;
				}
			}

			direc_father.indexBlock = temp_dir_block;

		}

		read_block(father_block, direc_father.indexBlock);
		read_block(dir_block, father_block[0] << 8 | father_block[1]);
		j = 0;
		read_entry(dir_block, &direc_father, &j);
		direc_father.numberOfEntries++;
		j = 0;
		dirent_to_bytes_array(dir_block, direc_father, &j);
		write_block(dir_block, father_block[0] << 8 | father_block[1]);


		strcpy(direc_entry.name, path[path_size-1]);
		direc_entry.fileType = 3;
		direc_entry.indexBlock = free_blocks[0];
		direc_entry.numberOfEntries = 2;
		entry_number = get_entry_number(direc_entry.name, direc_father.numberOfEntries); 
		index_entry = father_block[(entry_number/entry_p_dir_blck) * 4] << 8 | father_block[((entry_number/entry_p_dir_blck) * 4)+1];
		
		if(index_entry == 65535)//unsigned word -1
		{
			extra_block = free_block_bit();
			j = (entry_number/entry_p_dir_blck) * 4;
			//printf("extra allocated block: %d, byte position: %d \n", extra_block, j);//33
			two_bytes_to_bytes_array(father_block, extra_block, &j);
			two_bytes_to_bytes_array(father_block, 1, &j);
			write_block(father_block, direc_father.indexBlock);
			
			read_block(dir_block, extra_block);
			j = 0;
			dirent_to_bytes_array(dir_block, direc_entry, &j);
			for(i = sizeof(DIRENT2); i < block_size; i++) {
				//printf("%d\n", i);
				dir_block[i] = (BYTE) 0;
			}

			write_block(dir_block, extra_block);
		}

		else
		{
			read_block(dir_block, index_entry);
			j = sizeof(DIRENT2) * (entry_number - entry_p_dir_blck*(entry_number/entry_p_dir_blck));
			dirent_to_bytes_array(dir_block, direc_entry, &j);
			write_block(dir_block, index_entry);
		}


	}
	else
	{
		free_blocks[0] = root_block;
		free_blocks[1] = free_block_bit();
		if(free_blocks[1] == -1)
			return -3;

		direc_father.indexBlock = 0;
		direc_father.numberOfEntries = 0;
	}


	for(j = 0; j < dir_idx_adrs_bytes; j++) {
		index_block[j] = -1;
	}

	for(j = 0; j < dir_files_max; j++) {
		if(j == 0)
		{
			ht[j].name[0] = '\0';
			ht[j].block = free_blocks[0];
		}
		else if(j == 1)
		{
			ht[j].name[0] = '\0';
			ht[j].block = direc_father.indexBlock;
		}
		else
		{
			ht[j].name[0] = '\0';
			ht[j].block = 0;
		}
	}

	j = 0;
	two_bytes_to_bytes_array(index_block, free_blocks[1], &j);
	two_bytes_to_bytes_array(index_block, entries_used, &j);

	for(i = j; i < dir_idx_adrs_bytes; i++)
		two_bytes_to_bytes_array(index_block, -1, &j);
	//dir vai ser lido de 2 em 2 bytes até chegar na hash

	if(ht_to_bytes_array(index_block, ht, &j) != 0)
		return -1;

	write_block(index_block, free_blocks[0]);

	direc_self.name[0] = '.';
	direc_self.name[1] = '\0'; 
	direc_self.fileType = 3;
	direc_self.indexBlock = free_blocks[0];
	direc_self.numberOfEntries = 2;

	direc_father.name[0] = '.';
	direc_father.name[1] = '.';
	direc_father.name[2] = '\0'; 
	direc_father.fileType = 3;

	j = 0;
	dirent_to_bytes_array(entries_block, direc_self, &j);//'.' e '..' salvos em posições fixas, sem relação à hash
	dirent_to_bytes_array(entries_block, direc_father, &j);

	for(i = sizeof(DIRENT2)*2; i < block_size; i++) {
		//printf("%d\n", i);
		entries_block[i] = (BYTE) 0;
	}


	write_block(entries_block, free_blocks[1]);

	//BYTE *test_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	//read_block(test_block, working_dir_block);
	//list_entries(test_block);

    return 0;
}
