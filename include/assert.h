
// outputs one line for each macro whose expansion leads to the offending code
#define LargeINT unsigned long long
// Perform an unsigned cast to LargestIntegralType.
#define cast_to_largest_integral_type(value) ((LargeINT)((unsigned)(value)))
// Assert that the given expression is true.
#define assert_true(c) _assert_true(cast_to_largest_integral_type(c), #c, __func__, __FILE__, __LINE__)
// Assert that the given expression is false.
#define assert_false(c) _assert_true(!(cast_to_largest_integral_type(c)), #c, __FILE__, __LINE__)