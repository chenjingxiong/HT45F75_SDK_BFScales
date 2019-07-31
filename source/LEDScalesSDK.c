//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2015 BY HOLTEK SEMICONDUCTOR INC
//  File Name : Display.c
// Description: 显示程序
//   Customer :
//Targer Board:
//     MCU    :
//___________________________________________________________________
//___________________________________________________________________
#include "common.h"

//@@@@@@@@@@@@@@@@@@@@@@@@ 變量定義  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


volatile unsigned char gu8v_Weight_Unit;
volatile unsigned char gu8v_LED_Buffer[C_LED_BUFF_MAX];
volatile __byte_type   gu8v_LED_BufferPoint;
volatile unsigned char LEDScan_Cnt;	//顯示掃描計數
const unsigned char lu8v_LED_HEX[C_LED_CHAR_NUM] ={\
			//0,   1,   2,	 3,	  4,   5,   6,   7,   8,   9,   //数组下标
			 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F, //显示字符
			//'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

			//10,  11,  12,	 13,     14		  15		16,    17
    		 0x76,0x38,0x5C,0x40,	0xFF,    0x00,		0x73,	0x77
    	   //'H', 'L', 'O', '-',  'all on', 'all off',	'P',	'A' ,
    	 };

//由于硬件2COM的LED连接方式与另外三个COM不一样，所以需要再建个不同的显示数字表格.
const unsigned char lu8v_2COM_HEX[16] = {\
			//0,   1,   2,	 3,	  4,   5,   6,   7,   8,   9,   //数组下标
			 0x6F,0x41,0x76,0x73,0x59,0x3B,0x3F,0x61,0x7F,0x7B, //显示字符
			//'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

			//10,  11,  12,	 13,  14,      15
    		 0x5D,0x0E,0x17,0x10,0xFF,    0x00
    	   //'H', 'L', 'O', '-', 'all on', 'all off'
    	 };

#if 0
/********************************************************************
Function: HEX 轉換為4位BCD碼顯示
IO_IS_INPUT	:
OUTPUT	: gu8v_LED_Buffer[4] High → gu8v_LED_Buffer[4] Low
NOTE	: 最大顯示9999,超過萬位以上不顯示,比如10000,顯示0000,54321顯示4321
NOTE	: 此顯示用於LED秤上,通常有一位小數點,所以小於10,比如9顯示09,十位0會顯示
********************************************************************/
void fun_HEX2BCD4(unsigned int DisplayNum)
{
	unsigned char i,j;
	if (DisplayNum < 100)
	{
		gu8v_LED_Buffer[2] = LED_CHAR_OFF;
		gu8v_LED_Buffer[3] = LED_CHAR_OFF;
		j = 2;
	}
	else if (DisplayNum < 1000)
	{
		gu8v_LED_Buffer[3] = LED_CHAR_OFF;
		j = 3;
	}
	else
	{
		j = 4 ;
	}
	for(i=0;i<j;)
	{
		gu8v_LED_Buffer[i]=DisplayNum%10;
		DisplayNum/=10;
		i++;
	}
}
#endif

//===============================================================
//function: 设置所有显示buffer为1或0.
//input   : C_SEG_ON_ALL:所有buffer写1；C_SEG_OFF_ALL:所有buffer写0.
//output  : NULL
//description: 设置所有显示buffer为1或0.
//===============================================================
void Set_AllLEDBuffer(u8 OnOff)
{
	u8 i;
	if(OnOff){
		for(i = 0; i < C_LED_BUFF_MAX; i++){
			gu8v_LED_Buffer[i] = LED_CHAR_ALL;
		}
		fg_led_Byte = 0xFF;
	}else{
		for(i = 0; i < C_LED_BUFF_MAX; i++){
			gu8v_LED_Buffer[i] = LED_CHAR_OFF;
		}
		fg_led_Byte = 0x00;
	}
}



/********************************************************************
Function: HEX 转换为BCD码显示
INPUT	:显示数据,只能显示最低4位十进制:qian bai shi ge
OUTPUT	:
NOTE	:
********************************************************************/
void fun_HEX2BCD(u16 DisplayNum)
{
    u8 i = 0,j = 4;

	if(DisplayNum < 10){
			fg_led_piont1 = 1;
			fg_led_piont2 = 0;
			gu8v_LED_Buffer[NUM_QIAN] = LED_CHAR_OFF;
			gu8v_LED_Buffer[NUM_BAI]= LED_CHAR_OFF;
			gu8v_LED_Buffer[NUM_SHI]= DisplayNum/10;
			gu8v_LED_Buffer[NUM_GE] = DisplayNum%10;
	}else{
		fg_led_piont1 = 1;
		fg_led_piont2 = 0;
		if (DisplayNum < 100){
			gu8v_LED_Buffer[NUM_QIAN] = LED_CHAR_OFF;
			gu8v_LED_Buffer[NUM_BAI] = LED_CHAR_OFF;
			j = NUM_BAI;
		}else if (DisplayNum < 1000){
			gu8v_LED_Buffer[NUM_QIAN] = LED_CHAR_OFF;
			j = NUM_QIAN;
		}
		else if (DisplayNum >= 1000)	//
		{
	//		DisplayNum %= 1000;
		}

		for(i=0;i<j;)
		{
			gu8v_LED_Buffer[i]=DisplayNum%10;
			DisplayNum/=10;
			i++;
		}
	}



}

/********************************************************************
Function: 掃描顯示
IO_IS_INPUT	:
OUTPUT	:
NOTE	: 放置在Time中定時掃描,建議掃描週期2ms
********************************************************************/
 void fun_LEDBufScan()
{
	LEDCOM1 = LOW ;
	LEDCOM2 = LOW ;
	LEDCOM3 = LOW ;
	LEDCOM4 = LOW ;
	LEDCOMC1 = OUTPUT ;
	LEDCOMC2 = OUTPUT ;
	LEDCOMC3 = OUTPUT ;
	LEDCOMC4 = OUTPUT ;
	LEDSEGC	 = OUTPUT ;
	if(!fg_led_flash){

		switch (LEDScan_Cnt)
		{
			case 0:
				LEDSEG = lu8v_LED_HEX[gu8v_LED_Buffer[NUM_GE]];
				if(fg_led_unit_kg)
				{
					LEDSEG_UNIT_PIONT=HIGH;
				}
				LEDCOM1 = HIGH;
				LEDScan_Cnt = 1;
				break;

			case 1:
				LEDSEG = lu8v_2COM_HEX[gu8v_LED_Buffer[NUM_SHI]];
				if(fg_led_unit_lb)
				{
					LEDSEG_UNIT_PIONT=HIGH;
				}
				LEDCOM2 = HIGH;
				LEDScan_Cnt = 2;
				break;

			case 2:
				LEDSEG = lu8v_LED_HEX[gu8v_LED_Buffer[NUM_BAI]];
				if(fg_led_piont1)
				{
					LEDSEG_UNIT_PIONT = HIGH;
				}
				LEDCOM3 = HIGH;
				LEDScan_Cnt = 3;
				break;

			case 3:
				LEDSEG = lu8v_LED_HEX[gu8v_LED_Buffer[NUM_QIAN]];
				if(fg_led_piont2)
				{
					LEDSEG_UNIT_PIONT=HIGH;
				}
				LEDCOM4 = HIGH;
				LEDScan_Cnt = 0;
				break;

		default:
			LEDScan_Cnt = 0;
			break;
		}

		if(fg_led_ble)//蓝牙
		{
			P_LED_BLE = HIGH;
		}else{
			P_LED_BLE = LOW;
		}

		if(fg_led_unit_pct)//%
		{
			P_LED_UNIT_PCT = HIGH;
		}else{
			P_LED_UNIT_PCT = LOW;
		}
	}else{
		//LED全部关闭
		P_LED_UNIT_PCT = LOW;
		P_LED_BLE = LOW;
		LEDSEG_UNIT_PIONT = LOW;
		LEDSEG = LOW;
	}
}
/********************************************************************
Function: 單位轉換
IO_IS_INPUT	:
OUTPUT	:
NOTE	: 根據輸入的重量Jin,轉換為輸出對應單位的重量值
********************************************************************/
#if 0
unsigned int fun_WeightUnitChange(unsigned int weightJin)
{
	if (gu8v_Weight_Unit == UNIT_KG)
	{
		weightJin = weightJin/2;
	}
	if (gu8v_Weight_Unit == UNIT_LB)
	{
		weightJin = ((u32)weightJin/2*(u32)22046/1000+5)/10 & 0xfffe;
	}
	return weightJin;
}
#endif


