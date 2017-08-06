#include <core.h>

// initial a UnitTest with a function
#define fn_to_UnitTest(f) {#f,f,UNIT_TEST_FUNCTION_TYPE_TEST}

//Run tests specified by an array of UnitTest structures.
int __run_tests(const UnitTest * const tests,const size_t number_of_tests);
#define run_tests(tests) __run_tests(tests, sizeof(tests) / sizeof(tests)[0])