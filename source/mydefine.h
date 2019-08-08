#ifndef __MYDEFINE_H__
#define __MYDEFINE_H__

//**************************************************************
#define	_UART_ENABLE				ENABLE	//1: enable;	0: disable
#define _LED_FLASH_ENABLE			ENABLE//LED��˸���ܵ�Ԥ�����Ƕ�ʹ��λ,//��ѡ:ENABLE,DISABLE
#define _LVD_LVDEN	 	 			DISABLE//��ѡ:ENABLE,DISABLE
#define _UART_DEBUG					DISABLE//ENABLE  //(_UART_ENABLE && ENABLE) for debug msg


/* TIME */
#define C_TIME_100MS				50//50*2MS=100MS
#define C_TIME_1S					10//10*100MS=1S
#define C_TIME_05S					5	// 100MS*5=500MS
#define C_TIME_3S					30	// 100MS*30=3S
#define C_TIME_5S					50	// 100MS*100=10S
#define C_TIME_10S					100	// 100MS*100=10S
//#define C_TIME_10S					10	// 100MS*10=1S
#define C_TIME_30S					30//1S*30=30S 看门狗唤醒计�?

#define C_LED_FLASH_OFF				0
#define C_LED_FLASH_ON				1
#define C_LED_FLASH_CNT				6	//=2 LED亮灭一�? =4 LED亮灭2�? =6 LED亮灭3�?以此类推.
#define C_LED_FLASH_IMP				12	//体脂测试完成后体重和体脂交替闪烁3�?
#define C_LED_FLASH_DELAY			5//30  //100MS*30=3S

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
#define DISPLAY_ALLOFF				11//关闭所有显示LED
#define DISPLAY_UNLOCK_WEIGHT		12//关闭所有显示LED
#define DISPLAY_MAX					13

//2.5KG~5KG之间不锁定重量，只显示当前�?
#define C_UNLOCK_WEIGHT				100//精度:0.1�?

#define C_REMEBER_200G				4//单位0.1JIN, 4*0.1jin=0.4jin=200g

//阻抗测试错误
//阻抗 - 阻抗�?單位ohm. 若為(0xffff-無待測阻抗錯�?/(0xFFF1-接觸異常錯誤)/(0xFFF2-阻抗超出範圍)
#define IMPEDANCE_ERR_NOTEST					0xFFFF//(0xffff-無待測阻抗錯�?
#define IMPEDANCE_ERR_ABNORMAL					0xFFF1//(0xFFF1-接觸異常錯誤)
#define IMPEDANCE_ERR_OUTRANGE					0xFFF2//(0xFFF2-阻抗超出範圍)


#define C_TIMEING_CYCLE100MS 		2
#define C_TIMEING_TIMEOUT 			2
#define C_BLE_COUNT					5//5*100MS=500MS.

#define DATA_BUF_LEN				11
#define REQ_TIME						0xF1//时间同步指令
#define REQ_VIEWTIME					0xF1//0xF100查看时间
#define REQ_HISTORY						0xF2//0xF200同步历史数据
#define REQ_DELHISTORY					0xF2//0xF201 删除历史
#define REQ_DIS_BT						0xF3//0xF300蓝牙收到指令，断开连接，根�?0�?9日讨论结果，秤端在断开前，需要发送F300告知app
#define REQ_VERTION						0xF4//0xF400
#define REQ_UNITSYN						0xFD//0xFD00单位同步

//����ĸ����������λ�� position
#define POS_HEARD						0//0xFD//heard:0xFD ��ͷ��FD����ʾ�ֻ������ӵ�����
#define POS_CMDTYPE						1//cmd type:=34 �궨����; =35 �ػ���� =36 ��ʾ�͵�LO��� =37 ��ʾΪ��Ч�û�����Ϣ�� =38 ��ʾ�ӽ��뱧Ӥ
#define POS_UNIT						2//unit:��λת���ֽ�	=00:KG ; =01:LB; =02 ST; =03 jin -->����LEDû��ST��λ��ֻ��=0x01��ʾKG������ֵ��ʾLB
#define POS_USER						3//user: 00~09:P0~P9
#define POS_CHECKSUM					10//checksum:8.BYTE2-BYTE7 �����У��͡�= (DATA_BUF_LEN - 1)

#define CMDTYPE_CALIBRATION				0x34//表示标定命令
#define CMDTYPE_SHUTDOWN				0x35//表示关机命令
#define CMDTYPE_LO						0x36//表示显示低电LO命令
#define CMDTYPE_USEROK					0x37//表示为有效的用户组信�?
#define CMDTYPE_MOMBABY					0x38//表示秤进入抱�?

#define C_DATA_LOCK						0x00//表示锁定数据.
#define C_DATA_ING						0x01//表示过程数据.
#define C_DATA_OVERLOAD					0x02//表示超重.

#endif //__MYDEFINE_H__
