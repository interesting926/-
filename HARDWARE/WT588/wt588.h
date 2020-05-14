#ifndef __MUSIC_H
#define __MUSIC_H
#include "sys.h"

/*
#define MUSIC_DATA_PORT GPIOC
#define MUSIC_DATA_PIN GPIO_Pin_6
*/

#define MUSIC_DATA_PORT GPIOB
#define MUSIC_DATA_PIN GPIO_Pin_15

#define MUSIC_BUSY_PORT GPIOB
#define MUSIC_BUSY_PIN GPIO_Pin_14


#define MUSIC_DATA  PBout(15)
#define MUSIC_BUSY  PBin(14)
//#define MUSIC_DATA  PCout(6)


#define SPEAK_CTRL_PORT GPIOB
#define SPEAK_CTRL_PIN1 GPIO_Pin_13
#define SPEAK_CTRL_PIN2 GPIO_Pin_12



void MUSIC_GPIO_Init(void);
void gpio_test(void);
void MUSIC_Write_Cmd(u16 cmd);
void MUSIC_SET_VOLUMN(u8 vol);




void wt588_test(void);




void MP3_Init(void);
void MUSIC_START_PLAY(void);
void MUSIC_PLAY_CONTENT(u8 num);
void MUSIC_STOP_PLAY(void);
void MUSIC_CLEAR_QUEUE(void);
void MUSIC_ADD_CONTENT(u8 num);
void MUSIC_ADD_NUMBER_CONTNET(u16 num);
void MUSIC_ADD_INT_CONTENT(u16 num);
void MUSIC_SET_VOLUMN(u8 vol);



#endif

