/**
 * Format a virtual disk.
 * 
 * @author Guilherme Sartori
 */
//#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/apidisk.h"
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
	BYTE *mbr = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);

	if(read_sector(SecMBR, mbr) != 0)
		return -1;

	/*
     *the information is stored in little endian fashion in the disk, because of this
	 *we need manipulate the data stored in the mbr array, in order to that in a data
	 *with 2 bytes, first we take the second byte and shift it 8 bits to the left, then
	 *we use the "or" operator with the first byte, creating a value with 2 bytes of info
	 *in the correct order.
	 *The same logic is applied in information with more than 2 bytes
	*/

	//trasformar toda essa porra em uma função de ler dados..
	version = (mbr[1] << 8 | mbr[0]);
	sector_bytes = (mbr[3] << 8 | mbr[2]);
	
	if(sector_bytes != SECTOR_SIZE)
		return -2;
	block_size = sectors_per_block * SECTOR_SIZE;

	partition_table = (mbr[5] << 8 | mbr[4]);
	partition_count = (mbr[7] << 8 | mbr[6]);


	char partition_names[partition_count][24];
	unsigned int start[partition_count], end[partition_count];
	unsigned int partition_size[partition_count];

	i_byte_adrs = partition_table;
	for(i = 0; i < partition_count; i++)
	{

		start[i] = (mbr[i_byte_adrs+3] << 24 | 
			        mbr[i_byte_adrs+2] << 16 | 
			        mbr[i_byte_adrs+1] << 8  | 
			        mbr[i_byte_adrs]);
		i_byte_adrs += 4;

		end[i] = (mbr[i_byte_adrs+3] << 24 | 
			      mbr[i_byte_adrs+2] << 16 | 
			      mbr[i_byte_adrs+1] << 8  | 
			      mbr[i_byte_adrs]);
		i_byte_adrs += 4;

		partition_size[i] = end[i] - start[i] -1;//we use 2 fixed sectors for the superblock, the rest is then treated as 
                                                 //blocks
		partition_size[i] /= sectors_per_block;


		for(j = 0; j < 24; j++)
			partition_names[i][j] = mbr[i_byte_adrs+j];

		i_byte_adrs += 24;
	}
	
	unsigned int bitmap_start;
	unsigned int bitmap_end;
	unsigned int root_block = 1;

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
	*/
	//boot lê o superbloco e armazena em globais as infos
	//em um diretório vão caber 16 ou mais arquivos(tamanho bloco/32), depende da hash tambéḿ já que ela ocupa espaço. 
	//Cada entrada vai ter 
	//32 bytes -> posição do índice vai ser 4 bytes; 1 tipo; 4 tamanho; nome 22(23) bytes
	//tamanho da entrada é verificável ao dar sizeof na struct...  Acessar dados é só acessar dados de struct...

	//format vai criar todos os roots mas te dropa no root da partição 0

	unsigned int entry_p_dir_blck, dir_idx_adrs_bytes, dir_idx_hash_bytes, dir_idx_adrs_number;
	unsigned int dir_files_max, hash_size, dir_idx_leftovers;
	unsigned int file_idx_entries, file_max_size;
	
	//directory definitions
	entry_p_dir_blck = block_size/sizeof(DIRENT2);
	dir_idx_adrs_bytes = block_size/(16*sectors_per_block);
	dir_idx_hash_bytes = (block_size*(16*sectors_per_block - 1)) /(16*sectors_per_block);
	dir_idx_adrs_number = dir_idx_adrs_bytes/(sizeof(WORD) + sizeof(WORD));
	dir_files_max = dir_idx_adrs_number * entry_p_dir_blck;
	hash_size = dir_files_max * sizeof(HashTable);
	dir_idx_leftovers = block_size - hash_size;

	//file definitions
	file_idx_entries = block_size/sizeof(DWORD);
	file_max_size = file_idx_entries * block_size;

	//o que sobra é sempre o numero de arquivos... fazer tipo um bitmap...
	//1 byte por arquivo...
	//armazenamos isso no setor 2 também

    //mesma logica do hash, calcula indice pelo nome e coloca nele...
    //nao faz por ponteiros..
    HashTable ht[dir_files_max];
   	
   	for(i = 0; i < dir_files_max; i++)
       	ht[j].name[0] = '\0';
    

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

		for(j = 0; j < sizeof(unsigned int); j++)
			first_sector[j] = (BYTE) bitmap_start >> (24-(8*j)) & 0xFF;

		for(j = j; j < sizeof(unsigned int) * 2; j++)
			first_sector[j] = (BYTE) bitmap_end >> (24-(8*(j-4))) & 0xFF;

		for(j = (int) bitmap_start; j < (int) bitmap_end; j++)	
		{
			if(j == bitmap_start)
				first_sector[j] = 1; //sets the first bit of the bitmap to 1 (byte 1 is 0000 0001 = 1...), since
			                         //the first block is used for the root itselt. The superblock space is not treated as a block
									 //1 byte (8 numbers in b) to hexa is 2. 0000 0001 becomes 01
			else
				first_sector[j] = 0; //to actually visualize the bitmap area, set this to 1, this confirms the
			                         //theory that the bitmap is correctly mapped to the number of blocks.
		}
		//a cada 16 linhas no disco fecha um setor!, cada xxxx são 2 bytes em binário
		//a pilha é que o MBR é no zero, e o star das partições começam no lugar certo
		//meter isso na função de write blocks dps

		write_sector(start[i], first_sector);

		for(j = 0; j < sizeof(unsigned int); j++)//big endian storing
			second_sector[j] = sectors_per_block >> 8*j;

		for(j = j; j < sizeof(unsigned int) * 2; j++)
			second_sector[j] = (start[i]+2) >> 8*(j-sizeof(unsigned int));//start[i]+2 é onde o raiz está salvo...

		for(j = j; j < sizeof(unsigned int) * 3; j++)
			second_sector[j] = end[i] >> 8*(j-sizeof(unsigned int)*2);

		for(j = j; j < sizeof(unsigned int) * 4; j++)
			second_sector[j] = root_block >> 8*(j-sizeof(unsigned int)*3);

		for(j = j; j < sizeof(unsigned int) * 5; j++)
			second_sector[j] = entry_p_dir_blck >> 8*(j-sizeof(unsigned int)*4);

		for(j = j; j < sizeof(unsigned int) * 6; j++)
			second_sector[j] = dir_idx_adrs_bytes >> 8*(j-sizeof(unsigned int)*5);	

		for(j = j; j < sizeof(unsigned int) * 7; j++)
			second_sector[j] = dir_idx_hash_bytes >> 8*(j-sizeof(unsigned int)*6);	

		for(j = j; j < sizeof(unsigned int) * 8; j++)
			second_sector[j] = dir_idx_adrs_number >> 8*(j-sizeof(unsigned int)*7);

		for(j = j; j < sizeof(unsigned int) * 9; j++)
			second_sector[j] = dir_files_max >> 8*(j-sizeof(unsigned int)*8);

		for(j = j; j < sizeof(unsigned int) * 10; j++)
			second_sector[j] = hash_size >> 8*(j-sizeof(unsigned int)*9);

		for(j = j; j < sizeof(unsigned int) * 11; j++)
			second_sector[j] = dir_idx_leftovers >> 8*(j-sizeof(unsigned int)*10);


		//alem disso tem que verificar se tem blocos suficientes para arquivo crescer...
		//quando for adicionar dados e etc... 
		if(file_idx_entries <= partition_size[i])
		{ 
			for(j = j; j < sizeof(unsigned int) * 12; j++)
				second_sector[j] = file_idx_entries >> 8*(j-sizeof(unsigned int)*11);

			for(j = j; j < sizeof(unsigned int) * 13; j++)
				second_sector[j] = file_max_size >> 8*(j-sizeof(unsigned int)*12);
		}

		else
		{

			for(j = j; j < sizeof(unsigned int) * 12; j++)
				second_sector[j] = (partition_size[i]-1) >> 8*(j-sizeof(unsigned int)*11);
			for(j = j; j < sizeof(unsigned int) * 13; j++)
				second_sector[j] = ((partition_size[i]-1)*block_size) >> 8*(j-sizeof(unsigned int)*12);
		}


		write_sector(start[i]+1, second_sector);

		free(first_sector);
		free(second_sector);


		//root allocation
			//hash for root directories allocation
		//basicamente pra root pego o bloco (bitmap e tudo) e faço o mkdir literal! e pego
		//e faço a estrutura hash dele e gg? root pronto? só poder receber um open que já era?
		//salvo zeros nas entradas e deu?
	}
	//testar pra ver se os dados tão sendo salvos de maneira correta, se vou conseguir ler depois
	//nas posições que eu acho q está pra abstrair as infos e tals...
	//basicamente fazer a função de boot
	//mas primeiro, tenho que fazer uma função de hash e fazer o diretório raiz!
	//mas antes do raiz, tenho que definir o tamanho das estradas e terminar de salvar as coisas
	//do superbloco!


	//fazer função format_info que é basicamente isso:
	//debugging:
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
	printf("necessary space for the hash table: %d\n", hash_size);
	printf("remaining bytes of a dir index: %d\n", dir_idx_leftovers);
    printf("hash size official: %d\n", sizeof(ht));
    printf("the number of entries in a reg. file index (also the max number of blocks) in theory: %d\n", file_idx_entries);
    printf("the max. size in bytes of file in theory: %d\n", file_max_size);
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

    return 0;
}

int main()
{
	format2(2);

	return 0;
}
