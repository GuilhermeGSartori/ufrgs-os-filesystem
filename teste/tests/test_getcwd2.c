/**
 * Test getcwd2() function.
 * 
 * @author Renan Kummer
 */
#include "../include/test_core.h"
#include "../../include/constants.h"
#include "../../include/t2fs.h"
#include <string.h>

extern char working_dir_path[10000];

// ================================================================================================
//                                            TEST CASES
// ================================================================================================

boolean test_Getcwd2_NullBuffer()
{
    if (getcwd2(NULL, 1) == T2FS_NULL_POINTER)
        return TRUE;
    
    return FALSE;
}

boolean test_Getcwd2_SmallBufferSize()
{
    unsigned bufferSize = 0;
    string pathname = (string) malloc(sizeof(char) * bufferSize);

    if (getcwd2(pathname, bufferSize) == T2FS_NOT_ENOUGH_MEMORY)
        return TRUE;

    return FALSE;
}

boolean test_Getcwd2_ValidCall()
{
    unsigned bufferSize = 2;
    string pathname = (string) malloc(sizeof(char) * bufferSize);

    if (getcwd2(pathname, bufferSize) == T2FS_SUCCESS && strcmp(pathname, working_dir_path) == 0)
        return TRUE;

    return FALSE;
}


// ================================================================================================
//                                             THE TEST
// ================================================================================================

void test_getcwd2()
{
    // Set up test cases.
    working_dir_path[0] = '\0';
    strcpy(working_dir_path, "/");

    run_test_case("NullBuffer", test_Getcwd2_NullBuffer);
    run_test_case("SmallBufferSize", test_Getcwd2_SmallBufferSize);
    run_test_case("ValidCall", test_Getcwd2_ValidCall);
}
