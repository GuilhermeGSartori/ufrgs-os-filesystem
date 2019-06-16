/**
 * Test support functions.
 * 
 * @author Renan Kummer
 */
#include "../include/test_core.h"
#include "../../include/constants.h"
#include "../../include/support.h"
#include "../../include/t2fs.h"
#include <string.h>

extern char working_dir_path[10000];

// ================================================================================================
//                                            TEST CASES
// ================================================================================================

boolean test_ReadBlock_NullBuffer()
{
    if (read_block(NULL, 0) == T2FS_NULL_POINTER)
        return TRUE;
    
    return FALSE;
}


// ================================================================================================
//                                             THE TEST
// ================================================================================================

void test_support()
{
    // Set up test cases.
    working_dir_path[0] = '\0';
    strcpy(working_dir_path, "/");

    // Run all test cases.
    run_test_case("ReadBlock_NullBuffer", test_ReadBlock_NullBuffer);
}
