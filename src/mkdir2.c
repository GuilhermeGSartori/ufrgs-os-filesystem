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

int mkdir2(char *pathname)
{
	int i, j, k;
	extern _Bool __boot_init; 
	extern _Bool __root_done;
	extern BYTE *bitmap;
	extern unsigned int root_block, bitmap_end, bitmap_start;
	extern unsigned int dir_idx_adrs_bytes;
	extern unsigned int block_size, dir_files_max;
	WORD entries_used = 2;
	DIRENT2 direc_self; 
	DIRENT2 direc_father;//, direc_entry;
	int block_range_s;
	WORD free_blocks[2];


	if(__boot_init == 0)
		boot2();

	BYTE *index_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	BYTE *entries_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	HashTable ht[dir_files_max];

	if(__root_done == 1)
	{
		for(k = 0; k < 2; k++)
		{
			i = 0;
			
			while(bitmap[i] == 255 && i < (bitmap_end - bitmap_start))
				i++;

			if(i == (bitmap_end - bitmap_start))
			{
				printf("DISK IS FULL!\n");
				return -1;
			}
			//caso especial se estou na root
			//ver isso '/' é o dir atual
			//faz root de pai daí
			//ou não precisa...
			//só vê o dir pai, universal pra tudo inclusive root

			block_range_s = (i*8)+1;

			j = 0;
			while((bitmap[i] & (1 << j)) != 0)
				j++;

			free_blocks[k] = block_range_s + j;
			//write_block(BYTE *block, WORD block_number)

			bitmap[i] |= 1 << j;
		}

		//criação do diretório que estou criando agora, para s*er inserido no pai dele
		//verificável no caminho (se relativo, é o ".")
		//"." vai ter os mesmos valores que essa entrada, só vai mudar onde vai estar salva
		//e o nome. 
		//faz hash com o nome informado e coloca no pai
		//root não tem isso, pois não é uma entrada, já que não tem pai
		/*direc_entry.name[0] = '\0';//parametro
		direc_entry.fileType = 3;
		//directory.fileSize
		direc_entry.indexBlock = free_blocks[0];
		direc_entry.numberOfEntries = 2;
		//salva no pai (pelo caminho, com o nome indicado)
		//como é relativo, salva no pai que tá
		//nome também depende se é 
		direc_father.indexBlock = 0;//ver do pai
		direc_father.numberOfEntries = 0;//ver do pai*/

	}
	else
	{
		printf("ROOT NOT DONE!\n");
		free_blocks[0] = root_block;

		i = 0;
			
		while(bitmap[i] == 255 && i < (bitmap_end - bitmap_start))
			i++;

		if(i == (bitmap_end - bitmap_start))
		{
			printf("DISK IS FULL!\n");
			return -1;
		}

		block_range_s = (i*8)+1;

		j = 0;
		while((bitmap[i] & (1 << j)) != 0)
			j++;

		free_blocks[1] = block_range_s + j;

		bitmap[i] |= 1 << j;
		//falta escrever o bitmap...
		//fazer função "write_bitmap"
		//format limpa bitmap...

		//gravar bitmap em setor
		//fazer função de pegar bitmap

		direc_father.indexBlock = 0;
		direc_father.numberOfEntries = 0;

		printf("byte %d and bit %d.\n", i, j);
		printf("block number: %d\n", free_blocks[1]);
	}


	for(j = 0; j < dir_idx_adrs_bytes; j++) {
		index_block[j] = -1;
	}

	for(j = 0; j < dir_files_max; j++) {
		ht[j].name[0] = '\0';
	}

	j = 0;
	printf("writing in the index block...\n");
	two_bytes_to_bytes_array(index_block, free_blocks[1], &j);
	two_bytes_to_bytes_array(index_block, entries_used, &j);

	for(i = j; i < dir_idx_adrs_bytes; i++)
		two_bytes_to_bytes_array(index_block, -1, &j);
	//dir vai ser lido de 2 em 2 bytes até chegar na hash

	if(ht_to_bytes_array(index_block, ht, &j) != 0)
		return -1;

	write_block(index_block, free_blocks[0]);

	printf("writing in the entries block...\n");
	direc_self.name[0] = '.';
	direc_self.name[1] = '\0'; 
	direc_self.fileType = 3;
	direc_self.indexBlock = free_blocks[0];
	direc_self.numberOfEntries = 2;

	//precisa de bloco do pai... tem que ver se é root ou não...
	//se é root coloca 0, daí sabe quando tem que trocar de partição
	direc_father.name[0] = '.';
	direc_father.name[1] = '.';
	direc_father.name[2] = '\0'; 
	direc_father.fileType = 3;
	
	printf("converting entries to array...\n");
	j = 0;
	dirent_to_bytes_array(entries_block, direc_self, &j);//'.' e '..' salvos em posições fixas, sem relação à hash
	dirent_to_bytes_array(entries_block, direc_father, &j);
	printf("entries are now in the array!\n");

	for(i = sizeof(DIRENT2)*2; i < block_size; i++) {
		//printf("%d\n", i);
		entries_block[i] = (BYTE) 0;
	}

	printf("writing entries block\n");

	write_block(entries_block, free_blocks[1]);

	//não vai ser problema para arquivos regulares, pois pra saber
	//que byte utilizar, basta olhar o quanto o arquivo ocupa

	//criamos entradas 

    return 0;
}
