/**
 * Test identify2() function.
 * 
 * @author Renan Kummer
 */
#include "../include/test_core.h"
#include "../../include/constants.h"
#include "../../include/t2fs.h"

// ================================================================================================
//                                            TEST CASES
// ================================================================================================

boolean test_Identify2_NullBuffer()
{
    if (identify2(NULL, 98) == T2FS_NULL_POINTER)
        return TRUE;
    
    return FALSE;
}

boolean test_Identify2_SmallBufferSize()
{
    unsigned bufferSize = 97;
    string name = (string) malloc(sizeof(char) * bufferSize);

    if (identify2(name, bufferSize) == T2FS_NOT_ENOUGH_MEMORY)
        return TRUE;

    return FALSE;
}

boolean test_Identify2_ValidCall()
{
    unsigned bufferSize = 98;
    string name = (string) malloc(sizeof(char) * bufferSize);

    if (identify2(name, bufferSize) == T2FS_SUCCESS && 
        strcmp(name, "Guilherme Girotto Sartori 00274713\nMarlize Ramos Batista 00274703\nRenan Kummer de Jesus 00208946\n") == 0)
    {
        return TRUE;
    }

    return FALSE;
}


// ================================================================================================
//                                             THE TEST
// ================================================================================================

void test_identify2()
{
    run_test_case("NullBuffer", test_Identify2_NullBuffer);
    run_test_case("SmallBufferSize", test_Identify2_SmallBufferSize);
    run_test_case("ValidCall", test_Identify2_ValidCall);
}
