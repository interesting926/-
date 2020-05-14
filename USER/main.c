#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "Exti.h"
#include "timer.h"
#include "wt588.h"
#include "main.h"
#include "function.h"
 



SYSTEM System;
SPORT rpm;
MP3_TYPE_DEF mp3;

 int main(void)
 {	
	SystemInit();
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	KEY_Init();         	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	EXTIX_Init();		 	//�ⲿ�жϳ�ʼ��
	MUSIC_GPIO_Init();
	uart_init(9600);
	TIM3_Int_Init(9999,7199);//10Khz�ļ���Ƶ�ʣ�������10000Ϊ1s 
	TIM2_Int_Init(999,7199);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ100ms 
    MP3_Init();
    Init_System();
    MUSIC_ADD_CONTENT(17);//��ӭ
    //MUSIC_ADD_CONTENT(17);//��ӭ
	//MUSIC_PLAY_CONTENT(17);
    //wt588_test();
	while(1)
	{
        function();

		/*
		LED0=0;
		delay_ms(300);	 //��ʱ300ms
		LED0=1;
		delay_ms(300);	//��ʱ300ms
		//uart_test();*/
		
	}
 }



 