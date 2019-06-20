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
 * Global Variables Declaration
 * When used, they need to be declares in the files
 * that require them. Using "extern <type> <name>"
*/

_Bool __boot_init = 0;
_Bool __root_done = 1;//aplica o mkdir pra cada partição depois deixa isso 1...
int partition = 0;
unsigned int bitmap_start = 0, bitmap_end = 0;
unsigned int sectors_per_block = 0, root_sector = 0, partition_end = 0, root_block = 0, entry_p_dir_blck = 0, dir_idx_adrs_bytes = 0;
unsigned int dir_idx_hash_bytes = 0, dir_idx_adrs_number = 0, dir_files_max = 0, hash_size = 0, dir_idx_leftovers = 0;
unsigned int file_idx_entries = 0, file_max_size = 0, block_size = 0;
unsigned int information[14];
BYTE *bitmap;


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
	for(i = 0; i < 14; i++)
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
}

//a proxima função é essa só que refatorada, tá mto mais lógica e fácil de entender agora
//essa que tá comentada é mto satânica pro meu gosto
/*void four_bytes_to_sector_array(BYTE *sector, unsigned int info, int *iterator, int position)
{
	//the loops is used to store 4 bytes of unsigned ints to 4 single bytes (to save in the disk)
	//big endian storing
	for(*iterator = *iterator; *iterator < sizeof(unsigned int) * position; (*iterator)++)
			sector[*iterator] = (info) >> (24-(8*((*iterator) - sizeof(unsigned int)*(position-1)))) & 0xFF;
	//não colocar (BYTE) antes da info... por algum motivo dá overflow (?)
}*/

void four_bytes_to_sector_array(BYTE *sector, unsigned int info, int *iterator)
{
	//the loops is used to store 4 bytes of unsigned ints to 4 single bytes (to save in the disk)
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
		block[*iterator] = *entry_bytes + i;
		(*iterator)++;
	}
	//printf("\n");
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