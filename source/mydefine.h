#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__

//**************************************************************
#define	_UART_ENABLE				ENABLE	//1: enable;	0: disable
#define _UART_DEBUG					DISABLE//ENABLE  //(_UART_ENABLE && ENABLE) for debug msg


/* TIME */
#define C_TIME_100MS				50  // 50*2MS=100MS
#define C_TIME_1S					10  // 10*100MS=1S
#define C_TIME_05S					5	// 100MS*5=500MS
#define C_TIME_3S					30	// 100MS*30=3S
#define C_TIME_5S					50	// 100MS*100=10S
#define C_TIME_10S					100	// 100MS*100=10S
#define C_TIME_30S					30  // 1S*30=30S 看门狗唤醒计时.

/* LED FLASH */
#define C_LED_FLASH_OFF				0
#define C_LED_FLASH_ON				1
#define C_LED_FLASH_CNT				6	//=2 LED亮灭一次,eg: =4 LED亮灭各2次; =6 LED亮灭各3以此类推.
#define C_LED_FLASH_IMP				12	//体脂测试完成后体重和体脂交替闪烁3次.
#define C_LED_FLASH_DELAY			0   //30  //100MS*30=3S

/* WORK MODE */
#define TASK_STARTUP				0
#define TASK_LOWBATTERY				1
#define TASK_SCALES					2
#define TASK_SLEEP					3
#define TASK_WEIGHT_AUTOON			4

/* DISPLAY MODE */
#define DISPLAY_POWERON				0
#define DISPLAY_LOWBATTERY			1//低电.
#define DISPLAY_NOLOAD				2//空载 当前重量小于最小称重重量，显示0.0,10s后关机.
#define DISPLAY_LOADUP				3//完成一次称重.
#define DISPLAY_LOADFIX				4//完成一次稱重測量后重量穩定沒有解鎖.
#define DISPLAY_LOADDOWN			5//下秤動作.
#define DISPLAY_OVERLOAD			6//超重.
#define DISPLAY_IMPEDANCEING		7//正在测量阻抗.
#define DISPLAY_IMPEDANCE_FINISH	8//测试阻抗.
#define DISPLAY_CAL0				9//校准0点.
#define DISPLAY_CAL1				10//校准50kg.
#define DISPLAY_CAL2				11//校准100kg.
#define DISPLAY_CAL3				12//校准150kg.
#define DISPLAY_CALPASS				13//校准成功.
#define DISPLAY_CALFAIL				14//校准失败.
#define DISPLAY_ALLOFF				15//关闭所有显示LED.
#define DISPLAY_UNLOCK_WEIGHT		16//小重量不锁定显示LED.
#define DISPLAY_MAX					17

/* WEIGHT */
//2.5KG~5KG之间不锁定重量，只显示当前值.
#define C_UNLOCK_WEIGHT				100//精度:0.1jin

#define C_REMEBER_200G				4//单位0.1JIN, 4*0.1jin=0.4jin=200g
#define C_WEIGHT_MANUALCAL			2000//第一次上电且没校准过,大于100KG时进入手动校准.

/* IMPEDANCE */
//阻抗测试错误
//阻抗 - 阻抗 單位ohm. 若為(0xffff-無待測阻抗錯误/(0xFFF1-接觸異常錯誤)/(0xFFF2-阻抗超出範圍)
#define IMPEDANCE_ERR_NOTEST		0xFFFF//(0xffff-無待測阻抗錯误)
#define IMPEDANCE_ERR_ABNORMAL		0xFFF1//(0xFFF1-接觸異常錯誤)
#define IMPEDANCE_ERR_OUTRANGE		0xFFF2//(0xFFF2-阻抗超出範圍)

/* BODY FAT RATE */
//体脂率范围//
#define C_BODYFATRATE_MIN			50//小于5%,show 'L'
#define C_BODYFATRATE_MAX			500//>50%,show 'H'

/* BLUETOOTH COUNT */
#define C_BLE_COUNT					5//5*100MS=500MS.

//-------------------------------- UART RECEIVED && SEND -----------------------------------
#define C_TIMEING_CYCLE100MS 		2//2*100MS=200MS
#define C_TIMEING_TIMEOUT 			2

#define REC_BUF_DATA_LEN			11
#define REC_HEAD_CODE				0xFD//received data Head Code.
#define SEND_HEAD_CODE 				0xCF//body fat scales(体脂秤)

/* UART CMD TYPE */
#define CMDTYPE_LO					0x36//battery low表示显示低电LO命令

//接收到蓝牙数据的Byte位置:RECEIVED POSITION
#define POS_HEARD					0//0xFD//head code
#define POS_CMDTYPE					1//cmd type:=0x36 battery low.
#define POS_UNIT					2//unit:=00:KG ; =01:LB;
#define POS_CHECKSUM				10//checksum:BYTE10 (REC_BUF_DATA_LEN - 1)

/* UART DATA TYPE */
#define DATA_TYPE_LOCK				0x00//表示锁定数据.
#define DATA_TYPE_ING				0x01//表示过程数据.
#define DATA_TYPE_OVERLOAD			0x02//表示超重.

#endif //__MYDEFINE_H__
