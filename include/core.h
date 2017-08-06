
/*
    提供一些需要暴露给其它模块的接口
*/
#include<signal.h>

// Function prototype for setup, test and teardown functions.
typedef void (*UnitTestFunction)(void **state);

// Type of the unit test function.
typedef enum UnitTestFunctionType {
    UNIT_TEST_FUNCTION_TYPE_TEST = 0,
    UNIT_TEST_FUNCTION_TYPE_SETUP,
    UNIT_TEST_FUNCTION_TYPE_TEARDOWN,
} UnitTestFunctionType;

// Stores a unit test function with its name and type
typedef struct UnitTest {
    const char* name;
    UnitTestFunction function;
    UnitTestFunctionType function_type;
} UnitTest;
/*
    typedef unsigned int size_t
    unsigned int随着编译平台不同，其长度可能不一样，比如：TC：2字节，VC：4字节，GCC下：4字节
    而size_t则希望保持不变,当换了平台之后，只需要重新定义size_t到新的类型，就可以不修改任何库就可以使用
*/

// Calculates the number of elements in an array.
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

// Signals caught by exception_handler().
static const int exception_signals[] = {
    SIGFPE,
    SIGILL,
    SIGSEGV,
    SIGBUS,
    SIGSYS,
};

