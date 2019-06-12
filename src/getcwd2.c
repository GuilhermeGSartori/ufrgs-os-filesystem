/**
 * Display the current working directory.
 * 
 * @author Renan Kummer
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include <stdlib.h>
#include <string.h>

extern char working_dir_path[10000];

// ================================================================================================
//                                  SUPPORT FUNCTIONS: DECLARATION
// ================================================================================================

/**
 * Get minimum size required to store the working directory path.
 * 
 * @return Size required to store the working directory path.
 */
int getcwd2_GetMinimumSizeForPath();


// ================================================================================================
//                                         T2FS: GETCWD2
// ================================================================================================

int getcwd2(char *pathname, int size)
{
    if (pathname == NULL)
        return T2FS_NULL_POINTER;

    else if (size >= getcwd2_GetMinimumSizeForPath())
    {
        strcpy(pathname, working_dir_path);
        return T2FS_SUCCESS;
    }

    return T2FS_NOT_ENOUGH_MEMORY;
}


// ================================================================================================
//                                 SUPPORT FUNCTIONS: IMPLEMENTATION
// ================================================================================================

int getcwd2_GetMinimumSizeForPath()
{
    return strlen(working_dir_path) + 1;
}
