#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include "usart.h"
#include <stdarg.h>
#include <string.h>

//开启此宏定义，即可开启调试功能，在开发结束时关闭此宏定义，就可关闭调试功能。
//#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE

/*
__FILE_NAME宏定义，用于获取当前源文件名，并去除路径中"\"前面内容。
*/
#define __FILE_NAME (strrchr((char *)__FILE__, '\\')?strrchr((char *)__FILE__, '\\') + 1:__FILE__) 
/*
__FILE_NAME__宏定义，用于获取当前源文件名，并去除路径"/"前面内容。注意此宏定义是基于__FILE_NAME宏定义去除路径中"\"前面内容后的结果。
*/
#define __FILE_NAME__ (strrchr((char *)__FILE_NAME, '/')?strrchr((char *)__FILE_NAME, '/') + 1:__FILE_NAME)  

/*
定义调试打印宏，用于格式化输出调试信息，整个宏的作用是将 Debug_printf(...) 调用直接转发给 printf(...)。
#define Debug_printf(format,...)定义了一个宏 [Debug_printf]支持可变参数；
printf(format,##__VA_ARGS__)调用 printf 输出格式化字符串和可变参数；
##__VA_ARGS__表示省略号部分的参数，若无参数则自动去掉前面的逗号。
*/
//包含文件名和行数，格式化输出调试信息
#define Debug_printf(format,...) printf("[%s:%d]: " format,__FILE_NAME__,__LINE__,##__VA_ARGS__)
//包含文件名和行数，格式化输出调试信息，并换行
#define Debug_printf_line(format,...) printf("[%s:%d]: " format "\n",__FILE_NAME__,__LINE__,##__VA_ARGS__)

#else
/*
这段代码在#else分支下定义了两个空宏[__FILE_NAME]和[__FILE_NAME__])，并定义了一个空的调试打印宏[Debug_printf]，其作用是：
- 在非调试环境下禁用文件名宏和调试输出  
- [Debug_printf][define Debug_printf_line]不执行任何操作，用于屏蔽调试信息输出
*/
#define __FILE_NAME 
#define __FILE_NAME__ 
#define Debug_printf(format,...) 
#define Debug_printf_line(format,...)

#endif

//初始化调试功能
void Com_Debug_Init(void);

#endif /* __COM_DEBUG_H__ */
