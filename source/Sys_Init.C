//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2015 BY HOLTEK SEMICONDUCTOR INC
//  File Name : Sys_init.c
// Description: 系統初始化
//   Customer : 樂福衡器有限公司
//Targer Board: CH376BLE
//   MCU      : HT45F75
//___________________________________________________________________
//___________________________________________________________________
#include "common.h"

//===============================================================
//function: 配置系统时钟
//input   : none.
//output  : none
//description:
//===============================================================
void SysFrequencyInit(void)
{
	_hlclk = 1;
//	//配置系统时钟
//	#if (HXT == ENABLE)
//		// 修改IO为OSC
//		_pas07 = 1;
//		_pas06 = 1;
//		_pas11 = 1;
//		_pas10 = 1;
//		//开启外部时钟源.
//		#if (SYS_FREQUENCY_OSC > 10000000)
//			_hxtm  = 1;
//		#else
//			_hxtm  = 0;
//		#endif
//		_hxten = 1;
//		_fhs   = 1; // 选择系统时钟来自外部.
//		_hircc = 0; // 关闭内部时钟.
//	#else
//		// 修改IO为普通IO
//		_pas07 = 0;
//		_pas06 = 0;
//		_pas11 = 0;
//		_pas10 = 0;
//		// 开启内部时钟源
//		#if   (SYS_FREQUENCY_OSC == SYS_OSC_4MHZ)
//			_hircc = 0x01;
//		#elif (SYS_FREQUENCY_OSC == SYS_OSC_8MHZ)
//			_hircc = 0x05;
//		#elif (SYS_FREQUENCY_OSC == SYS_OSC_12MHZ)
//			_hircc = 0x09;
//		#endif
//		_fhs   = 0; // 选择系统时钟来自内部
//		_hxtc  = 0x00; // 关闭外部时钟源
//	#endif
//	// 系统低速时钟配置
//	#if (LXT == ENABLE)
//		_fss   = 1;
//		_lxten = 1;
//	#else
//		_fss  = 0;
//		_lxtc = 0;
//	#endif
}


static void fun_RamInit()
{
	_mp1h = 0;
	_mp1l = 0x80;
	while(_mp1h <2)
	{
		for(_tblp = 0x00;_tblp < 128;_tblp++)
		{
			 _iar1 = 0;
			  _mp1l++;
		}
		_mp1l = 0x80;
		_mp1h++;
	}
}
/********************************************************************
Function: GPIO初始化
INPUT	: none
OUTPUT	: none
NOTE	: 所有IO config為輸入
********************************************************************/
static void fun_GPIOInit()
{
	//IO
	_pa  = PA_Default;		//PA PORT
	_pac = PAC_Default;
	_papu= PAPU_Default;
	_pawu= PAWU_Default;

	_pb  = PB_Default;		//PB PORT
	_pbc = PBC_Default;
	_pbpu= PBPU_Default;

	_pc  = PC_Default;		//PC PORT
	_pcc = PCC_Default;
	_pcpu= PCPU_Default;

	_pd  = PD_Default;		//PD PORT
	_pdc = PDC_Default;
	_pdpu= PDPU_Default;

	 P_LED_BLE = LOW;//蓝牙
	 P_LED_BLE_C = OUTPUT;
	 P_LED_UNIT_PCT	= LOW;//百分率:%
	 P_LED_UNIT_PCT_C = OUTPUT;

	/*Bluetooth init */
	 P_BT_Status_C = OUTPUT;//INPUT;//OUTPUT;
	 P_BLE_EN_C = OUTPUT;
	 P_BLE_EN = LOW;
	 P_BT_Status = LOW;//LOW;

	_ctrl0 = 0x00;

	SETLEDCURRENT_LEVEL3();


}

void LED_Init(void)
{
	LEDCOM1 = LOW ;
	LEDCOM2 = LOW ;
	LEDCOM3 = LOW ;
	LEDCOM4 = LOW ;
	LEDSEG = LOW;
	LEDCOMC1 = OUTPUT ;
	LEDCOMC2 = OUTPUT ;
	LEDCOMC3 = OUTPUT ;
	LEDCOMC4 = OUTPUT ;
	LEDSEGC  = OUTPUT ;
	P_LED_BLE = LOW;
	P_LED_BLE_C = OUTPUT;
	P_LED_UNIT_PCT = LOW;
	P_LED_UNIT_PCT_C = OUTPUT;
}

/********************************************************************
Function: Timer初始化
INPUT	: none
OUTPUT	: none
NOTE	:
********************************************************************/
static void fun_TimerInit()
{
	// TM0
	_tm0c0 = 0x20;		// fsys/16 4us
	_tm0c1 = 0xc1;		// TimeCnt Mode
	_tm0al = 500%256; 	// 500*4us = 2ms;
	_tm0ah = 500/256;
	_t0on  = 1;
	SETCTMA_ISR_ENABLE();

	// TM1
//    _ptm1c0= 0x20;		// fsys/4 4us
//    _ptm1c1= 0xc1;		// TimeCnt Mode
//    _ptm1al= 500%256;	// 500*4us = 2ms
//    _ptm1ah= 500/256;
//    _t1on = 1;
	_t1on = 0;
//	SETPTM1A_ISR_ENABLE();

	// TM2
   // _ptm2c0= 0x00;		// fsys/4 1us
   // _ptm2c1= 0xc1;		// TimeCnt Mode
   // _ptm2al= 1000%256;	// 1000*1us = 1ms
   // _ptm2ah= 1000/256;
   // _ptm2on=1;
	_t2on = 0;

	// TimeBase 0
//	 SETTIMEBASE0_8MS();
//	 SETTIMEBASE0_ISR_Enable();
//	 SETTIMEBASE_ON();
	SETTIMEBASE0_ISR_Disable();
	SETTIMEBASE_OFF();


	// TimeBase 1
//	 SETTIMEBASE1_1000MS();
//	 SETTIMEBASE1_ISR_Enable();
//	 SETTIMEBASE_ON();
	SETTIMEBASE1_ISR_Disable();
	SETTIMEBASE_OFF();

}
/********************************************************************
Function: MCU上電初始化
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_PowerOnSysInit()
{
	//IAP not need	to init
	//EEPROM not need to init
	//Wait Sys Clock Ok

	SysFrequencyInit();
//	while(!_hto)
//	{
//		GCC_CLRWDT();
//	}
	//RAM
	fun_RamInit();
	//WDT
	SETWDTTIME_125MS();
	//LVR
	SETLVR_2_1V();//正常执行时 LVR 会于休眠或空闲时自动除能关闭。
	//LVD
	SETLVD_LVDIN();
	SETLVD_ENABLE();
	//GPIO
	fun_GPIOInit();
	// TM & TimeBase
	fun_TimerInit();
	// SIM User Define
	// UART User Define
	// ADC User Define

	user_init();
}
/********************************************************************
Function: 關閉各個模塊進入HLAT模式
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_PrepareToHalt()
{

	//關閉 timer
	_t0on = 0;
	_t1on = 0;
	_t2on = 0;
	// 關閉中斷
	_emi = 0;
	SETTIMEBASE_OFF();
//	_tbon = 1;
	SETWDTTIME_1000MS();// 1S喚醒
}

/********************************************************************
Function: 初始化用户设置.
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void user_init(void)
{
	#if(_LVD_LVDEN == ENABLE)
	if(LVD_LVDO)
	{
//		gu8v_worktasks = TASK_LOWBATTERY;
	}
	else
	#endif
	{
//		gu8v_worktasks = TASK_STARTUP;
		gu8v_worktasks = TASK_SCALES;
	}

	Set_AllLEDBuffer(0);
	Set_DisplayMode(DISPLAY_ALLOFF);
	gu8v_weigh_targeunit = UNIT_KG;
	set_overtime2poweroff(C_TIME_10S);
	//总中断位:=0 关总中断;=1 打开总中断.

	_t0on  = 1;
	SETCTMA_ISR_ENABLE();
	_emi = 1;

	fg_led_timing = 0;
	fg_led_flash = 0;
	fg_time_10s = 0;
}

//@------------�ⲿ�Д�0��ں���--------------@
DEFINE_ISR(INT0_ISR, INT0_VECTOR)
{

}
//@------------�ⲿ�Д�0��ں���--------------@
DEFINE_ISR(INT1_ISR, INT1_VECTOR)
{
	//gbv_Data_Recive_flag = 1;
}
//@-------MuFunction0 �Д���ں���------------@
//TM0
DEFINE_ISR(MuFunction0_ISR, MuFunction0_VECTOR)
{
	_t0pf=0;
	_t0af=0;
	_mf0f=0;

	if(C_TIME_100MS <= ++gu8v_time_100ms){
		gu8v_time_100ms = 0;
		fg_time_100ms = 1;

		if(C_TIME_1S <= ++gu8v_time_1s){
			gu8v_time_1s = 0;
			fg_time_1s = 1;
		}

		/* LED显示闪烁切换定时 */
		if(fg_led_timing){
			//先延时 后执行LED闪烁功能
			if(!fg_led_delay){
				if(gu8v_led_delay)
					gu8v_led_delay--;
				else
					fg_led_delay = 1;
			}
			// 延时时间到后执行LED显示闪烁切换标志。
			if(fg_led_delay){
				gu8v_05s_count++;
				if(gu8v_led_speed <= gu8v_05s_count){
					gu8v_05s_count = 0;
					if(gu8v_howtimes){
						fg_led_flash = !fg_led_flash;//控制LED一亮一灭闪烁
						//fg_led_change:可以用来控制闪烁时体脂与体重的轮流闪烁。
						//注意:最好在fg_led_flash=1时,即LED处于熄灭状态
						if(fg_led_flash){
							fg_led_change = !fg_led_change;
						}
						gu8v_howtimes--;
					}else{
						fg_led_delay = 0;
						fg_led_timing = 0;
						fg_led_flash = 0;
						fg_led_change = 0;
					}
				}
			}
		}else{
			/* 定时休眠关机,延迟时间过后才开始计时. */
			if(gu8v_timed_shutdown){
				gu8v_timed_shutdown--;
				fg_time_10s = 0;
				//当定时休眠时间没到，也没执行LED闪烁功能时,
				//fg_led_flash设为0，进行所有显示LED扫描，防止fg_led_flash=1但还没到定时时间就关闭所有LED了。
				fg_led_flash = 0;//防止显示出现错误关闭LED.
			}else{
				fg_time_10s = 1;
			}
		}

	}

	fun_LEDBufScan();

}
//@------------ADC �Д���ں���---------------@
//DEFINE_ISR(ADC_ISR, ADC_VECTOR)
//{
//
//}
//@----------Timebase0 �Д���ں���-----------@
DEFINE_ISR(Timebase0_ISR, Timebase0_VECTOR)
{
//	Timebase_2S_LVD_Check++;
//	gu16v_Updata_Display_Time++;
//	gu16v_Test_OutTime++;
//	gbv_8ms_Using_Key_Scan = 1;
}
//@----------Timebase1 �Д���ں���-----------@
DEFINE_ISR(Timebase1_ISR, Timebase1_VECTOR)
{
/*	gu8v_HaltTime++;*/
	_tb1f = 0;

}
//@-------MuFunction1 �Д���ں���-----------@
// LVD&EEPROM&UART&SIM
DEFINE_ISR(MuFunction1_ISR, MuFunction1_VECTOR)
{

	_mf1f=0;


}
//@-------MuFunction2 �Д���ں���-----------@
// I2C &TM1
DEFINE_ISR(MuFunction2_ISP, MuFunction2_VECTOR)
{
	_t1pf=0;
	_t1af=0;
	_mf2f=0;
}
//@-------MuFunction3 �Д���ں���-----------@
// TM2
DEFINE_ISR(MuFunction3_ISP, MuFunction3_VECTOR)
{
	_t2pf=0;
	_t2af=0;
	_mf3f=0;
}
