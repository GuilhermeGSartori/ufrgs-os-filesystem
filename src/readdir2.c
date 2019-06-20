/**
 * Read entries in the directory.
 * 
 * @author Guilherme Sartori
 */
#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/support.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/constants.h"
#include <string.h>

extern _Bool __boot_init;

int readdir2(DIR2 handle, DIRENT2 *dentry)
{
	if(__boot_init == 0)
		boot2();
	
	extern OpenFile openDirs[MAX_OPEN_DIRS];
	extern unsigned int dir_files_max, entry_p_dir_blck, block_size;
	int index_entry, /*index_count,*/ j, i;

	BYTE *entry_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	BYTE *dir_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	DIRENT2 entry;

	read_block(dir_block, openDirs[handle].record.indexBlock);

	while(openDirs[handle].currentPointer < dir_files_max)
	{
		//é o bloco root, primeiro de entries está salvo no 2 sim
		//tenho que atualizar index_entry...
		//entre blocos, não da dirent
		index_entry = dir_block[(openDirs[handle].currentPointer/entry_p_dir_blck) * 4] << 8 | dir_block[((openDirs[handle].currentPointer/entry_p_dir_blck) * 4)+1];
		//index_count = block[((openDirs[handle].currentPointer/entry_p_dir_blck) * 4)+2] << 8 | block[((openDirs[handle].currentPointer/entry_p_dir_blck) * 4)+3];


		if(index_entry == 65535)//unsigned word -1
		{
			//printf("entry vazia\n");
			openDirs[handle].currentPointer += entry_p_dir_blck;
		}
		else
		{
			read_block(entry_block, index_entry);
			j = 0;
			for(i = 0; i < entry_p_dir_blck; i++)
			{
				//printf("i: %d\n", i);
				read_entry(entry_block, &entry, &j);
				if(entry.fileType != 0 && i >= openDirs[handle].currentEntry)
				{
					strcpy(dentry->name, entry.name);
					dentry->fileType = entry.fileType;
					dentry->indexBlock = entry.indexBlock;
					if((strcmp(entry.name, "..\0") != 0) && entry.fileType == 3)
						dentry->numberOfEntries = entry.numberOfEntries;	
					openDirs[handle].currentEntry = i + 1;
					return 0;
				}
			}
			openDirs[handle].currentEntry = 0;
			openDirs[handle].currentPointer += entry_p_dir_blck;
		}

	}
	return -1;

}
