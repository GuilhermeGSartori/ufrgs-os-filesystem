/**
 * Identify the developers of T2FS.
 * 
 * @author ?
 */
#include "../include/constants.h"
#include "../include/t2fs.h"
#include <stdlib.h>
#include <string.h>

// ================================================================================================
//                                  SUPPORT FUNCTIONS: DECLARATION
// ================================================================================================

/**
 * Get T2FS' developers' names and university IDs.
 * 
 * @return T2FS' developers' names and university IDs.
 */
char* identify2_GetDevelopers();

/**
 * Get minimum buffer size required to write the names and university IDs of T2FS' develoepers. 
 * 
 * @return Minimum buffer size required for identification.
 */
unsigned identify2_GetMinimumBufferSize();


// ================================================================================================
//                                         T2FS: IDENTIFY2
// ================================================================================================

int identify2(char* name, int size)
{
    if (name == NULL)
        return T2FS_NULL_POINTER;
    if (size < identify2_GetMinimumBufferSize())
        return T2FS_NOT_ENOUGH_MEMORY;
 
    strcpy(name, identify2_GetDevelopers());
    return T2FS_SUCCESS;
}


// ================================================================================================
//                                 SUPPORT FUNCTIONS: IMPLEMENTATION
// ================================================================================================

char* identify2_GetDevelopers()
{
    return "Guilherme Girotto Sartori 00274713\nMarlize Ramos Batista 00274703\nRenan Kummer de Jesus 00208946\n";
}

unsigned identify2_GetMinimumBufferSize()
{
    return 98;
}
