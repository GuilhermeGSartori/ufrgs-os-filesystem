/**
 * Definition of core structures and functions to run tests.
 * 
 * @author Renan Kummer
 */
#include "../include/test_core.h"
#include "../include/tests.h"

// ======================================================================================
//                                    GLOBAL VARIABLES
// ======================================================================================

string TestCore_SupportString = NULL;


// ======================================================================================
//                             SUPPORT FUNCTIONS: DECLARATION
// ======================================================================================

/**
 * Search for a test name in the array of tests.
 * 
 * @param test_name The name of the test to search for.
 * @return Test instance if found, NULL otherwise.
 */
Test* find_test(string test_name);

/**
 * Build the test case string to be written.
 * 
 * @param test_name  Name of test case.
 * @param has_passed Indicate if test case has passed or not.
 * @return The test case string to be written.
 */
string build_test_case_string(string test_name, boolean has_passed);

/**
 * Get test core's support string. It's pre-allocated with 100 characters.
 * 
 * @return Test core's support string.
 */
string get_test_core_support_string();


// ======================================================================================
//                                     MAIN FUNCTIONS
// ======================================================================================

int main(int argc, string argv[])
{
    if (argc > 1)
    {
        int i;
        for (i = 1; i < argc; i++)
        {
            Test* test = find_test(argv[i]);

            if (test != NULL)
                test->run();
            else
                printf("Test '%s' could not be found.\n", argv[i]);

            printf("\n");
        }
    }

    else
    {
        int i;
        for (i = 0; i < number_of_tests; i++)
        {
            the_tests[i].run();
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}


// ======================================================================================
//                           SUPPORT FUNCTIONS: IMPLEMENTATION
// ======================================================================================

boolean run_test_case(string test_name, boolean (*test_function)())
{
    boolean has_passed = test_function();
    printf("%s\n", build_test_case_string(test_name, has_passed));

    return has_passed;
}

Test* find_test(string test_name)
{
    int i;
    for (i = 0; i < number_of_tests; i++)
    {
        if (strcmp(the_tests[i].name, test_name) == 0)
            return &the_tests[i];
    }

    return NULL;
}

string build_test_case_string(string test_name, boolean has_passed)
{
    unsigned max_size  = 40;
    unsigned name_size = (unsigned) strlen(test_name);

    string test_result = get_test_core_support_string();
    strcpy(test_result, test_name);

    if (name_size < max_size)
    {
        int i;
        for (i = 0; i < (max_size - name_size); i++)
            strcat(test_result, ".");
        
    } else
        strcat(test_result, "..");

    if (has_passed)
        strcat(test_result, "OK");
    else
        strcat(test_result, "FAIL");

    return test_result;
}

string get_test_core_support_string()
{
    if (TestCore_SupportString == NULL)
        TestCore_SupportString = (string) malloc(sizeof(char) * 100);

    return TestCore_SupportString;
}
