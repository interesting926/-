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
#include "string.h"



int times=0;
int len=0;
int t=0;

void uart_test(void)
{
    if(USART_RX_STA&0x8000)
    {					   
    	len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
    	printf("\r\n您发送的消息为:\r\n\r\n");
    	for(t=0;t<len;t++)
    	{
    		USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
    		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
    	}
    	printf("\r\n\r\n");//插入换行
    	USART_RX_STA=0;
    }else
    {
    	times++;
    	if(times%5000==0)
    	{
    		printf("\r\n战舰STM32开发板 串口实验\r\n");
    		printf("正点原子@ALIENTEK\r\n\r\n");
    	}
    	if(times%200==0)printf("请输入数据,以回车键结束\n");  
    	delay_ms(10);   
    }


}



void Init_System(void)
{
    System.refreshFlag =1;
    System.step_delay=0;
    System.step =6;
    
}


void Init_RPM(void)
{



}

void time_count_hander(void)
{
    //System.refreshFlag =1;
    //System.step_delay=0;
    if (System.step <=5 )System.step++;
}


void RPM_Timeticket(void)
{
	if(rpm.filter) rpm.filter--;    //PEM消抖
	if(rpm.timeout) rpm.timeout--;  //划船超时
	//if(rpm.delay_cnt) rpm.delay_cnt--;
	//if(rpm.time_cnt) rpm.time_cnt--;
	if(mp3.dely) mp3.dely--;
}



void System_step_hander(void)
{

    if(System.step_delay == 0)
    {

        switch(System.step)
        {
            /*
            case 0:
                MUSIC_ADD_CONTENT(17);//欢迎
                System.step_delay=7;
                break;*/
                                  
            case 1:
                MUSIC_ADD_CONTENT(30);//海浪声5秒
                //MUSIC_ADD_CONTENT(23);//5秒后开始起航
                System.step_delay=30;
                break;
                
            //经过1分钟之后开始    
            case 2:
                MUSIC_ADD_CONTENT(24);//今天天气不错
                MUSIC_ADD_CONTENT(29);//让我们荡起双桨
                System.step_delay=50;
                break;

            case 3:
                MUSIC_ADD_CONTENT(25);//注意前方水流急
                MUSIC_ADD_CONTENT(26);//急流5S
                MUSIC_ADD_CONTENT(30);//海鸥声
                System.step_delay=30;
                break;
            case 4:
                MUSIC_ADD_CONTENT(27);//小朋友前方要变航
                MUSIC_ADD_CONTENT(29);//儿童音乐30S
                System.step_delay=30;
                break;

            case 5:
                MUSIC_ADD_CONTENT(28);//目的地要到了
                System.step_delay=30;
                break;

                
            //最后一步播报结果
            case 6:
                if(rpm.total_cnts)//有结果才报
                {
                    MUSIC_CLEAR_QUEUE();    //播报结果
                    mp3.ChangeNow =1;
                    //System.step =0;        //重置步骤
                    MUSIC_ADD_CONTENT(18);//您本次的运动结果为
                    MUSIC_ADD_INT_CONTENT(rpm.total_cnts);
                    MUSIC_ADD_CONTENT(15); //次
                    MUSIC_ADD_CONTENT(21); //卡路里
                    MUSIC_ADD_INT_CONTENT(rpm.calorie);
                    MUSIC_ADD_CONTENT(22); //欢迎下次使用

                    rpm.total_cnts=0;
                    rpm.calorie=0;
                }
                break;
        }
    }

}


void goto_step(int step)
{
    System.step =step;
    System.step_delay =0;

}

void RPM_Loop_Handler()
{
    if(rpm.int_flg==1)
    {
    	rpm.int_flg=0;
    	if(rpm.filter==0)
    	{
    		rpm.filter=3;
    		//rpm.cnt++;
    		rpm.total_cnts++;

    		if(rpm.start_flg==0) //开始计时标志 没人划船到有人划船开始
    		{
                
                goto_step(0);   //开始
    			//rpm.total_cnts=0;
    			//rpm.calorie=0;
    			//timer.total_time=0;
    			rpm.start_flg=1;

    		}
    	}
    	rpm.calorie=rpm.total_cnts/17;

        rpm.timeout=Timeout_Max;	//超时时间

    }


	if(rpm.timeout==0)
	{
        rpm.start_flg = 0;

        // MUSIC_CLEAR_QUEUE();
        //播报结果
        goto_step(6); //播报结果
	}


}




void Music_Play_Handler(void)
{
    if(mp3.queue_play)//如果开始播放队列里的音乐
    {
		if(((MUSIC_BUSY == SET)&&(mp3.dely ==0))||(mp3.ChangeNow != 0)) //当播放完一首或者强行切歌
		{
            mp3.ChangeNow =0;
            mp3.queue_play --;
			MUSIC_Write_Cmd(mp3.queue[mp3.QueueIndex]);
            //注意是 ++++mp3.QueueIndex 而不是++mp3.QueueIndex++ 否则判断会慢一步导致报错
			if ((++mp3.QueueIndex) == mp3_buff_size ) mp3.QueueIndex =0; 
            mp3.dely =2;
		} 			
    }
}




void function(void)
{
    Music_Play_Handler();
    System_step_hander();
    RPM_Loop_Handler();
}



















