#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include<time.h>
#include<io.h>

/*
    输出字符串颜色设置
*/
#define NONE                 "\e[0m"
#define GREEN                "\e[0;32m"
#define BLACK                "\e[0;30m"
#define L_BLACK              "\e[1;30m"
#define RED                  "\e[0;31m"
#define L_RED                "\e[1;31m"
#define YELLOW               "\e[1;33m"
#define BLUE                 "\e[0;34m"
#define WHITE                "\e[1;37m"

void log(int state,const char *fmt, ...)
{
    switch(state)
    {
        case START:
            fprintf(stdout,WHITE "[RUN      ] " NONE);
            break;
        case SUCCESS:
            fprintf(stdout,GREEN "[       OK] " NONE);
            break;
        case FAILED:
            fprintf(stdout,RED "[   FAILED] " NONE);
            break;
        default:
            fprintf(stdout,BLUE "[=========] " NONE);
            break;
    }
   
    va_list args;
    va_start(args,fmt);
    vfprintf(stdout,fmt,args);
    va_end(args);
    fprintf(stdout,"\n");
}