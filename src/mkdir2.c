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
	extern unsigned int dir_idx_adrs_bytes;
	extern unsigned int block_size, dir_files_max;
	extern char working_dir_path[10000];
	extern WORD working_dir_block;
	WORD entries_used = 2;
	DIRENT2 direc_self, direc_father; 
	DIRENT2 direc_entry;
	WORD free_blocks[2];
	int path_size;


	if(__boot_init == 0)
		boot2();

	BYTE *index_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	BYTE *entries_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	HashTable ht[dir_files_max];

	if(__root_done == 1)
	{
		for(k = 0; k < 2; k++)
		{
			free_blocks[k] = free_block_bit();
			if(free_blocks[k] == -1)
				return -3;
		
			//caso especial se estou na root
			//ver isso '/' é o dir atual
			//faz root de pai daí
			//ou não precisa...
			//só vê o dir pai, universal pra tudo inclusive root
		}

		//criação do diretório que estou criando agora, para s*er inserido no pai dele
		//verificável no caminho (se relativo, é o ".")
		//"." vai ter os mesmos valores que essa entrada, só vai mudar onde vai estar salva
		//e o nome. 
		//faz hash com o nome informado e coloca no pai
		//root não tem isso, pois não é uma entrada, já que não tem pai

		string *path = parse_path(pathname, &path_size);
		if(path[path_size-2][0] == '.')
		{
			direc_father.indexBlock = working_dir_block;//ver do pai
			printf("bloco do pai: %d\n", direc_father.indexBlock);
			//salva direto onde tá com working_dir
			//string *working_path = parse_path(working_dir_path, &path_size);
			//strcpy(direc_entry.name, working_path[path_size-1]);
			//vamos começar assim
			//salvando localmente, só no primeiro nível
			//tenho que pegar o bloco, ler o bloco, salvar a entrada na hash e deu

		}
		//else
			//strcpy(direc_entry.name, path[path_size-2]);

		strcpy(direc_entry.name, path[path_size-1]);
		//direc_entry salvo no pai!
		printf("nome do novo dir: %s\n", direc_entry.name);
		direc_entry.fileType = 3;
		//directory.fileSize
		direc_entry.indexBlock = free_blocks[0];
		direc_entry.numberOfEntries = 2;
		//salva no pai (pelo caminho, com o nome indicado)
		//como é relativo, salva no pai que tá
		//nome também depende se é 
		direc_father.numberOfEntries = 0;//ver do pai
										 //tem que ver pq ao atualizar tem que salvar na dele também

	}
	else
	{
		printf("ROOT NOT DONE!\n");
		free_blocks[0] = root_block;
		free_blocks[1] = free_block_bit();
		if(free_blocks[1] == -1)
			return -3;

		direc_father.indexBlock = 0;
		direc_father.numberOfEntries = 0;

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
