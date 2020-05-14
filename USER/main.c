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
	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	
	KEY_Init();         	//初始化与按键连接的硬件接口
	EXTIX_Init();		 	//外部中断初始化
	MUSIC_GPIO_Init();
	uart_init(9600);
	TIM3_Int_Init(9999,7199);//10Khz的计数频率，计数到10000为1s 
	TIM2_Int_Init(999,7199);//10Khz的计数频率，计数到1000为100ms 
    MP3_Init();
    Init_System();
    MUSIC_ADD_CONTENT(17);//欢迎
    //MUSIC_ADD_CONTENT(17);//欢迎
	//MUSIC_PLAY_CONTENT(17);
    //wt588_test();
	while(1)
	{
        function();

		/*
		LED0=0;
		delay_ms(300);	 //延时300ms
		LED0=1;
		delay_ms(300);	//延时300ms
		//uart_test();*/
		
	}
 }



 