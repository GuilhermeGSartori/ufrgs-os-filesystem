#ifndef UFRGS_OS_FILESYSTEMS_TESTE_INCLUDE_TEST_CORE_H
#define UFRGS_OS_FILESYSTEMS_TESTE_INCLUDE_TEST_CORE_H
/**
 * Definition of core structures and functions to run tests.
 * 
 * @author Renan Kummer
 */
#include "../../include/support.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// ======================================================================================
//                                   TYPES & STRUCTURES
// ======================================================================================

// Constants for boolean type.
typedef enum boolean { FALSE, TRUE } boolean;


// Definition of Test structure.
typedef struct Test
{
    string name;
    void (*run)();
} Test;



// ======================================================================================
//                                     MAIN FUNCTIONS
// ======================================================================================

/**
 * Entry point for Test executable. To run the tests:
 *   - Run the executable without any arguments to run all the tests.
 *   - For a single Test only, pass the name of the Test as argument.
 * 
 * @param argc Count of arguments passed in command-line (always >= 1).
 * @param argv Array of arguments passed in command-line (the first is always the name of executable).
 * @return Exit code, i.e. indicates if the executable finished successfully or not.
 */
int main(int argc, string argv[]);


// ======================================================================================
//                                   SUPPORT FUNCTIONS
// ======================================================================================

/**
 * Runs a TestCase function and writes to the command-line if it has passed or not.
 * 
 * @param the_test Test case to run.
 * @return TRUE if passed, FALSE otherwise.
 */
boolean run_test_case(string test_name, boolean (*test_function)());


#endif // UFRGS_OS_FILESYSTEMS_TESTE_INCLUDE_TEST_CORE_H