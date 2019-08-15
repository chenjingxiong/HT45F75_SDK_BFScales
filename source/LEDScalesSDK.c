#include "common.h"

volatile unsigned char gu8v_Weight_Unit;
volatile unsigned char gu8v_LED_Buffer[C_LED_BUFF_MAX];
volatile __byte_type   gu8v_LED_BufferPoint;
volatile unsigned char LEDScan_Cnt;	//顯示掃描計數
const unsigned char lu8_LED_HEX[C_LED_CHAR_NUM] ={\
			//0,   1,   2,	 3,	  4,   5,   6,   7,   8,   9,   //数组下标
			 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F, //显示字符
			//'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

			//10,  11,  12,	 13,     14		  15		16,    17,	  18,	19
    		 0x76,0x38,0x5C,0x40,	0xFF,    0x00,		0x77,	0x73, 0x71, 0x30
    	   //'H', 'L', 'O', '-',  'all on', 'all off',	'A',	'P',  'F',  'I'
    	 };

//由于硬件2COM的LED连接方式与另外三个COM不一样，所以需要再建个不同的显示数字表.
const unsigned char lu8_2COM_HEX[C_LED_CHAR_NUM2] = {\
			//0,   1,   2,	 3,	  4,   5,   6,   7,   8,   9,   //数组下标
			 0x6F,0x41,0x76,0x73,0x59,0x3B,0x3F,0x61,0x7F,0x7B, //显示字符
			//'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

			//10,  11,  12,	 13,  14,    	15   		16
    		 0x5D,0x0E,0x17,0x10, 0xFF, 	0x00, 		0x7D
    	   //'H', 'L', 'O', '-', 'all on', 'all off',	 'A'
    	 };


//===============================================================
//function: 设置所有显示buffer.
//input   : C_SEG_ON_ALL;C_SEG_OFF_ALL;
//output  : NULL
//description: 设置所有显示buffer.
//===============================================================
void Set_AllLEDBuffer(u8 OnOff)
{
	u8 i;
	if(OnOff){
		for(i = 0; i < C_LED_BUFF_MAX; i++){
			gu8v_LED_Buffer[i] = LED_CHAR_ALL;
		}
		flag_led_Byte = 0xFF & 0xB7;//上电全亮3s,蓝牙图标和%号由于太亮
	}else{
		for(i = 0; i < C_LED_BUFF_MAX; i++){
			gu8v_LED_Buffer[i] = LED_CHAR_OFF;
		}
		flag_led_Byte = 0x00;
	}
}



/********************************************************************
Function: HEX 转换为BCD码显示.
INPUT	:显示数据,只能显示最多4位十进制:qian bai shi ge
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
Function: 單位轉換
IO_IS_INPUT	:
OUTPUT	:
NOTE	: 根據輸入的重量Jin,轉換為輸出對應單位的重量�?
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


