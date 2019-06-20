#ifndef UFRGS_OS_FILESYSTEM_INCLUDE_CONSTANTS_H
#define UFRGS_OS_FILESYSTEM_INCLUDE_CONSTANTS_H
/**
 * Definition of constants to be reused in the library.
 * 
 * @author Guilherme Sartori
 * @author Marlize Ramos
 * @author Renan Kummer
 */

#define BOOL unsigned short int
#define TRUE 1
#define FALSE 0

typedef enum ReturnCode
{
    T2FS_SUCCESS =  0,
    T2FS_FAILURE = -1,

    T2FS_NULL_POINTER         = -2,
    T2FS_NOT_ENOUGH_MEMORY    = -3,
    T2FS_NO_FREE_HANDLE       = -4,
    T2FS_DIRECTORY_IS_FULL    = -5,
    T2FS_INVALID_BLOCK_NUMBER = -6
} ReturnCode;

typedef enum FileTypes
{
    INVALIDO = 0x00,
    REGULAR = 0x01,
    BINARIO = 0x02,
    DIRETORIO = 0x03

} FileTypes;



#endif // UFRGS_OS_FILESYSTEM_INCLUDE_CONSTANTS_H
