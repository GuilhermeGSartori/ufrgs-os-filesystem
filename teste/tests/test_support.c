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

#define ENTRY_BLOCK_NUM  30
#define ENTRY_BLOCK_NAME "ze_da_treta"

extern WORD working_dir_block;
extern unsigned int block_size;
extern unsigned int entry_p_dir_blck;
extern unsigned int dir_idx_adrs_bytes;

// ================================================================================================
//                                        SUPPORT FUNCTIONS
// ================================================================================================

void setup_test_block()
{
    BYTE* block = (BYTE*) malloc(sizeof(BYTE) * block_size);

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

void setup_directory_block()
{
    BYTE* current_directory = (BYTE*) malloc(sizeof(BYTE) * block_size);
    read_block(current_directory, working_dir_block);

    WORD* dir_as_word = (WORD*) current_directory;
    // First index is full.
    // Second index is not allocated.
    int i;
    for (i = 0; i < dir_idx_adrs_bytes; i++)
    {
        if (i == 0)
            dir_as_word[i] = (WORD) 150;
        else if (i == 1)
            dir_as_word[i] = (WORD) entry_p_dir_blck;
        else
            dir_as_word[i] = (WORD) -1;
    }

    DIRENT2* the_entry = (DIRENT2*) malloc(sizeof(DIRENT2));
    the_entry->indexBlock = (WORD) ENTRY_BLOCK_NUM;
    strcpy(the_entry->name, ENTRY_BLOCK_NAME);

    DIRENT2* entry_block = (DIRENT2*) malloc(sizeof(DIRENT2) * entry_p_dir_blck);
    for (i = 0; i < entry_p_dir_blck; i++)
        entry_block[i] = *the_entry;
    
    BYTE* entry_block_as_byte = (BYTE*) entry_block;
    write_block(current_directory, working_dir_block);
    write_block(entry_block_as_byte, (WORD) 150);    
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

boolean test_GetEntryName_NullParameter()
{
    if (get_entry_name(NULL) == NULL)
        return TRUE;

    return FALSE;
}

boolean test_GetEntryName_ValidCall()
{
    char test_path[] = {'.','/','a','l','o','/','t','r','e','t','a','\0'};
    string entry_name = get_entry_name(test_path);
    
    if (strcmp(entry_name, "treta") == 0)
        return TRUE;

    return FALSE;
}

boolean test_NewEntry_ValidCall()
{
    int index;

    if (new_entry(ENTRY_BLOCK_NAME, &index) != T2FS_SUCCESS)
    {
        printf("NewEntry: Status != T2FS_SUCCESS");
        return FALSE;
    }

    if (index != 2)
    {
        printf("NewEntry: Index != 2");
        return FALSE;
    }

    return TRUE;
}

boolean test_FindEntry_ValidCall()
{
    WORD index_block_number;

    if (find_entry(ENTRY_BLOCK_NAME, &index_block_number) != T2FS_SUCCESS)
    {
        printf("FindEntry: Status != T2FS_SUCCESS");
        return FALSE;
    }

    if (index_block_number != (WORD) 150)
    {
        printf("Wrong block number: %d\n", index_block_number);
        return FALSE;
    }

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
    setup_directory_block();

    // Run all test cases.
    run_test_case("ReadBlock_NullBuffer", test_ReadBlock_NullBuffer);
    run_test_case("ReadBlock_ValidCall", test_ReadBlock_ValidCall);
    run_test_case("ParsePath_NullParameters", test_ParsePath_NullParameters);
    run_test_case("ParsePath_ValidCalls", test_ParsePath_ValidCalls);
    run_test_case("GetEntryName_NullParameter", test_GetEntryName_NullParameter);
    run_test_case("GetEntryName_ValidCall", test_GetEntryName_ValidCall);
    run_test_case("NewEntry_ValidCall", test_NewEntry_ValidCall);
    run_test_case("FindEntry_ValidCall", test_FindEntry_ValidCall);
}
