#ifndef __INT_PID_H__
#define __INT_PID_H__

#include <stdlib.h>
#include "main.h"
#include "Int_BLDC.h"

// PID�ṹ��
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

//�޷���
#define LIMIT(x, min, max) (x < min ? min : (x > max ? max : x))

// PID�ṹ��
extern PID_t pid_struct;

// ��ʼ��PID�ṹ��
void Int_PID_Init(void);

//PID����
void Int_PID_Calculate(void);

#endif /* __INT_PID_H__ */