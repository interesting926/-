#ifndef __MAIN_h_
#define __MAIN_H_





typedef struct{

    int step;
    int step_delay;  //秒数计次
    int refreshFlag;
    int sysStatus;
}SYSTEM;

extern SYSTEM System;

typedef struct{
	u16 calorie;
	u16 total_cnts;		//总次数
	u8 start_flg:1;		//开始计时
	u8 int_flg:1;    //中断标志
	u8 upload_flg:1;
	u16 time_cnt;	//计时
	u16 timeout;	//超时待机
	u8 state:1;
	u8 filter;
	u8 delay_cnt;
	u8 cnt;
} SPORT;

extern SPORT rpm;


#define mp3_next 4  //强行切换歌曲
#define mp3_replay 3
#define mp3_playing 1	//单曲播放中
#define mp3_pause 2 	//单曲暂停
#define mp3_stop 0	//单曲完毕
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


