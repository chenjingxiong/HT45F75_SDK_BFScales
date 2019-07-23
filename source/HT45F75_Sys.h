//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2017 BY HOLTEK SEMICONDUCTOR INC
//  File Name : sys_init.h
// Description: 系統初始化相關程序
//   Customer : Holtek Demo Code
//Targer Board: HT45F75
//     Note   :
//___________________________________________________________________
//___________________________________________________________________
#ifndef SYS_INIT_H_
#define SYS_INIT_H_
//@@@@@@@@@@@@@@@@@@@@@@ 系統頻率設定 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//<<<<<< A: HIRC_HXT_select >>>>>>>
#define _HIRC_4_8M_
//#define _HIRC_9_6M_
//#define _HIRC_14_4M_
//#define _HXT_10M_OVER_
//#define _HXT_10M_BELOW_
// <<<<<< B: LIRC_LXT_select >>>>>>>
#define _LTRC_
//#define _LXT_
/* 系统配置 */
#define HXT							ENABLE		// Enable 外部晶振   Disable 內部晶振
#define LXT							DISABLE		// Enable 外部RTC    Disable 內部32768

//@@@@@@@@@@@@@@@@@@@@@@ MCU Ram 和頭文件 @@@@@@@@@@@@@@@@@@@@@@@@
#define RAMBANK	8
//#define UART_WAKE_ENABLE
//#define DEBUG
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 共用函數 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void fun_SysPowerOnInit();
void fun_PowerDownInit();
void fun_GPIOPowerOnInit();
void fun_Wake_Up_To_Weigh();
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 共用函數 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
extern volatile unsigned char gu8v_HaltTime;
extern volatile bit gbv_Mcu_Normal_Work;
extern volatile bit gbv_PowerOn;
// 系統自動休眠時間設定
#define TIMEBASE0_CYCLE8MS    	8	    // 8ms
#define TIMEBASE1_CYCLE1S    	500 	// 500ms
#define HALTTIME60S			(60000/TIMEBASE1_CYCLE1S)
#define HALTTIME40S			(40000/TIMEBASE1_CYCLE1S)
#define HALTTIME30S			(30000/TIMEBASE1_CYCLE1S)
#define HALTTIME20S			(20000/TIMEBASE1_CYCLE1S)
#define HALTTIME17S			(17000/TIMEBASE1_CYCLE1S)
#define HALTTIME15S			(15000/TIMEBASE1_CYCLE1S)
#define HALTTIME10S			(10000/TIMEBASE1_CYCLE1S)
#define HALTTIME5S			(5000/TIMEBASE1_CYCLE1S)
#define HALTTIME4S			(4000/TIMEBASE1_CYCLE1S)
#define HALTTIME3S			(3000/TIMEBASE1_CYCLE1S)
#define HALTTIME2S			(2000/TIMEBASE1_CYCLE1S)
#define HALTTIME1S			(1000/TIMEBASE1_CYCLE1S)
#define HALTTIME_DEFAULT	HALTTIME60S
#define MCU_SLEEP_GET_ZERO_TIME 2*60*0*60 // 2 个小时 MCU 1S 起来一次
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 預處理 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//					@-------------IO config--------------@
//Setting in Target.h
//					@---------------IAP config---------------@
// Setting in IAP.h
//					@-------------EEPROM config--------------@
// Setting in EEPROM.h , EEPROM.C
//					@---------------RAM config---------------@
// Seting in Sys_Init.c
//					@-----------SysClock config--------------@
// Seting in Sys_Init.c
//					@---------------WDT config---------------@
// WDT clock Source fs =fsub
// WDT time计算方式: WDT time =  2^18/fsub
// example:(max time 周期最长2^18，Source fs = 32000)
// WDT time = 2^18/32000= 8s
#define SETWDT_DISABLE()		{ _wdtc = 0xA8;}
#define SETWDTTIME_8MS()		{ _wdtc = 0x50;}
#define SETWDTTIME_32MS()		{ _wdtc = 0x51;}
#define SETWDTTIME_125MS()		{ _wdtc = 0x52;}
#define SETWDTTIME_500MS()		{ _wdtc = 0x53;}
#define SETWDTTIME_1000MS()		{ _wdtc = 0x54;}
#define SETWDTTIME_2000MS()		{ _wdtc = 0x55;}
#define SETWDTTIME_4000MS()		{ _wdtc = 0x56;}
#define SETWDTTIME_8000MS()		{ _wdtc = 0x57;}
//					@-------------LVR config--------------@
#define  SETLVR_2_1V()			{ _lvrc = 0x55;}
#define  SETLVR_2_55V()			{ _lvrc = 0x33;}
#define  SETLVR_3_15V()			{ _lvrc = 0x88;}
#define  SETLVR_3_8V()			{ _lvrc = 0xAA;}

//					@-------------LVD config--------------@
#define SETLVD_LVDIN()			{ _lvdc = 0x10; }
#define SETLVD_2V2()			{ _lvdc = 0x11; }
#define SETLVD_2V4()			{ _lvdc = 0x12; }
#define SETLVD_2V7()			{ _lvdc = 0x13; }
#define SETLVD_3V0()			{ _lvdc = 0x14; }
#define SETLVD_3V3()			{ _lvdc = 0x15; }
#define SETLVD_3V6()			{ _lvdc = 0x16; }
#define SETLVD_4V0()			{ _lvdc = 0x17; }
#define SETLVD_ENABLE()			{ _lvden = 1;}
#define SETLVD_DISABLE()		{ _lvden = 0;}
#define SET_DAC_DSIABLE()		{ _dsdacen = 0;}

//#define _HIRC_9_6M__HIRC_9_6M_
//                  @-------------TM config--------------@
// STM  Setting  when need
// PTM0 Setting  when need
// PTM1 Setting  when need
// PTM2 For LED Scan 2ms Timer/Counter Mode
#ifdef _HIRC_4_8M_
    #define SETPTM2_TIMER2MS()  { _ptm2c0 = 0x20; _ptm2c1 = 0xC1; _ptm2al = 500%256;_ptm2ah = 500/256;}
#endif
#ifdef _HIRC_9_6M_
    #define SETPTM2_TIMER2MS()  { _ptm2c0 = 0x20; _ptm2c1 = 0xC1; _ptm2al = 500%256;_ptm2ah = 500/256;}
#endif
#ifdef _HIRC_14_4M_
    #define SETPTM2_TIMER2MS()  { _ptm2c0 = 0x30; _ptm2c1 = 0xC1; _ptm2al = 125%256;_ptm2ah = 125/256;}
#endif
#define SETPTM2_ISRENABLE()     { _ptm2af = 0; _ptm2ae=1; _mf1f = 0;_mf1e = 1; _pt2on = 1;}
#define SETPTM2_ISRDISABLE()    { _pt2on = 0; _ptm2ae = 0;}

//                  @-----------TimeBase config----------@
// 默認使用fs =fsub, LIRC/LXT
#define SETTIMEBASE0_8MS()		{ _pscr = 0x03; _tb0c = 0x00;}
#define SETTIMEBASE0_16MS()		{ _pscr = 0x03; _tb0c = 0x01;}
#define SETTIMEBASE0_32MS()		{ _pscr = 0x03; _tb0c = 0x02;}
#define SETTIMEBASE0_62MS()		{ _pscr = 0x03; _tb0c = 0x03;}
#define SETTIMEBASE0_124MS()	{ _pscr = 0x03; _tb0c = 0x04;}
#define SETTIMEBASE0_250MS()	{ _pscr = 0x03; _tb0c = 0x05;}
#define SETTIMEBASE0_500MS()	{ _pscr = 0x03; _tb0c = 0x06;}
#define SETTIMEBASE0_1000MS()	{ _pscr = 0x03; _tb0c = 0x07;}
#define SETTIMEBASE0_ENABLE()	{ _tb0f = 0x00; _tb0e = 0x01; _tb0on = 1;}
#define SETTIMEBASE0_RUNNING()	{ _tb0on = 1; }
#define SETTIMEBASE0_STOP()	    { _tb0on =0; }
#define SETTIMEBASE0_DISABLE()	{ _tb0e = 0; _tb0on =0; }


#define SETTIMEBASE1_8MS()		{ _pscr = 0x03; _tb1c = 0x00;}
#define SETTIMEBASE1_16MS()		{ _pscr = 0x03; _tb1c = 0x01;}
#define SETTIMEBASE1_32MS()		{ _pscr = 0x03; _tb1c = 0x02;}
#define SETTIMEBASE1_62MS()		{ _pscr = 0x03; _tb1c = 0x03;}
#define SETTIMEBASE1_124MS()	{ _pscr = 0x03; _tb1c = 0x04;}
#define SETTIMEBASE1_250MS()	{ _pscr = 0x03; _tb1c = 0x05;}
#define SETTIMEBASE1_500MS()	{ _pscr = 0x03; _tb1c = 0x06;}
#define SETTIMEBASE1_1000MS()	{ _pscr = 0x03; _tb1c = 0x07;}
#define SETTIMEBASE1_ENABLE()	{ _tb1f = 0x00; _tb1e = 0x01; _tb1on = 1;}
#define SETTIMEBASE1_DISABLE()	{_tb1e = 0; _tb1on =0; }
#define SETTIMEBASE1_RUNNING()	{ _tb1on = 1; }
#define SETTIMEBASE1_STOP()	    { _tb1on =0; }


#define SET_MF0E_DISABLE()		{ _mf0e = 0;}
#define SET_MF0E_ENABLE()		{ _mf0e = 1;}
#define SET_MF1E_DISABLE()		{ _mf1e = 0;}
#define SET_MF1E_ENABLE()		{ _mf1e = 1;}
#define SET_MF2E_DISABLE()		{ _mf2e = 0;}
#define SET_MF2E_ENABLE()		{ _mf2e = 1;}
#define SET_EMI_ENABLE()          {  _emi  = 1;}
#define SET_EMI_DISABEL()         {  _emi  = 0;}
#define SET_INT1EDISABLE()		{ _int1e =0 ;}
#define SET_ADC_IN_DISABLE()		{ _ade = 0;}
//						@-------------SIM config-------------@
// Setting in SIM.h
//						@------------SPIA config-------------@
// Setting in SIM.h
//						@-------------UART config------------@
// Setting in UART.h
//						@------------ADC config--------------@
// Setting in BodyFatScalesMCUConfig.h
//						@-----------BodyFat config-----------@
// Setting in BodyFatScalesMCUConfig.h
//__________________________________________________________________//
//					-----ISR VECTOR Definations------;
//中斷入口地址
#define	START_VECTOR			0x000		// 主程序
#define INT0_VECTOR				0x004		// Interrupt 0
#define INT1_VECTOR				0x08		// Interrupt 1
#define MuFunction0_VECTOR		0x00C		// Multi function0 TM0
#define ADC_VECTOR				0x010		// ADC
#define Timebase0_VECTOR		0x014		// timebase0
#define Timebase1_VECTOR		0x018		// timebase1
#define MuFunction1_VECTOR		0x01c		// Multi function1 LVD&EEPROM&UART&SIM
#define MuFunction2_VECTOR		0x020		// Multi function2 I2C &TM1
#define MuFunction3_VECTOR		0x024		// Multi function3 TM2

#endif
