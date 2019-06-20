/**
 * Close a directory.
 * 
 * @author Marlize Ramos
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include "../include/support.h"

int closedir2(DIR2 handle)
{
    if(__boot_init == 0)
		boot2();

    if(isDirHandleValid(handle)) {
        openDirs[handle].record.fileType = INVALIDO;
		return T2FS_SUCCESS;
	}

    return T2FS_FAILURE;
}
