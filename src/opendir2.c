/**
 * Open an existing directory from the disk.
 * 
 * @author Marlize Ramos
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/support.h"

extern OpenFile openDirs[MAX_OPEN_DIRS];

DIR2 opendir2(char *pathname)
{
	int i;
	extern _Bool __boot_init;
	extern WORD working_dir_block, temp_dir_block;
	int path_size;

    if(__boot_init == 0)
		boot2();

    DIR2 freeHandle = getFreeDirHandle(); 
	if(freeHandle == T2FS_FAILURE) 
		return T2FS_NO_FREE_HANDLE;     // quantidade máxima permitida de diretórios abertos foi alcançada

    DIRENT2 record;
	string *path = parse_path(pathname, &path_size);

   	temp_dir_block = working_dir_block; //faz chdir com temp. e último alvo salva a entrada em record
	for(i = 1; i < path_size-1; i++)
	{
		if(find_target_dir(path[i]) != 0)
		{
			printf("path invalido '%s' nao existe!\n", path[i]);
			return T2FS_FAILURE;
		}
	}

	if(entry_to_record(path[path_size-1], &record) != 0)
		return T2FS_FAILURE;

	if(record.fileType != DIRETORIO){
		return T2FS_FAILURE;
	}

	openDirs[freeHandle].record = record;
	openDirs[freeHandle].currentPointer = 0;
	openDirs[freeHandle].currentEntry = 0;

	return freeHandle;
}
