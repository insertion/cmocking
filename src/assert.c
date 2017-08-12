
#include <io.h>
#include <core.h>
#include <assert.h>

/*
    测试逻辑错误
    使用assert的缺点是，频繁的调用会极大的影响程序的性能，增加额外的开销。 
*/

void _assert_true(const LargeINT value,const char *expression,const char *function, const char * file,const int line)
{
    if(!value)
    {
        log_err("%s in %s %s:%d" ,expression,function,file,line);
        exit_test(1);
    }
}