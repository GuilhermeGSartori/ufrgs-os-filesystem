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
#include <string.h>

#define SecMBR 0

int format2(int sectors_per_block)
{
	int i, j;
	unsigned int partition_count = 0, partition_table = 0;
	unsigned int version = 0, sector_bytes = 0; 
	unsigned int i_byte_adrs = 0;
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
		printf("%d\n", partition_size[i]);       //blocks
		partition_size[i] /= sectors_per_block;
		printf("start: %d\n", start[i]);
		printf("end: %d\n", end[i]);

		for(j = 0; j < 24; j++)
			partition_names[i][j] = mbr[i_byte_adrs+j];

		i_byte_adrs += 24;
	}

	unsigned int bitmap_start;
	unsigned int bitmap_end;

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
        *armazenar entrada do dir raiz aqui! (bloco 1 ué)
        *armazenar tamanho de um hash
        *armazenar divisoes de um dir e tals, quanto ocupa e informações e o q é o q
	*/
	//boot lê o superbloco e armazena em globais as infos
	//em um diretório vão caber 16 ou mais arquivos(tamanho bloco/32), depende da hash tambéḿ já que ela ocupa espaço. 
	//Cada entrada vai ter 
	//32 bytes -> posição do índice vai ser 4 bytes; 1 tipo; 4 tamanho; nome 22(23) bytes
	//tamanho da entrada é verificável ao dar sizeof na struct...  Acessar dados é só acessar dados de struct...
	//SÓ SE NÃO PODE MUDAR, DAÍ EU VOU PRECISAR DO TAMANHO, PRA SABER ONDE FICAR OS
	//BYTES DE ÍNDICE

	//format vai criar todos os roots mas te dropa no root da partição 0

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

		write_sector(start[i]+1, second_sector);

		free(first_sector);
		free(second_sector);

		HashTable ht;

		printf("aa %d\n", sizeof(element));
		for(j = 0; j < 32; j++)
		{

        	ht.entries[j] = malloc(sizeof(element));
        	strcpy(ht.entries[j]->name, "1234567890");
            printf("Size: %d\n", sizeof(ht.entries[j]));
            printf("bb %d\n", sizeof(ht.entries[j]->name));
    	}

    	//mesma logica do hash, calcula indice pelo nome e coloca nele...
    	//nao faz por ponteiros..
    	HashTable2 htt[32];
   		for(j = 0; j < 32; j++)
		{
        	strcpy(htt[j].name, "1");
            printf("Size2: %d\n", sizeof(htt));
            printf("bb2 %d\n", sizeof(htt[j].name));
    	}	

		//root allocation
			//hash for root directories allocation
		//basicamente pra root pego o bloco (bitmap e tudo) e faço o mkdir primal e pego
		//e faço a estrutura hash dele e gg? root pronto? só poder receber um open que já era?
		//salvo zeros nas entradas e deu?
	}
	//testar pra ver se os dados tão sendo salvos de maneira correta, se vou conseguir ler depois
	//nas posições que eu acho q está pra abstrair as infos e tals...
	//basicamente fazer a função de boot
	//mas primeiro, tenho que fazer uma função de hash e fazer o diretório raiz!
	//mas antes do raiz, tenho que definir o tamanho das estradas e terminar de salvar as coisas
	//do superbloco!


	//debugging:
	printf("version: %x\n", version);
	printf("number of partitions: %d\n", partition_count);
	printf("partition 1 with name %s size in blocks: %d\n", partition_names[0], (partition_size[0]/8)*8);
	printf("partition 2 with name %s size in blocks: %d\n", partition_names[1], (partition_size[1]/8)*8);
	printf("partition 3 with name %s size in blocks: %d\n", partition_names[2], (partition_size[2]/8)*8);
	printf("partition 4 with name %s size in blocks: %d\n", partition_names[3], (partition_size[3]/8)*8);

    return 0;
}

int main()
{
	format2(2);

	return 0;
}
