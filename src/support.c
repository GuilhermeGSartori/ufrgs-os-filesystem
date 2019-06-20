/**
 * Support functions for T2FS library.
 * 
 * @author Guilherme Sartori
 * @author Marlize Ramos
 * @author Renan Kummer
 */
#include "../include/support.h"
#include <string.h>

/*
 * Global Variables for Boot Declaration
 * When used, they need to be declares in the files
 * that require them. Using "extern <type> <name>"
 * put then in the global variables files... or not...
*/

extern _Bool __boot_init;
extern _Bool __root_done;
extern int partition;
extern unsigned int bitmap_start, bitmap_end, superblock_start, number_of_blocks;
extern unsigned int sectors_per_block, root_sector, partition_end, root_block, entry_p_dir_blck, dir_idx_adrs_bytes;
extern unsigned int dir_idx_hash_bytes, dir_idx_adrs_number, dir_files_max, hash_size, dir_idx_leftovers;
extern unsigned int file_idx_entries, file_max_size, block_size;
extern unsigned int information[16];
extern BYTE *bitmap;


#define SecMBR 0

int boot2()
{
	int i;
	unsigned int first_sector = 0;
	unsigned int second_sector = 0;
	unsigned int partition_count = 0, partition_table = 0;
	unsigned int version = 0, sector_bytes = 0, i_byte_adrs = 0;

	BYTE *bitmap_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
	BYTE *info_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
	BYTE *mbr = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);

	if(read_sector(SecMBR, mbr) != 0)
		return -1;

	if(mbr_info(mbr, &version, &sector_bytes, &partition_table, &partition_count) != 0)
		return -2;

	unsigned int start[partition_count];

	i_byte_adrs = partition_table;
	for(i = 0; i < partition_count; i++)
	{
		read_little_DWORD(mbr, &start[i], i_byte_adrs);
		i_byte_adrs += 32;
	}

	first_sector = start[partition];
	second_sector = start[partition] + 1;

	if(read_sector(first_sector, bitmap_sector) != 0)
		return -2;

	if(read_sector(second_sector, info_sector) != 0)
		return -3;


	i_byte_adrs = 0;
	read_big_DWORD(bitmap_sector, &bitmap_start, i_byte_adrs);
	i_byte_adrs += 4;
	read_big_DWORD(bitmap_sector, &bitmap_end, i_byte_adrs);

	//printf("READ -> Bitmap start: %d\n", bitmap_start);
	//printf("READ -> Bitmap end: %d\n", bitmap_end);

	bitmap = (BYTE *) malloc(sizeof(BYTE )*(bitmap_end - bitmap_start));

	//printf("bitmap size: %d\n", sizeof(*bitmap));
	//printf("READ -> Bitmap: \n");
	i_byte_adrs = bitmap_start;

	for(i = 0; i < bitmap_end - bitmap_start; i++)
	{
		bitmap[i] = bitmap_sector[i_byte_adrs];
		//printf("	Bitmap value byte %d: %d \n",i, bitmap[i]);
		i_byte_adrs++;
	}

	i_byte_adrs = 0;
	for(i = 0; i < 16; i++)
	{
		read_big_DWORD(info_sector, &information[i], i_byte_adrs);
		i_byte_adrs += 4;
	}

	global_initialization(information);

	//printf("READ -> sectors per block: %d\n", sectors_per_block);
	//printf("READ -> root sector: %d\n", root_sector);
	//printf("READ -> partition end: %d\n", partition_end);
	//printf("READ -> root block: %d\n", root_block);
	//printf("READ -> entry per block of directories: %d\n", entry_p_dir_blck);
	//printf("READ -> bytes for entry blocks in the directory index: %d\n", dir_idx_adrs_bytes);
	//printf("READ -> bytes for the hash table in the directory index: %d\n", dir_idx_hash_bytes);
	//printf("READ -> number of entries for the DIRENT2 blocks in the index: %d\n", dir_idx_adrs_number);
	//printf("READ -> max number of files in a directory: %d\n", dir_files_max);
	//printf("READ -> space used for the hash table: %d\n", hash_size);
	//printf("READ -> remaining bytes of a dir index: %d\n", dir_idx_leftovers);
    //printf("READ -> the real number of entries in a reg. file index  (also the max number of blocks): %d\n", file_idx_entries);
    //printf("READ -> the real max. size in bytes of a file: %d\n", file_max_size);
    //printf("READ -> the size in bytes of a single block: %d\n", block_size);
    printf("READ -> superblock start: %d\n", superblock_start);
    printf("READ -> number_of_blocks: %d\n", number_of_blocks);

	//colocar o brother no root 0, sempre que fizer boot, troca de partição
	//e isso acontece quando faz cd .. estando no root
	//ao trocar, incrementa o número da partição e faz o boot
	//format chama o boot, como global tá em zero, vai colocar em zero...
	//format só faz os diretórios, boot te coloca no dir
	//format pra fazer o raiz, mete boot_done = 1, depois de tudo mete = 0
	//pra não fazer o boot...
	//boot vem depois
	//no final da format

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

/*
 *the information is stored in little endian fashion in the disk, because of this
 *we need manipulate the data stored in the mbr array, in order to that in a data
 *with 2 bytes, first we take the second byte and shift it 8 bits to the left, then
 *we use the "or" operator with the first byte, creating a value with 2 bytes of info
 *in the correct order.
 *The same logic is applied in information with more than 2 bytes
*/

int mbr_info(BYTE *mbr, unsigned int *version, unsigned int *sector_bytes, unsigned int *partition_table, unsigned int *partition_count)
{
	*version = (mbr[1] << 8 | mbr[0]);
	*sector_bytes = (mbr[3] << 8 | mbr[2]);
	
	if(*sector_bytes != SECTOR_SIZE)
		return -2;

	*partition_table = (mbr[5] << 8 | mbr[4]);
	*partition_count = (mbr[7] << 8 | mbr[6]);

	return 0;
}


void read_little_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator)
{
	*dest = (source[iterator+3] << 24 | 
		     source[iterator+2] << 16 | 
			 source[iterator+1] << 8  | 
			 source[iterator]);
}

void read_big_DWORD(BYTE *source, unsigned int *dest, unsigned int iterator)
{
	*dest = (source[iterator] << 24   | 
			 source[iterator+1] << 16 | 
			 source[iterator+2] << 8  | 
			 source[iterator+3]);
}


void global_initialization(unsigned int *information)
{
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
	block_size = information[13];
	superblock_start = information[14];
	number_of_blocks = information[15];
}


void four_bytes_to_sector_array(BYTE *sector, unsigned int info, int *iterator)
{
	//the loop is used to store 4 bytes of unsigned ints to 4 single bytes (to save in the disk)
	//big endian storing
	int i = 0;
	int j = (*iterator);
	for(*iterator = *iterator; *iterator < j + sizeof(unsigned int); (*iterator)++)
	{
		sector[*iterator] = (info) >> (24-(8*i)) & 0xFF;
		i++;
	}
	//não colocar (BYTE) antes da info... por algum motivo dá overflow (?)
}


void two_bytes_to_bytes_array(BYTE *array, short int info, int *iterator)
{
	//the loops is used to store 4 bytes of unsigned ints to 4 single bytes (to save in the disk)
	//big endian storing
	int i = 0;
	int j = (*iterator);
	for(*iterator = *iterator; *iterator < j + sizeof(short int); (*iterator)++)
	{
			array[*iterator] = (info) >> (8-(8*i)) & 0xFF;
			i++;
	}
	//não colocar (BYTE) antes da info... por algum motivo dá overflow (?)
}

int ht_to_bytes_array(BYTE *array, HashTable *ht, int *iterator)
{
	int i, k;
	HashTable size;
	size.name[0] = '\0';
	//é para a hash table ter sempre o tamanho menor que o definido
	//pelo separação de bytes, preencho o necessário e depois, o que sobrar
	//preencho com -1, daí tu sabe que isso NÃO faz parte da hash
	//ht SEMPRE depois das entradas pra blocos de dir

	if(sizeof(ht) > dir_idx_hash_bytes)
		return -1;
	for(i = 0; i < dir_files_max; i++)
	{
		for(k = 0; k < sizeof(size.name); k++)// ou só mete 7 e foda-se
		{
			//printf("%d ", k);
			array[*iterator] = ht[i].name[k];
			(*iterator)++;
		}
		two_bytes_to_bytes_array(array, ht[i].block, iterator);
	}

	for(*iterator = *iterator; *iterator < block_size; (*iterator)++)
		array[*iterator] = -1;



	return 0;
}

int write_block(BYTE *block, WORD block_number)
{
	//if (block_number > number_of_blocks || block_number < 0)
	//	return T2FS_INVALID_BLOCK_NUMBER;

	int free_sector_s;
	int i, j;

	if(block_number == root_block)
		free_sector_s = root_sector;
	
	else
		free_sector_s = root_sector + (sectors_per_block * (block_number - 1));

	for(i = 0; i < sectors_per_block; i++)
	{
		BYTE *sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);
		for(j = 0; j < SECTOR_SIZE; j++) 
		{
			sector[j] = block[j+(SECTOR_SIZE*i)];
		}
		write_sector(free_sector_s+i, sector);
		free(sector);
	}

	return 0;
}

void dirent_to_bytes_array(BYTE *block, DIRENT2 entry, int *iterator)
{
	BYTE *entry_bytes = (BYTE*) &entry;
	int i;
	for(i = 0; i < sizeof(entry); i++)
	{
		//printf("%d ", i);
		//printf("Ta: %c\n", *(entry_bytes + i));
		block[*iterator] = *(entry_bytes + i);
		(*iterator)++;
	}
	//printf("\n");
}

void read_entry(BYTE *block, DIRENT2 *entry, int *iterator)
{
	BYTE *entry_bytes = (BYTE*) entry;
	int i;
	for(i = 0; i < sizeof(DIRENT2); i++)
	{
		//printf("%d ", i);
		*(entry_bytes + i) = block[*iterator];
		(*iterator)++;
	}
	//printf("\n");
}


int check_if_root(char *name)
{
	if(name[0] == 'r' && name[1] == 'o' && name[2] == 'o' && name[3] == 't' && name[4] == '\0')
		return 0;

	return -1;
}

ReturnCode read_block(BYTE *block, WORD block_number)
{
	if (block == NULL)
		return T2FS_NULL_POINTER;
	//if (block_number > number_of_blocks || block_number < 0)
	//	return T2FS_INVALID_BLOCK_NUMBER;

	int block_initial_sector;
	if (block_number == root_block)
		block_initial_sector = root_sector;
	else
		block_initial_sector = root_sector + (sectors_per_block * (block_number -1));

	int i;
	for (i = 0; i < sectors_per_block; i++)
		read_sector(block_initial_sector + i, block + (i * SECTOR_SIZE));

	return T2FS_SUCCESS;
}

string* parse_path(string path, int *array_size)
{
	if (array_size == NULL || path == NULL)
		return NULL;
	if (path[0] == '\0')
		return NULL;

	// Copy original path since strtok() alters the parameter.
	string path_copy = (string) malloc((sizeof(char) * strlen(path)) + 3);
	
	if (path[0] == '/')
	{
		path_copy[0] = '.';
		path_copy[1] = '\0';
	} else if (path[0] != '.')
	{
		path_copy[0] = '.';
		path_copy[1] = '/';
		path_copy[2] = '\0';	
	} else
	{
		path_copy[0] = '\0';
	}

	strcat(path_copy, path);

	// Count number of entries in path.
	int i;
	*array_size = 1;
	for (i = 0; i < strlen(path_copy); i++)
	{
		if (path_copy[i] == '/' && i != (strlen(path_copy)-1))
			*array_size += 1;
	}

	// Break path into array of entries.
	string* path_entries = (string*) malloc(sizeof(string) * (*array_size));
	string entry;

	i = 0;
	path_copy = strtok(path_copy, "/");
	while (path_copy != NULL)
	{
		entry = (string) malloc(sizeof(char) * strlen(path_copy) + 1);
		strcpy(entry, path_copy);

		path_entries[i] = entry;

		i++;
		path_copy = strtok(NULL, "/");
	}

	return path_entries;
}

string get_entry_name(string path)
{
	int number_of_entries;
	string *path_entries = parse_path(path, &number_of_entries);

	if (number_of_entries == 0 || path_entries == NULL)
		return NULL;
	else
		return path_entries[number_of_entries - 1];
}


int get_entry_number(string name, WORD number)
{
	return number - 1; 
}


int free_block_bit()
{
	int i, j, k;
	extern BYTE *bitmap;
	extern unsigned int bitmap_end, bitmap_start;
	int block_range_s;
	BYTE *bitmap_sector = (BYTE *) malloc(sizeof(BYTE ) * SECTOR_SIZE);

	i = 0;

	while(bitmap[i] == 255 && i < (bitmap_end - bitmap_start))
			i++;

	if(i == (bitmap_end - bitmap_start))
	{
		printf("DISK IS FULL!\n");
		return -1;
	}

	block_range_s = (i*8)+1;
	printf("block range %d\n", block_range_s);

	j = 0;
	while((bitmap[i] & (1 << j)) != 0)
		j++;

	bitmap[i] |= 1 << j;

	printf("byte %d and bit %d.\n", i, j);


	for(k = 0; k < sizeof(BYTE) * SECTOR_SIZE; k++)
		bitmap_sector[k] = 0;

	k = 0;
	four_bytes_to_sector_array(bitmap_sector, bitmap_start, &k);
	four_bytes_to_sector_array(bitmap_sector, bitmap_end, &k);

	i = 0;
	for(k = (int) bitmap_start; k < (int) bitmap_end; k++)
	{
		bitmap_sector[k] = bitmap[i];
		i++;
	}

	write_sector(superblock_start, bitmap_sector);//start is the sector number of the first sector of the superblock

	return block_range_s + j;
}


void list_entries(BYTE *block)//basicamente readdir
{
	extern unsigned int dir_files_max, entry_p_dir_blck;
	int entry_number = 0, index_entry, /*index_count,*/ j, i;
	BYTE *entry_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	DIRENT2 entry;

	while(entry_number < dir_files_max)
	{
		//é o bloco root, primeiro de entries está salvo no 2 sim
		//tenho que atualizar index_entry...
		//entre blocos, não da dirent
		index_entry = block[(entry_number/entry_p_dir_blck) * 4] << 8 | block[((entry_number/entry_p_dir_blck) * 4)+1];
		//index_count = block[((entry_number/entry_p_dir_blck) * 4)+2] << 8 | block[((entry_number/entry_p_dir_blck) * 4)+3];


		if(index_entry == 65535)//unsigned word -1
		{
			//printf("entry vazia\n");
			entry_number += entry_p_dir_blck;
		}
		else
		{
			read_block(entry_block, index_entry);
			j = 0;
			for(i = 0; i < entry_p_dir_blck; i++)
			{
				//printf("i: %d\n", i);
				read_entry(entry_block, &entry, &j);
				if(entry.fileType != 0)
				{
					printf("\n");
					printf("nome do arquivo: %s\n", entry.name);
					printf("tipo do arquivo: %d\n", entry.fileType);
					printf("bloco do arquivo: %d\n", entry.indexBlock);
					if((strcmp(entry.name, "..\0") != 0) && entry.fileType == 3)
						printf("numero de entradas: %d\n", entry.numberOfEntries);
					if(entry.fileType == 1 || entry.fileType == 2)
						printf("tamanho do arquivo: %d\n", entry.fileSize);
					printf("\n");
				}
			}
			entry_number += entry_p_dir_blck;
		}

	}


}

int find_target_dir(string target)
{
	extern unsigned int dir_files_max, entry_p_dir_blck;
	extern WORD temp_dir_block;
	int entry_number = 0, index_entry, /*index_count,*/ j, i;
	BYTE *index_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	BYTE *entry_block = (BYTE *) malloc(sizeof(BYTE ) * block_size);
	DIRENT2 entry;
	read_block(index_block, temp_dir_block);

	while(entry_number < dir_files_max)
	{
		//é o bloco root, primeiro de entries está salvo no 2 sim
		//tenho que atualizar index_entry...
		//entre blocos, não da dirent
		index_entry = index_block[(entry_number/entry_p_dir_blck) * 4] << 8 | index_block[((entry_number/entry_p_dir_blck) * 4)+1];
		//index_count = block[((entry_number/entry_p_dir_blck) * 4)+2] << 8 | block[((entry_number/entry_p_dir_blck) * 4)+3];


		if(index_entry == 65535)//unsigned word -1
		{
			//printf("entry vazia\n");
			entry_number += entry_p_dir_blck;
		}
		else
		{
			read_block(entry_block, index_entry);
			j = 0;
			for(i = 0; i < entry_p_dir_blck; i++)
			{
				read_entry(entry_block, &entry, &j);
				if(entry.fileType != 0)
				{
					if(strcmp(target, entry.name) == 0)
					{
						temp_dir_block = entry.indexBlock;
						return 0;
					}
				}
			}
			entry_number += entry_p_dir_blck;
		}

	}

	return -1;
}

BOOL isFileHandleValid(FILE2 handle){
	if(handle < 0 || handle >= MAX_OPEN_FILES || (openFiles[handle].record.fileType != REGULAR && openFiles[handle].record.fileType != BINARIO))
		return FALSE;
	else
		return TRUE;
}

BOOL isDirHandleValid(DIR2 handle){
	if(handle < 0 || handle >= MAX_OPEN_DIR || openDirs[handle].record.fileType != DIRETORIO)
		return FALSE;
	else
		return TRUE;
}

void initializeOpenFiles(){
	int i;
	for(i = 0; i < MAX_OPEN_FILES; i++){
		openFiles[i].record.fileType = INVALIDO;		
	}
}

void initializeOpenDirs(){
	int i;
	for(i = 0; i < MAX_OPEN_DIR; i++){
		openDirs[i].record.fileType = INVALIDO;
	}
}

FILE2 getFreeFileHandle(){
	FILE2 freeHandle;
	for(freeHandle = 0; freeHandle < MAX_OPEN_FILES; freeHandle++){
		if(openFiles[freeHandle].record.fileType == INVALIDO)
			return freeHandle;
	}
	return T2FS_FAILURE;
}

DIR2 getFreeDirHandle(){
	DIR2 freeHandle;
	for(freeHandle = 0; freeHandle < MAX_OPEN_DIR; freeHandle++){
		if(openDirs[freeHandle].record.fileType == INVALIDO)
			return freeHandle;
	}
	return T2FS_FAILURE;
}