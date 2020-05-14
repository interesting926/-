#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "wt588.h"
#include "led.h"
#include "main.h"







void MP3_Init(void)
{
	mp3.QueueSize=0;
	mp3.QueueIndex=0;
	mp3.status = mp3_stop;
	mp3.queue_play=0;
    MUSIC_SET_VOLUMN(1);
}


void MUSIC_START_PLAY(void)
{
		mp3.QueueIndex=0;
		mp3.ChangeNow=1;	//立即播放
		mp3.queue_play=1;
}


void MUSIC_PLAY_CONTENT(u8 num)
{
		mp3.queue[0]=num;
		mp3.QueueSize=1;
		mp3.QueueIndex=0;
		mp3.ChangeNow=1;	//立即播放
		mp3.queue_play=1; 			
}
void MUSIC_STOP_PLAY(void)
{
		mp3.status = mp3_stop;
		mp3.queue_play=0;
}

void MUSIC_CLEAR_QUEUE(void)
{
		mp3.QueueSize=0;
		mp3.ChangeNow=0;
		mp3.QueueIndex=0;
		mp3.status = mp3_stop;
		mp3.queue_play=0;
}



void MUSIC_ADD_NUMBER_CONTNET(u16 num)
{
	switch(num)
	{
		case 0:	MUSIC_ADD_CONTENT(0);break;
		case 1:	MUSIC_ADD_CONTENT(1);break;
		case 2:	MUSIC_ADD_CONTENT(2);break;
		case 3:	MUSIC_ADD_CONTENT(3);break;
		case 4:	MUSIC_ADD_CONTENT(4);break;
		case 5:	MUSIC_ADD_CONTENT(5);break;
		case 6:	MUSIC_ADD_CONTENT(6);break;
		case 7:	MUSIC_ADD_CONTENT(7);break;
		case 8:	MUSIC_ADD_CONTENT(8);break;
		case 9:	MUSIC_ADD_CONTENT(9);break;
		case 10: MUSIC_ADD_CONTENT(10);break;
		case 100: MUSIC_ADD_CONTENT(11);break;
		case 1000: MUSIC_ADD_CONTENT(12);break;
	}

}


void MUSIC_ADD_INT_CONTENT(u16 num)
{
	u8 qian,bai,shi,ge;
	qian = (num/1000)%10;
	bai = (num/100)%10;
	shi = (num/10)%10;
	ge = (num/1)%10;
	if(num<10)
	{
		MUSIC_ADD_NUMBER_CONTNET(ge);
	}
	else if(num==10)
	{
		MUSIC_ADD_NUMBER_CONTNET(10);
	}
	else if(num<=19)
	{
		MUSIC_ADD_NUMBER_CONTNET(10);
		if(ge)	MUSIC_ADD_NUMBER_CONTNET(ge);
	}
	else if(num<=99) 
	{
		MUSIC_ADD_NUMBER_CONTNET(shi);
		MUSIC_ADD_NUMBER_CONTNET(10);
		if(ge)
		MUSIC_ADD_NUMBER_CONTNET(ge);
	}
	else if(num<=999)
	{
		MUSIC_ADD_NUMBER_CONTNET(bai);	//百位
		MUSIC_ADD_NUMBER_CONTNET(100);	//百
		if(num%100)
		{
			MUSIC_ADD_NUMBER_CONTNET(shi);	//十位
			if(shi)
			{
				MUSIC_ADD_NUMBER_CONTNET(10);	//十
				if(ge)
				{
					MUSIC_ADD_NUMBER_CONTNET(ge);	//个位
				}
			}
			else
			{
				MUSIC_ADD_NUMBER_CONTNET(ge);	//个位
			}
		}
	}
	else if(num<=9999)
	{
		MUSIC_ADD_NUMBER_CONTNET(qian);
		MUSIC_ADD_NUMBER_CONTNET(1000);
		if(num%1000)
		{
			MUSIC_ADD_NUMBER_CONTNET(bai);	//百位
			if(bai)
			{
				MUSIC_ADD_NUMBER_CONTNET(100);	//百
			}
			if(num%100)
			{
				if(shi)
				{
					MUSIC_ADD_NUMBER_CONTNET(shi);	//十位
					MUSIC_ADD_NUMBER_CONTNET(10);	//十
					if(ge)
					{
						MUSIC_ADD_NUMBER_CONTNET(ge);	//个位
					}
				}
				else
				{	
					if(bai)	MUSIC_ADD_NUMBER_CONTNET(shi);	//十位
					MUSIC_ADD_NUMBER_CONTNET(ge);	//个位
				}
			}
		}
	}	
}



void MUSIC_GPIO_Init()
{

 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

    GPIO_InitStructure.GPIO_Pin = MUSIC_BUSY_PIN; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//复用推挽输出
    GPIO_Init(MUSIC_BUSY_PORT, &GPIO_InitStructure);//初始化GPIOA.9

	GPIO_InitStructure.GPIO_Pin = MUSIC_DATA_PIN; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
    GPIO_Init(MUSIC_DATA_PORT, &GPIO_InitStructure);//初始化GPIOA.9

	GPIO_InitStructure.GPIO_Pin = SPEAK_CTRL_PIN1|SPEAK_CTRL_PIN2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
    GPIO_Init(SPEAK_CTRL_PORT, &GPIO_InitStructure);//初始化GPIOA.9

    //ht6873需要把CTRL1和CTRL2都设置成高电平
    GPIO_SetBits(SPEAK_CTRL_PORT,SPEAK_CTRL_PIN1);
	GPIO_SetBits(SPEAK_CTRL_PORT,SPEAK_CTRL_PIN2);

}


void MUSIC_Write_Cmd(u16 cmd)
{
	u8 i;
	static u8 LowByte,HighByte;
	HighByte=cmd>>8;
	LowByte=cmd&0xff;


	MUSIC_DATA=0;
    delay_us(5000);
	for(i=0;i<8;i++)
	{
		if(HighByte&0x01)
		{

            MUSIC_DATA=1;
			delay_us(600);
			MUSIC_DATA=0;
			delay_us(200);
		}
		else
		{
			MUSIC_DATA=1;
			delay_us(200);
			MUSIC_DATA=0;
			delay_us(600);
		}
		HighByte>>=1;
	}

    MUSIC_DATA=1;
	delay_us(2000);
	MUSIC_DATA=0;
	delay_us(5000);
	for(i=0;i<8;i++)
	{
		if(LowByte&0x01)
		{
			MUSIC_DATA=1;
			delay_us(600);
			MUSIC_DATA=0;
			delay_us(200);
		}
		else
		{
			MUSIC_DATA=1;
			delay_us(200);
			MUSIC_DATA=0;
			delay_us(600);
		}
		LowByte>>=1;
	}
	MUSIC_DATA=1;
}


void MUSIC_SET_VOLUMN(u8 vol)
{

	MUSIC_Write_Cmd(0xFFE0+vol);//调整音量
	delay_ms(200);
}



void wt588_test(void)
{

    /*while(1)
    {
        LED0 = ~LED0;
        MUSIC_ADD_INT_CONTENT(1024);
        delay_ms(1800);  //延时300ms

        Music_Play_Handler();
    }*/
}




void MUSIC_ADD_CONTENT(u8 num)
{
    mp3.queue_play++;    //未播放音乐数量+1
	mp3.queue[mp3.QueueSize++]=num;   //数组中存储的地址+1
    if(mp3.QueueSize >= mp3_buff_size) mp3.QueueSize =0;    //越界则回到0位置
}





