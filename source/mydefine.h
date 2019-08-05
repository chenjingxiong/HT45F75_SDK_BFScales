#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__

//**************************************************************
#define	_UART_ENABLE				ENABLE	//1: enable;	0: disable
#define _LED_FLASH_ENABLE			ENABLE//LED��˸���ܵ�Ԥ�����Ƕ�ʹ��λ,//��ѡ:ENABLE,DISABLE
#define _LVD_LVDEN	 	 			DISABLE//��ѡ:ENABLE,DISABLE
#define _UART_DEBUG					ENABLE  //(_UART_ENABLE && ENABLE) for debug msg


/* TIME */
#define C_TIME_100MS				50//50*2MS=100MS
#define C_TIME_1S					10//10*100MS=1S
#define C_TIME_05S					5	// 100MS*250=500MS
#define C_TIME_10S					100	// 100MS*100=10S


#define C_LED_FLASH_OFF				0
#define C_LED_FLASH_ON				1
#define C_LED_FLASH_CNT				6	//=2 LED亮灭一�? =4 LED亮灭2�? =6 LED亮灭3�?以此类推.
#define C_LED_FLASH_IMP				12	//体脂测试完成后体重和体脂交替闪烁3�?
#define C_LED_FLASH_DELAY			30  //100MS*30=3S

#endif //__MYDEFINE_H__
