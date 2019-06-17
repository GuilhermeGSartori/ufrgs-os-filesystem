#ifndef TESTE_INCLUDE_TEST_SETUP_H
#define TESTE_INCLUDE_TEST_SETUP_H
/**
 * Functions to setup test cases for the library.
 * 
 * @author Renan Kummer
 */
#include "../../include/constants.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Copy the original virtual disk file to bin/ directory.
 * 
 * @return T2FS_SUCCESS if the file has been copied, T2FS_FAILURE otherwise.
 */
ReturnCode setup_test_virtual_disk();

#endif // TESTE_INCLUDE_TEST_SETUP_H
