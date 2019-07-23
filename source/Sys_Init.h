//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2015 BY HOLTEK SEMICONDUCTOR INC
//  File Name : Sys_init.c
// Description: 系統初始化子程序
//Targer Board: CH376BLE
//   MCU      : HT45F75
//___________________________________________________________________
//___________________________________________________________________
#ifndef SYS_INIT_H_
#define SYS_INIT_H_

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 共用函數 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void fun_PowerOnSysInit();
void fun_PrepareToHalt();
void fun_ResumeWork();
void fun_ResumeWakeup();
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Userdefine @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//					@---------------LVD config---------------@
#define SETLVD_LVDIN()			{ _lvdc = 0x00; }
#define SETLVD_2V2()			{ _lvdc = 0x01; }
#define SETLVD_2V4()			{ _lvdc = 0x02; }
#define SETLVD_2V7()			{ _lvdc = 0x03; }
#define SETLVD_3V0()			{ _lvdc = 0x04; }
#define SETLVD_3V3()			{ _lvdc = 0x05; }
#define SETLVD_3V6()			{ _lvdc = 0x06; }
#define SETLVD_4V0()			{ _lvdc = 0x07; }

#define SETLVD_ENABLE()			{ _lvden = 1;}
#define SETLVD_DISABLE()		{ _lvden = 0;}

//					@---------------WDT config---------------@
#define SETWDTDISABLE()			{ _wdtc = 0xA8;}
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
//					@-------------LED Current-------------@
#define SETLEDCURRENT_LEVEL0()	{ _sledc0 = 0x00; _sledc1 = 0x00;}
#define SETLEDCURRENT_LEVEL1()	{ _sledc0 = 0x55; _sledc1 = 0x55;}
#define SETLEDCURRENT_LEVEL2()	{ _sledc0 = 0xAA; _sledc1 = 0xAA;}
#define SETLEDCURRENT_LEVEL3()	{ _sledc0 = 0xFF; _sledc1 = 0xFF;}
//					@-------------LVD config--------------@
//Setting in LVD.h

//					@-------------EEPROM config----------@
//Setting in EEPROM.h
//					@-------------IO config--------------@
//Setting in GPIO.h
//                  @---------------CTMnC0---------------@
#define SETCTMA_ISR_ENABLE()	{ _t0af = 0; _mf0f = 0 ; _t0ae =1; _mf0e = 1;}
#define SETCTMP_ISR_ENABLE()	{ _t0pf = 0; _mf0f = 0 ; _t0pe =1; _mf0e = 1;}

#define SETPTM1A_ISR_ENABLE()	{ _t1af = 0; _mf2f = 0 ; _t1ae =1; _mf2e = 1;}
#define SETPTM1P_ISR_ENABLE()	{ _t1pf = 0; _mf2f = 0 ; _t1pe =1; _mf2e = 1;}

#define SETPTM2A_ISR_ENABLE()	{ _t2af = 0; _mf3f = 0 ; _t2ae =1; _mf3e = 1;}
#define SETPTM2P_ISR_ENABLE()	{ _t2pf = 0; _mf3f = 0 ; _t2pe =1; _mf3e = 1;}
//  ______________________________________________________________________________
// | Bit  |  Bit7  |  Bit6  |  Bit5  |  Bit4  |  Bit3  |  Bit2  |  Bit1  |  Bit0  |
//  ______________________________________________________________________________
// | Name | CTnPAU | CTnCK2 | CTnCK1 | CTnCK0 |  CTnON | CTnRP2 | CTnRP1 | CTnRP0 |
// |______________________________________________________________________________
// | POR  |   0    |   0    |   0    |   0    |   0    |   0    |   0    |   0    |
// |_______________________________________________________________________________
// BIT 7  CTnPAU:TMn計數器暫停控制位
// 			0:  運行
//			1:  暫停
// BIT 6~4  CTnCK2~CTnCK0:選擇TMn計數時鐘
// 			000:  fsys/4   001:  fsys   010:   fh/16
// 			011:  fh/64    100:  fsub   101:   fsub
// 			110:  TCKn上升沿    111:  TCKn下降沿
// BIT 3  CTnON:TMn計數器on/off控制位
// 			0:  off
//			1:  on
// BIT 2~0 CTnRP2~CTnRP0:TMn CCRP 3bit寄存器，與TMn計數器bit9~bit7比較器P匹配週期	/時鐘週期
// 			000:  1024   001:  128   010:   256
// 			011:  384    100:  512   101:   640
// 			110:  768    111:  896
#define CTMC0_Default		0x20   // CLOCK  fsys/16  // 4us
//                           @--------------CTMnC1--------------@
//  ______________________________________________________________________________
// | Bit  |  Bit7  |  Bit6  |  Bit5  |  Bit4  |  Bit3  |  Bit2  |  Bit1  |  Bit0  |
//  ______________________________________________________________________________
// | Name |  CTnM1 |  CTnM0 | CTnIO1 | CTnIO0 |  CTnOC | CTnPOL | CTnDPX |CTnCCLR |
// |______________________________________________________________________________
// | POR  |   0    |   0    |   0    |   0    |   0    |   0    |   0    |   0    |
// |_______________________________________________________________________________
// BIT 7~6  CTnM1~CTnM0:選擇TMn工作模式位
// 			00:  比較器匹配輸出模式   01:未定義模式
//			10:  PWM模式              11:定時/計數器模式
// BIT 5~4  CTnIO1~CTnIO0:選擇TPn_0，TPn_1輸出功能位
//比較匹配輸出模式
// 			00:  無變化          01:  輸出低
//			10:  輸出高          11:  輸出翻轉
//PWM模式
// 			00:  強制無效狀態    01:  強制有效狀態
//			10:  PWM輸出         11:  未定義
//計數器/定時模式: 未定義
// BIT 3  CTnOC
// 比較匹配輸出模式             PWM模式
// 			0:  初始低             0:  低有效
//			1:  初始高             1:  高有效
// BIT 2  CTnPOL:CTPn_0,CTPn_1輸出極性控制位
// 			0:  同相
//			1:  反相
// BIT 1  CTnDPX: CTMn PWM週期/占空比控制位
// 			0:  CCRP-週期   ;CCRA-占空比
//			1:  CCRP-占空比 ;CCRP-週期
// BIT 0  CTnCCLR:選擇CTMn計數器清零條件位
// 			0:  CTMn比較器P匹配
//			1:  CTMn比較器A匹配
#define CTMC1_Default		0xC1		// 定時器模式,A比較器中斷
#define CTMAL_Default		500%256
#define CTMAH_Default		500/256
//                     @--------------TBC-------- - ------@
#define SETTIMEBASE0_8MS()		{ _tbck = 0x00; _tb02 = 0; _tb01 = 0 ; _tb00 = 0 ;}
#define SETTIMEBASE0_16MS()		{ _tbck = 0x00; _tb02 = 0; _tb01 = 0 ; _tb00 = 1 ;}
#define SETTIMEBASE0_32MS()		{ _tbck = 0x00; _tb02 = 0; _tb01 = 1 ; _tb00 = 0 ;}
#define SETTIMEBASE0_62MS()		{ _tbck = 0x00; _tb02 = 0; _tb01 = 1 ; _tb00 = 1 ;}
#define SETTIMEBASE0_124MS()	{ _tbck = 0x00; _tb02 = 1; _tb01 = 0 ; _tb00 = 0 ;}
#define SETTIMEBASE0_250MS()	{ _tbck = 0x00; _tb02 = 1; _tb01 = 0 ; _tb00 = 1 ;}
#define SETTIMEBASE0_500MS()	{ _tbck = 0x00; _tb02 = 1; _tb01 = 1 ; _tb00 = 0 ;}
#define SETTIMEBASE0_1000MS()	{ _tbck = 0x00; _tb02 = 1; _tb01 = 1 ; _tb00 = 1 ;}

#define SETTIMEBASE1_124MS()	{ _tbck = 0x00; _tb11 = 0; _tb10 = 0;}
#define SETTIMEBASE1_250MS()	{ _tbck = 0x00; _tb11 = 0; _tb10 = 1;}
#define SETTIMEBASE1_500MS()	{ _tbck = 0x00; _tb11 = 1; _tb10 = 0;}
#define SETTIMEBASE1_1000MS()	{ _tbck = 0x00; _tb11 = 1; _tb10 = 1;}

#define SETTIMEBASE_ON()		{ _tbon = 1;}
#define SETTIMEBASE_OFF()		{ _tbon = 0;}

#define SETTIMEBASE0_ISR_Enable()		{ _tb0f = 0x00; _tb0e = 0x01;}
#define SETTIMEBASE0_ISR_Disable()		{ _tb0e = 0x00;}
#define SETTIMEBASE1_ISR_Enable()		{ _tb1f = 0x00; _tb1e = 0x01;}
#define SETTIMEBASE1_ISR_Disable()		{ _tb1e = 0x00;}

//						@-------Internal Power config--------@
//Setting in ADC.h
//						@------------ADC config--------------@
//Setting in ADC.h
//						@-------------SIM config-------------@
//Setting in SIM.h
#define	START_VECTOR			0x000		// 主程序
#define INT0_VECTOR				0x004		// Interrupt 0
#define INT1_VECTOR				0x008		// Interrupt 1
#define MuFunction0_VECTOR		0x00C		// Multi function0 TM0
#define ADC_VECTOR				0x010		// ADC
#define Timebase0_VECTOR		0x014		// timebase0
#define Timebase1_VECTOR		0x018		// timebase1
#define MuFunction1_VECTOR		0x01c		// Multi function1 LVD&EEPROM&UART&SIM
#define MuFunction2_VECTOR		0x020		// Multi function2 I2C &TM1
#define MuFunction3_VECTOR		0x024		// Multi function3 TM2

#endif
