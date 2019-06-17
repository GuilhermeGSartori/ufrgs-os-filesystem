/**
 * Test support functions.
 * 
 * @author Renan Kummer
 */
#include "../include/test_core.h"
#include "../../include/constants.h"
#include "../../include/support.h"
#include "../include/test_setup.h"
#include "../../include/t2fs.h"
#include <string.h>

// ================================================================================================
//                                        SUPPORT FUNCTIONS
// ================================================================================================

void setup_test_block()
{
    BYTE* block = (BYTE*) malloc(sizeof(BYTE) * (SECTOR_SIZE * 2));

    unsigned char c = '0';
    int i;
    for (i = 0; i < 10; i++)
    {
        block[i] = c;
        c++;
    }

    c = '9'; 
    for (i = 0; i < 10; i++)
    {
        block[i + SECTOR_SIZE] = c;
        c--;
    }

    write_block(block, 10);
}


// ================================================================================================
//                                            TEST CASES
// ================================================================================================

boolean test_ReadBlock_NullBuffer()
{
    if (read_block(NULL, 0) == T2FS_NULL_POINTER)
        return TRUE;
    
    return FALSE;
}

boolean test_ReadBlock_ValidCall()
{
    BYTE *block = (BYTE*) malloc(sizeof(BYTE) * (SECTOR_SIZE * 2));

    if (read_block(block, 10) == T2FS_SUCCESS)
    {
        unsigned char c = '0';

        int i;
        for (i = 0; i < 10; i++)
        {
            if (block[i] != c)
                return FALSE;
            
            c++;
        }

        c = '9';
        for (i = 0; i < 10; i++)
        {
            if (block[i + SECTOR_SIZE] != c)
                return FALSE;

            c--;
        }

        return TRUE;
    } 
    
    else
        return FALSE;
}


// ================================================================================================
//                                             THE TEST
// ================================================================================================

void test_support()
{
    // Set up test cases.
    if (setup_test_virtual_disk() == T2FS_FAILURE)
    {
        printf("Failed to setup test cases!\n");
        return;
    } else
    {
        format2(2);
        boot2();

        setup_test_block();
    }

    // Run all test cases.
    run_test_case("ReadBlock_NullBuffer", test_ReadBlock_NullBuffer);
    run_test_case("ReadBlock_ValidCall", test_ReadBlock_ValidCall);
}
