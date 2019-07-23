#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__

//**************************************************************
#define	_UART_ENABLE				ENABLE	//1: enable;	0: disable
#define _LED_FLASH_ENABLE			ENABLE//LED闪烁功能的预编译是都使能位,//可选:ENABLE,DISABLE
#define _LVD_LVDEN	 	 			DISABLE//可选:ENABLE,DISABLE
#define _UART_DEBUG					ENABLE  //(_UART_ENABLE && ENABLE) for debug msg


/* TIME */
#define C_TIME_100MS				50//50*2MS=100MS
#define C_TIME_1S					10//10*100MS=1S
#define C_TIME_05S					5	// 100MS*250=500MS
#define C_TIME_10S					100	// 100MS*100=10S


#define C_LED_FLASH_OFF				0
#define C_LED_FLASH_ON				1
#define C_LED_FLASH_CNT				6	//=2 LED浜伃涓�娆? =4 LED浜伃2娆? =6 LED浜伃3娆?浠ユ绫绘帹.
#define C_LED_FLASH_IMP				12	//浣撹剛娴嬭瘯瀹屾垚鍚庝綋閲嶅拰浣撹剛浜ゆ浛闂儊3娆?
#define C_LED_FLASH_DELAY			30  //100MS*30=3S

#endif //__MYDEFINE_H__
