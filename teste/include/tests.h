#ifndef UFRGS_OS_FILESYSTEM_TESTE_INCLUDE_TESTS_H
#define UFRGS_OS_FILESYSTEM_TESTE_INCLUDE_TESTS_H
/**
 * Declaration and addition of Test instances to run.
 * 
 * To create new tests:
 *   - Declare the test function under "DECLARATION OF TEST FUNCTIONS".
 *   - The test function must have follow the signature "void function_name()".
 *   - Add a row with your Test instance under "ADD YOUR TESTS BELOW!" (e.g. {<test_name>, <test_function>}).
 *   - Increment number_of_tests variable.
 * 
 * Two samples are provided below, one works like a main() function, the other uses TestCase instances.
 * 
 * @author Renan Kummer
 */
#include "test_core.h"

// ======================================================================================
//                             DECLARATION OF TEST FUNCTIONS
// ======================================================================================

/**
 * This test works in a similar way to writing a test in a single main() function.
 */
void test_sample_single_function();

/**
 * This test uses TestCase instances to handle printf() calls.
 */
void test_sample_testcases();


// ======================================================================================
//                                 ADD YOUR TESTS BELOW!
// ======================================================================================

// Number of tests in the_tests array.
unsigned number_of_tests = 2;

// Array containing all the tests.
Test the_tests[] = {
    {"Sample: Single Function", test_sample_single_function},
    {"Sample: Test Cases", test_sample_testcases}
};


#endif // UFRGS_OS_FILESYSTEM_TESTE_INCLUDE_TESTS_H