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
    	len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
    	printf("\r\n�����͵���ϢΪ:\r\n\r\n");
    	for(t=0;t<len;t++)
    	{
    		USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
    		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
    	}
    	printf("\r\n\r\n");//���뻻��
    	USART_RX_STA=0;
    }else
    {
    	times++;
    	if(times%5000==0)
    	{
    		printf("\r\nս��STM32������ ����ʵ��\r\n");
    		printf("����ԭ��@ALIENTEK\r\n\r\n");
    	}
    	if(times%200==0)printf("����������,�Իس�������\n");  
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
	if(rpm.filter) rpm.filter--;    //PEM����
	if(rpm.timeout) rpm.timeout--;  //������ʱ
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
                MUSIC_ADD_CONTENT(17);//��ӭ
                System.step_delay=7;
                break;*/
                                  
            case 1:
                MUSIC_ADD_CONTENT(30);//������5��
                //MUSIC_ADD_CONTENT(23);//5���ʼ��
                System.step_delay=30;
                break;
                
            //����1����֮��ʼ    
            case 2:
                MUSIC_ADD_CONTENT(24);//������������
                MUSIC_ADD_CONTENT(29);//�����ǵ���˫��
                System.step_delay=50;
                break;

            case 3:
                MUSIC_ADD_CONTENT(25);//ע��ǰ��ˮ����
                MUSIC_ADD_CONTENT(26);//����5S
                MUSIC_ADD_CONTENT(30);//��Ÿ��
                System.step_delay=30;
                break;
            case 4:
                MUSIC_ADD_CONTENT(27);//С����ǰ��Ҫ�亽
                MUSIC_ADD_CONTENT(29);//��ͯ����30S
                System.step_delay=30;
                break;

            case 5:
                MUSIC_ADD_CONTENT(28);//Ŀ�ĵ�Ҫ����
                System.step_delay=30;
                break;

                
            //���һ���������
            case 6:
                if(rpm.total_cnts)//�н���ű�
                {
                    MUSIC_CLEAR_QUEUE();    //�������
                    mp3.ChangeNow =1;
                    //System.step =0;        //���ò���
                    MUSIC_ADD_CONTENT(18);//�����ε��˶����Ϊ
                    MUSIC_ADD_INT_CONTENT(rpm.total_cnts);
                    MUSIC_ADD_CONTENT(15); //��
                    MUSIC_ADD_CONTENT(21); //��·��
                    MUSIC_ADD_INT_CONTENT(rpm.calorie);
                    MUSIC_ADD_CONTENT(22); //��ӭ�´�ʹ��

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

    		if(rpm.start_flg==0) //��ʼ��ʱ��־ û�˻��������˻�����ʼ
    		{
                
                goto_step(0);   //��ʼ
    			//rpm.total_cnts=0;
    			//rpm.calorie=0;
    			//timer.total_time=0;
    			rpm.start_flg=1;

    		}
    	}
    	rpm.calorie=rpm.total_cnts/17;

        rpm.timeout=Timeout_Max;	//��ʱʱ��

    }


	if(rpm.timeout==0)
	{
        rpm.start_flg = 0;

        // MUSIC_CLEAR_QUEUE();
        //�������
        goto_step(6); //�������
	}


}




void Music_Play_Handler(void)
{
    if(mp3.queue_play)//�����ʼ���Ŷ����������
    {
		if(((MUSIC_BUSY == SET)&&(mp3.dely ==0))||(mp3.ChangeNow != 0)) //��������һ�׻���ǿ���и�
		{
            mp3.ChangeNow =0;
            mp3.queue_play --;
			MUSIC_Write_Cmd(mp3.queue[mp3.QueueIndex]);
            //ע���� ++++mp3.QueueIndex ������++mp3.QueueIndex++ �����жϻ���һ�����±���
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



















