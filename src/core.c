#include <setjmp.h>
#include <core.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>


/*
    测试运行时错误
    看别人代码时,看到有些变量觉得没有意义,先放者,当你需要某个功能时就觉得有意义了
*/
// Default signal functions that should be restored after a test is complete.
typedef void (*SignalFunction)(int signal);
static SignalFunction default_signal_functions[ARRAY_LENGTH(exception_signals)];
// static  修饰符有助于缓解符号冲突,所有不向外提供接口的符号都用static修饰
// Create function results and expected parameter lists.
static void initialize_testing(const char *test_name)
{
    // 测试前的环境配置
    return;
}
static void teardown_testing(const char *test_name)
{
    // 测试后的环境清理
    return;
}
// Keeps track of the calling context returned by setjmp
static jmp_buf global_run_test_env;
/*这个标志的作用是为了区别测试代码的错误和其它代码的错误
 *如果是测试代码出错,进程不会退出,只是退出测试用例函数,而
 *如果是其它代码出错,整个测试进程会结束*/
int global_running_test = 0;
// Exit the currently executing test.
void exit_test(int exit_flag)
{
    if(global_running_test)
        longjmp(global_run_test_env, 1);
    else if(exit_flag)
    {
        exit(-1);
    }

    // 返回调用处,相当与c语言里面的异常机制
    // 和setjmp相对应
}

static void handle_exceptions(int signal)
{
    log_err("%s", strsignal(signal));
    exit_test(1);
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
        log_run("%s",test->name);
        // run test case
        global_running_test = 1;
        test->function(NULL);
        global_running_test = 0;
        // if return successfully
        log_ok("%s",test->name);
    }
    else
    {
        failed = 1;
        log_fail("%s",test->name);
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