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
#define C_TIME_05S					5	// 100MS*5=500MS
#define C_TIME_3S					30	// 100MS*30=3S
#define C_TIME_5S					50	// 100MS*100=10S
#define C_TIME_10S					100	// 100MS*100=10S
//#define C_TIME_10S					10	// 100MS*10=1S

#define C_LED_FLASH_OFF				0
#define C_LED_FLASH_ON				1
#define C_LED_FLASH_CNT				6	//=2 LED亮灭一�? =4 LED亮灭2�? =6 LED亮灭3�?以此类推.
#define C_LED_FLASH_IMP				12	//体脂测试完成后体重和体脂交替闪烁3�?
#define C_LED_FLASH_DELAY			30  //100MS*30=3S

//WORK MODE
#define TASK_STARTUP				0
#define TASK_LOWBATTERY				1
#define TASK_SCALES					2
#define TASK_SLEEP					3
#define TASK_WEIGHT_AUTOON			4


//DISPLAY MODE
#define DISPLAY_POWERON				0
#define DISPLAY_LOWBATTERY				1//低电
#define DISPLAY_NOLOAD				2//空载 当前重量小于最小称重重量，显示0.0,10s后关�?
#define DISPLAY_LOADUP				3//完成一次称�?
#define DISPLAY_LOADFIX				4//完成一次稱重測量后重量穩定沒有解鎖
#define DISPLAY_LOADDOWN			5//下秤動作
#define DISPLAY_OVERLOAD			6//超重
#define DISPLAY_IMPEDANCEING		7
#define DISPLAY_IMPEDANCE_FINISH	8//测试阻抗
#define DISPLAY_CAL					9//校准
#define DISPLAY_CALPASS				10//校准成功.
#define DISPLAY_CALFAIL				11
#define DISPLAY_ALLOFF				12//关闭所有显示LED
#define DISPLAY_MAX					13


//阻抗测试错误
//阻抗 - 阻抗�?單位ohm. 若為(0xffff-無待測阻抗錯�?/(0xFFF1-接觸異常錯誤)/(0xFFF2-阻抗超出範圍)
#define IMPEDANCE_ERR_NOTEST					0xFFFF//(0xffff-無待測阻抗錯�?
#define IMPEDANCE_ERR_ABNORMAL					0xFFF1//(0xFFF1-接觸異常錯誤)
#define IMPEDANCE_ERR_OUTRANGE					0xFFF2//(0xFFF2-阻抗超出範圍)

#endif //__MYDEFINE_H__
