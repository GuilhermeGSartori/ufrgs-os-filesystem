/**
 * Open an existing file in the disk.
 * 
 * @author Marlize Ramos
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/support.h"

extern OpenFile openFiles[MAX_OPEN_FILES];

FILE2 open2(char* filename)
{
    if(__boot_init == 0)
		boot2();

    FILE2 freeHandle = getFreeFileHandle(); 
	if(freeHandle == T2FS_FAILURE) 
		return T2FS_NO_FREE_HANDLE;  // quantidade máxima permitida de arquivos abertos (10) foi alcançada

	DIRENT2 record;
	if(getRecordFromPath(filename, &record) != 0){
		return T2FS_FAILURE;
	}

	if(record.fileType == REGULAR || record.fileType == BINARIO){
		openFiles[freeHandle].record = record;
		openFiles[freeHandle].currentPointer = 0;
		return freeHandle;
	}
	return T2FS_FAILURE;    

}
