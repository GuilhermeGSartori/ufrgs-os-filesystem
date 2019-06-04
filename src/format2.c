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
	BYTE *mbr = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);

	if(read_sector(SecMBR, mbr) != 0)
		return -1;
	if(sectors_per_block < 2)
	{
		//PERGUNTAR PRO CARISSIMI ISSO, SE NÃO TEM QUE REVER O SUPERBLOCO!
		printf("this file system only works with blocks with more than one sector!\n");
		return -2;
	}

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
		return -3;
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

		partition_size[i] = end[i] - start[i] + 1;
		partition_size[i] /= sectors_per_block;

		for(j = 0; j < 24; j++)
			partition_names[i][j] = mbr[i_byte_adrs+j];

		i_byte_adrs += 24;
	}

	unsigned int bitmap_start;
	unsigned int bitmap_end;
	
	for(i = 0; i < partition_count; i++)
	{
		bitmap_start = 8;
		bitmap_end = (partition_size[i]/8) + bitmap_start;
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
			first_sector[j] = 0;
		//meter isso na função de write blocks dps
		//tô considerando que o write traduz pra hexa... se não, eu vou ter
		//q rever isso tudo... deve ser coisa do arquivo na real, nao existe
		//isso de traduzir, dado é dado, porra, é só questão de visualização
		write_sector(start[i], first_sector);

		for(j = 0; j < sizeof(unsigned int); j++)
			second_sector[j] = sectors_per_block >> 8*j;

		for(j = j; j < sizeof(unsigned int) * 2; j++)
			second_sector[j] = (start[i]+2) >> 8*(j-4);

		for(j = j; j < sizeof(unsigned int) * 3; j++)
			second_sector[j] = end[i] >> 8*(j-8); //rever isso do j, por enquanto deixa assim...

		write_sector(start[i]+1, second_sector);

		free(first_sector);
		free(second_sector);
	}
	//fazer estrutura do superbloco
	//nele vou armazenar onde tá o bitmap, o bitmap, tamanho dos blocos,
	//onde começa de fato (start, onde tá raiz), onde termina.

	//salvo dados no disco em high endian, caguei

	//isso vai ficar dentro de um loop maior de super bloco, na real
	//onde vou armazenar no disco os bitmaps? e onde fica a informação de onde achar eles?
	//e um bitmap não ocupa todo um setor, como lidar com isso?


	//root allocation
		//hash for root directories allocation

	//loop do bitmap, aloca quantidade de blocos pra partição, grava no disco
	//da free
	//unsigned int bitmap[];

	//debugging:
	printf("version: %x\n", version);
	printf("number of partitions: %d\n", partition_count);
	printf("partition 1 with name %s size in blocks: %d\n", partition_names[0], partition_size[0]);
	printf("partition 2 with name %s size in blocks: %d\n", partition_names[1], partition_size[1]);
	printf("partition 3 with name %s size in blocks: %d\n", partition_names[2], partition_size[2]);
	printf("partition 4 with name %s size in blocks: %d\n", partition_names[3], partition_size[3]);

    return 0;
}

int main()
{
	format2(2);

	return 0;
}