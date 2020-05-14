#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define Timeout_Max 50     //10S

void time_count_hander(void);
void System_step_hander(void);


void RPM_Timeticket(void);//每隔100ms需要处理的计数
void Music_Play_Handler(void);
void function(void);
void Init_System(void);



#endif




