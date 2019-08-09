/**
 * Close a file.
 * 
 * @author Marlize Ramos
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/support.h"

extern _Bool __boot_init;
extern OpenFile openFiles[MAX_OPEN_FILES];

int close2(FILE2 handle)
{
    if(__boot_init == 0)
		boot2();

    if(isFileHandleValid(handle)){
	 	openFiles[handle].record.fileType = INVALIDO;
        return T2FS_SUCCESS;
    }    

    return T2FS_FAILURE;
}
