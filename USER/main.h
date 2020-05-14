#ifndef __MAIN_h_
#define __MAIN_H_





typedef struct{

    int step;
    int step_delay;  //�����ƴ�
    int refreshFlag;
    int sysStatus;
}SYSTEM;

extern SYSTEM System;

typedef struct{
	u16 calorie;
	u16 total_cnts;		//�ܴ���
	u8 start_flg:1;		//��ʼ��ʱ
	u8 int_flg:1;    //�жϱ�־
	u8 upload_flg:1;
	u16 time_cnt;	//��ʱ
	u16 timeout;	//��ʱ����
	u8 state:1;
	u8 filter;
	u8 delay_cnt;
	u8 cnt;
} SPORT;

extern SPORT rpm;


#define mp3_next 4  //ǿ���л�����
#define mp3_replay 3
#define mp3_playing 1	//����������
#define mp3_pause 2 	//������ͣ
#define mp3_stop 0	//�������
#define mp3_buff_size 20


typedef struct 
{	
	u8 dely;
	u8 volumn;
	u8 status,busy;
	u8 queue[mp3_buff_size];
	u8 QueueIndex,QueueSize,queue_play,ChangeNow;
    u8 loadAdd;
}MP3_TYPE_DEF;



extern MP3_TYPE_DEF mp3;



#endif


