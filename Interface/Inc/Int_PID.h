#ifndef __INT_PID_H__
#define __INT_PID_H__

#include <stdlib.h>
#include "main.h"
#include "Int_BLDC.h"

// PID结构体
typedef struct
{
    float kp;
    float ki;
    float kd;
    float error;
    float error_last;
    float error_sum;
    float output;
} PID_t;

//限幅宏
#define LIMIT(x, min, max) (x < min ? min : (x > max ? max : x))

// PID结构体
extern PID_t pid_struct;

// 初始化PID结构体
void Int_PID_Init(void);

//PID计算
void Int_PID_Calculate(void);

#endif /* __INT_PID_H__ */