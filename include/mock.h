/*
    提供mock功能
*/
#ifndef MOCK_H
#define MOCK_H

#define LargeINT unsigned long long
#define mock() _mock(__func__,__FILE__,__LINE__)
#define will_return(func,value) _will_return(#func,value)

void _will_return(const char * function_name, const LargeINT value);
LargeINT _mock(const char *const function_name, const char *const file, int line);

#endif