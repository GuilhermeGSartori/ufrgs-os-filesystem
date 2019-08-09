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
 * This sample works in a similar way to writing a test in a single main() function.
 */
void test_sample_single_function();

/**
 * This sample uses TestCase instances to handle printf() calls.
 */
void test_sample_testcases();

/**
 * Test identify2() function.
 */
void test_identify2();

/**
 * Test getcwd2() function.
 */
void test_getcwd2();

/**
 * Test support functions.
 */
void test_support();

// ======================================================================================
//                                 ADD YOUR TESTS BELOW!
// ======================================================================================

// Number of tests in the_tests array.
unsigned number_of_tests = 3;

// Array containing all the tests.
Test the_tests[] = {
//  {"Sample: Single Function", test_sample_single_function},
//  {"Sample: Test Cases", test_sample_testcases},
    {"Identify2", test_identify2},
    {"Getcwd2", test_getcwd2},
    {"Support Functions", test_support}
};


#endif // UFRGS_OS_FILESYSTEM_TESTE_INCLUDE_TESTS_H
