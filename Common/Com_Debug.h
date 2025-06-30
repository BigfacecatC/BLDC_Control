#ifndef __COM_DEBUG_H__
#define __COM_DEBUG_H__

#include "usart.h"
#include <stdarg.h>
#include <string.h>

//�����˺궨�壬���ɿ������Թ��ܣ��ڿ�������ʱ�رմ˺궨�壬�Ϳɹرյ��Թ��ܡ�
//#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE

/*
__FILE_NAME�궨�壬���ڻ�ȡ��ǰԴ�ļ�������ȥ��·����"\"ǰ�����ݡ�
*/
#define __FILE_NAME (strrchr((char *)__FILE__, '\\')?strrchr((char *)__FILE__, '\\') + 1:__FILE__) 
/*
__FILE_NAME__�궨�壬���ڻ�ȡ��ǰԴ�ļ�������ȥ��·��"/"ǰ�����ݡ�ע��˺궨���ǻ���__FILE_NAME�궨��ȥ��·����"\"ǰ�����ݺ�Ľ����
*/
#define __FILE_NAME__ (strrchr((char *)__FILE_NAME, '/')?strrchr((char *)__FILE_NAME, '/') + 1:__FILE_NAME)  

/*
������Դ�ӡ�꣬���ڸ�ʽ�����������Ϣ��������������ǽ� Debug_printf(...) ����ֱ��ת���� printf(...)��
#define Debug_printf(format,...)������һ���� [Debug_printf]֧�ֿɱ������
printf(format,##__VA_ARGS__)���� printf �����ʽ���ַ����Ϳɱ������
##__VA_ARGS__��ʾʡ�ԺŲ��ֵĲ��������޲������Զ�ȥ��ǰ��Ķ��š�
*/
//�����ļ�������������ʽ�����������Ϣ
#define Debug_printf(format,...) printf("[%s:%d]: " format,__FILE_NAME__,__LINE__,##__VA_ARGS__)
//�����ļ�������������ʽ�����������Ϣ��������
#define Debug_printf_line(format,...) printf("[%s:%d]: " format "\n",__FILE_NAME__,__LINE__,##__VA_ARGS__)

#else
/*
��δ�����#else��֧�¶����������պ�[__FILE_NAME]��[__FILE_NAME__])����������һ���յĵ��Դ�ӡ��[Debug_printf]���������ǣ�
- �ڷǵ��Ի����½����ļ�����͵������  
- [Debug_printf][define Debug_printf_line]��ִ���κβ������������ε�����Ϣ���
*/
#define __FILE_NAME 
#define __FILE_NAME__ 
#define Debug_printf(format,...) 
#define Debug_printf_line(format,...)

#endif

//��ʼ�����Թ���
void Com_Debug_Init(void);

#endif /* __COM_DEBUG_H__ */
