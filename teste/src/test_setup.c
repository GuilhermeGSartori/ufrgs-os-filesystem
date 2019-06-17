/**
 * Functions to setup test cases for the library.
 * 
 * @author Renan Kummer
 */
#include "../include/test_setup.h"

ReturnCode setup_test_virtual_disk()
{
    FILE *originalFile = fopen("../teste/resources/t2fs_disk.dat", "rb");
    FILE *copyFile     = fopen("t2fs_disk.dat", "wb");

    if (originalFile == NULL || copyFile == NULL)
        return T2FS_FAILURE;

    char c;
    while ((c = fgetc(originalFile)) != EOF)
        fputc(c, copyFile);

    fclose(originalFile);
    fclose(copyFile);    
}
