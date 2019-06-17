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

boolean test_ParsePath_NullParameters()
{
    char test_path[] = {'.','/','a','l','o','/','t','r','e','t','a','\0'};
    int test_size;

    if (parse_path(NULL, &test_size) != NULL)
        return FALSE;
    if (parse_path(test_path, NULL) != NULL)
        return FALSE;

    return TRUE;
}

boolean test_ParsePath_ValidCalls()
{
    int test_size;
    char test_path1[] = {'.','/','a','l','o','/','t','r','e','t','a','\0'};
    char test_path2[] = {'.','\0'};
    char test_path3[] = {'/','\0'};
    char test_path4[] = {'.','.','\0'};
    char test_path5[] = {'a','l','o','\0'};

    string *path_entries = parse_path(test_path1, &test_size);
    if (test_size != 3)
        return FALSE;
    if (path_entries == NULL)
        return FALSE;
    if (strcmp(path_entries[0],".") != 0   || 
        strcmp(path_entries[1],"alo") != 0 ||
        strcmp(path_entries[2],"treta") != 0)
    {
        return FALSE;
    }

    path_entries = parse_path(test_path2, &test_size);
    if (test_size != 1)
        return FALSE;
    if (path_entries == NULL)
        return FALSE;
    if (strcmp(path_entries[0],".") != 0)
        return FALSE;

    path_entries = parse_path(test_path3, &test_size);
    if (test_size != 1)
        return FALSE;
    if (path_entries == NULL)
        return FALSE;
    if (strcmp(path_entries[0],".") != 0)
        return FALSE;

    path_entries = parse_path(test_path4, &test_size);
    if (test_size != 1)
        return FALSE;
    if (path_entries == NULL)
        return FALSE;
    if (strcmp(path_entries[0],"..") != 0)
        return FALSE;

    path_entries = parse_path(test_path5, &test_size);
    if (test_size != 2)
        return FALSE;
    if (path_entries == NULL)
        return FALSE;
    if (strcmp(path_entries[0],".") != 0 || strcmp(path_entries[1],"alo") != 0)
        return FALSE;

    return TRUE;
}

// ================================================================================================
//                                             THE TEST
// ================================================================================================

void test_support()
{
    // Set up test cases.
    format2(2);
    boot2();
    setup_test_block();

    // Run all test cases.
    run_test_case("ReadBlock_NullBuffer", test_ReadBlock_NullBuffer);
    run_test_case("ReadBlock_ValidCall", test_ReadBlock_ValidCall);
    run_test_case("ParsePath_NullParameters", test_ParsePath_NullParameters);
    run_test_case("ParsePath_ValidCalls", test_ParsePath_ValidCalls);
}
