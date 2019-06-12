/**
 * Create a new directory.
 * 
 * @author Guilherme Sartori
 */
#include "../include/constants.h"
#include "../include/t2fs.h"

int mkdir2(char *pathname)
{
	int i, j;
	extern _Bool __boot_init; 
	extern _Bool __root_done;
	extern static BYTE *bitmap;
	extern unsigned int root_sector;
	extern unsigned int sectors_per_block;
	int block_range_s, block_range_e;
	int free_block, free_sector_s, free_sector_e;
	HashTable ht[dir_files_max];
   	
   	for(i = 0; i < dir_files_max; i++)
       	ht[j].name[0] = '\0';

	if(__boot_init == 0)
		boot2();


	if(__root_done == 1)
	{
		i = 0;
		
		while(bitmap[i] == 255 && i < (bitmap_end - bitmap_start))
			i++;

		if(i == (bitmap_end - bitmap_start))
		{
			printf("DISK IS FULL!\n");
			return -1;
		}

		block_range_s = (i*8)+1;
		block_range_e = (i*8)+8;

		j = 0;
		while((bitmap[i] & (1 << j)) != 0)
			j++;

		free_block = block_range_s + j;
		free_sector_s = root_sector + (sectors_per_block * (free_block - 1));
		free_sector_e = free_sector_s + sectors_per_block - 1;

		//prender no bitmap, setar = 1
		//criar entrada e salvar no dir que tá
		//(root não faz isso)
		//criar .. e . e hash
		//alocar bloco de índice, pegando o primeiro livre
		//alocar bloco de dir, pegando outro livre
		//bloco de índice preencher com entrada pra bloco de dir
		//bloco de dir preencher com entrada para .. e .

	}
	else
		//bloco pra ser alocado = 1




    return 0;
}
