#ifndef __INT_BLDC_H__
#define __INT_BLDC_H__

#include "tim.h"
#include "main.h"
#include "Com_Debug.h"
#include "Int_PID.h"

void Int_BLDC_Start(void);

void Int_BLDC_Stop(void);

void Int_BLDC_Update_Speed(int16_t set_speed);

//声明一个函数指针,表示绕组控制函数
typedef void (*BLDC_Control)(void);

extern uint8_t motor_id_addr;
extern uint8_t motor_status;
extern uint8_t motor_dir;
extern int16_t measure_motor_speed ;
extern uint8_t motor_id;
extern int16_t set_motor_speed;
extern uint16_t last_hall_status;
extern uint16_t current_hall_status;
extern uint32_t timer_overflow_count;
extern uint32_t timer_overflow_final_count;
extern uint8_t hall_status_change_count;

#endif /* __INT_BLDC_H__ */