/*
    提供输入输出功能
*/
#ifndef IO_H
#define IO_H

#define log testlog
// 用来解决符号冲突
enum 
{
    START = 0,
    SUCCESS,
    FAILED,
    ERROR,
};
void log(int state,const char *fmt, ...);

// 缺省号代表一个可以变化的参数表。使用保留名 __VA_ARGS__ 把参数传递给宏
#define log_run(...) log(START,__VA_ARGS__)
#define log_ok(...) log(SUCCESS,__VA_ARGS__)
#define log_fail(...) log(FAILED,__VA_ARGS__)
#define log_err(...) log(ERROR,__VA_ARGS__)
#define log_info(...) log(ERROR,__VA_ARGS__)

#endif