#ifndef UFRGS_OS_FILESYSTEM_INCLUDE_CONSTANTS_H
#define UFRGS_OS_FILESYSTEM_INCLUDE_CONSTANTS_H
/**
 * Definition of constants to be reused in the library.
 * 
 * @author Renan Kummer
 */

typedef enum ReturnCode
{
    T2FS_SUCCESS =  0,
    T2FS_FAILURE = -1,

    T2FS_NULL_POINTER      = -2,
    T2FS_NOT_ENOUGH_MEMORY = -3
} ReturnCode;


#endif // UFRGS_OS_FILESYSTEM_INCLUDE_CONSTANTS_H
