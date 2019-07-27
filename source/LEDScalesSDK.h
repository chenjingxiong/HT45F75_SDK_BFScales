//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2015 BY HOLTEK SEMICONDUCTOR INC
//  File Name : Display.c
// Description: 显示程序
//   Customer : Holtek Demo Code
//Targer Board: PICOOC_Display
//     MCU    : BH66F5233
//___________________________________________________________________
//___________________________________________________________________
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 共用變量 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
extern volatile bit gbv_ADC_INSIDE_MDOE;
extern volatile bit gbv_Cal_Sucess;
extern volatile unsigned char 	LEDScan_Cnt;	//顯示掃描計數
extern volatile unsigned char 	gu8v_LED_Buffer[4];
extern volatile __byte_type 	gu8v_LED_BufferPoint;
#define fg_led_Byte    			gu8v_LED_BufferPoint.u8
#define fg_led_nosue	     	gu8v_LED_BufferPoint.bits.b7//nosue
#define fg_led_ble     	    	gu8v_LED_BufferPoint.bits.b6
#define fg_led_piont2      		gu8v_LED_BufferPoint.bits.b5
#define fg_led_piont1      		gu8v_LED_BufferPoint.bits.b4
#define fg_led_unit_pct	     	gu8v_LED_BufferPoint.bits.b3// nosue
#define fg_led_unit_jin	     	gu8v_LED_BufferPoint.bits.b2// nosue
#define fg_led_unit_lb	     	gu8v_LED_BufferPoint.bits.b1
#define fg_led_unit_kg 	    	gu8v_LED_BufferPoint.bits.b0
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 共用函數 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//void fun_HEX2BCD4(unsigned int DisplayNum);
void fun_HEX2BCD(unsigned int DisplayNum);
extern void Set_AllLEDBuffer(u8 OnOff);
void fun_LEDBufScan();
unsigned int fun_WeightUnitChange(unsigned int weightJin);
void fun_Display_PownDown();
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ IO congfig @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 預定義 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define LED_CHAR_0					0
#define LED_CHAR_1					1
#define LED_CHAR_2					2
#define LED_CHAR_3					3
#define LED_CHAR_4					4
#define LED_CHAR_5					5
#define LED_CHAR_6					6
#define LED_CHAR_7					7
#define LED_CHAR_8					8
#define LED_CHAR_9					9
#define LED_CHAR_H 					10
#define LED_CHAR_L					11
#define LED_CHAR_O					12
#define LED_CHAR__					13
#define LED_CHAR_ALL				14
#define LED_CHAR_OFF				15
#define LED_CHAR_P					16
#define LED_CHAR_A					17
#define C_LED_CHAR_NUM				18

//#define LED_CHAR_P					14
//#define LED_CHAR_A					15
//#define LED_CHAR_ALL				16
//#define LED_CHAR_OFF				17
//#define C_LED_CHAR_NUM				18

#define C_LED_BUFF_MAX				4

//qian bai shi ge 千百十个,这几个数必须为数组的有效范围下标gu8v_LED_buffer[],即小于C_LED_BUF_MAX
#define NUM_GE						0
#define NUM_SHI						1
#define NUM_BAI						2
#define NUM_QIAN					3

//********************************重量单位预定义*******************************************************
#define UNIT_JIN    0x00
#define UNIT_KG     0x01
#define UNIT_LB 	0x02
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ User Define @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define CurrentPB_PA_SLEDMAX() 	{_sledc0 = 0xff;}
#define CurrentPB_PA_SLEDL3() 	{_sledc0 = 0xaa;}
#define CurrentPB_PA_SLEDL2() 	{_sledc0 = 0x55;}
#define CurrentPB_PA_SLEDMin() 	{_sledc0 = 0x00;}
//                           @--------------SLEDC1---------------@
#define CurrentPC_PD_SLEDMAX() 	{_sledc1 = 0x3f;}
#define CurrentPC_PD_SLEDL3() 	{_sledc1 = 0xaa;}
#define CurrentPC_PD_SLEDL2() 	{_sledc1 = 0x55;}
#define CurrentPC_PD_SLEDMin() 	{_sledc1 = 0x00;}
#endif
