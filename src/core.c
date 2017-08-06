#include <setjmp.h>
#include <core.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Default signal functions that should be restored after a test is complete.
typedef void (*SignalFunction)(int signal);
static SignalFunction default_signal_functions[ARRAY_LENGTH(exception_signals)];

// Create function results and expected parameter lists.
void initialize_testing(const char *test_name)
{
    return;
}
void teardown_testing(const char *test_name)
{
    return;
}
// Keeps track of the calling context returned by setjmp
static jmp_buf global_run_test_env;
// Exit the currently executing test.
static void exit_test(void)
{
    longjmp(global_run_test_env, 1);
    // 返回调用处,相当与c语言里面的异常机制
    // 和setjmp相对应
}

void handle_exceptions(int signal)
{
    printf("%s\n", strsignal(signal));
    exit_test();
}
int __run_test(const UnitTest * const test)
{
    int failed = 0;
    // take over the exceptions handle
    for(int i=0;i<ARRAY_LENGTH(exception_signals);i++)
    {
       default_signal_functions[i] =  signal(exception_signals[i],handle_exceptions);
    }
    initialize_testing(test->name);
    // 下面相当于开一个新的线程跑测试用例
    // 新开一个函数栈跑测试用例,如果出错即异常无法从函数返回时,
    // 捕获异常,然后直接重该函数栈跳回调用处
    if(!setjmp(global_run_test_env))
    {
        // 直接返回
        printf("start test\n");
        test->function(NULL);
        printf("test successed\n");
    }
    else
    {
        failed = 1;
        printf("test failed\n");
    }
    teardown_testing(test->name);
    // 恢复默认异常处理
    for (int i = 0; i < ARRAY_LENGTH(exception_signals); i++)
    {
        signal(exception_signals[i], default_signal_functions[i]);
    }
    return failed;
}
int __run_tests(const UnitTest * const tests,const size_t number_of_tests)
{
    // return total failed test cases
    size_t current_test_index = 0;
    size_t run_unit_test = 0;
    int total_failed = 0;
    while(current_test_index < number_of_tests)
    {
        const UnitTest * const current_test = &tests[current_test_index++];
        if(!current_test->function)
        {
            continue;
        }
        switch (current_test->function_type)
        {
            case UNIT_TEST_FUNCTION_TYPE_TEST:
                run_unit_test = 1;
                break;
            case UNIT_TEST_FUNCTION_TYPE_SETUP:
                break;
            case UNIT_TEST_FUNCTION_TYPE_TEARDOWN:
                break;
            default:
                break;
        }
        if(run_unit_test)
        {
            if(__run_test(current_test))
            {
                total_failed++;
            }
        }
    }
    return total_failed;
}