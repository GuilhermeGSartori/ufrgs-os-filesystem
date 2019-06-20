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
    if(__boot_init == 0)
		boot2();

    DIR2 freeHandle = getFreeDirHandle(); 
	if(freeHandle == T2FS_FAILURE) 
		return T2FS_NO_FREE_HANDLE;     // quantidade máxima permitida de diretórios abertos foi alcançada

    DIRENT2 record;
	if(getRecordFromPath(pathname, &record) != 0){      //???????????
		return T2FS_FAILURE;
	} 

	if(record.fileType != DIRETORIO){
		return T2FS_FAILURE;
	}

	openDirs[freeHandle].record = record;
	openDirs[freeHandle].currentPointer = 0;

	return freeHandle;
}
