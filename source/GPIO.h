//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2019.08 BY NHD SEMICONDUCTOR INC
//  File Name : GPIO.h
// Description: IO相關設定
//   Customer :
//Targer Board: CH376BLE
//   MCU      : HT45F75
//___________________________________________________________________
//___________________________________________________________________
#ifndef _GPIO_H_
#define _GPIO_H_
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Userdefine @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*****************************************   數碼管  **********************************************
/*#define  LEDCOM5 	_pd3*/
#define  LEDCOM4 				_pa7
#define  LEDCOM3 				_pb5
#define	 LEDCOM2 				_pb6
#define	 LEDCOM1 				_pb7
#define	 LEDSEG	     			_pc
#define	 LEDSEGC     			_pcc
#define  LEDSEG_UNIT_PIONT		_pc7
#define	 LEDCOMC1   			_pbc7
#define	 LEDCOMC2   			_pbc6
#define	 LEDCOMC3   			_pbc5
#define	 LEDCOMC4   			_pac7

#define P_LED_BLE			_pd2
#define P_LED_BLE_C			_pdc2
#define P_LED_UNIT_PCT		_pb0
#define P_LED_UNIT_PCT_C	_pbc0

//********************************************  藍牙  **********************************************
//客户蓝牙引脚定义:蓝牙  BLE_EN上拉输入，BLE_ST高电平输出
#define P_BT_Status		_pd0
#define P_BT_Status_C	_pdc0 //Status状态
#define P_BLE_EN		_pd1
#define P_BLE_EN_C		_pdc1
//***************************************** IO Config  **********************************************
#define PA_Default		0B00000000
#define PAC_Default		0B01011111//0B00111000
#define PAPU_Default	0B01010101;//0B11111101
#define PAWU_Default	0B00000000

#define PB_Default		0B00000000//0B00011110
#define PBC_Default		0B00011000//0B00011110
#define PBPU_Default	0B00011000//0B00011110

#define PC_Default		0B00000000
#define PCC_Default		0B00000000
#define PCPU_Default	0B00000000

#define PD_Default		0B00000000//0B11111011
#define PDC_Default		0B00000001//0B11111001
#define PDPU_Default	0B00000000//0B11111001
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 預處理 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define OUTPUT 			0
#define INPUT			1
#define HIGH   1
#define LOW	   0
#endif

