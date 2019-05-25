/**
 * This test uses TestCase instances to handle printf() calls.
 * 
 * @author Renan Kummer
 */
#include "../include/test_core.h"

// ======================================================================================
//                                       TEST CASES
// ======================================================================================

boolean test_SampleTestCases_Case1()
{
    return TRUE == TRUE;
}

boolean test_SampleTestCases_Case2()
{
    return TRUE == FALSE;
}

// ======================================================================================
//                                        THE TEST
// ======================================================================================

void test_sample_testcases()
{
    printf("Running test 'Sample: Test Cases'...\n");
    run_test_case("Case 1", test_SampleTestCases_Case1);
    run_test_case("Case 2", test_SampleTestCases_Case2);
}
