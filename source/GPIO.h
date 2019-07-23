//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2015 BY HOLTEK SEMICONDUCTOR INC
//  File Name : GPIO.h
// Description: IO相關設定
//   Customer : 樂福衡器有限公司
//Targer Board: CH376BLE
//   MCU      : HT45F75
//___________________________________________________________________
//___________________________________________________________________
#ifndef _GPIO_H_
#define _GPIO_H_
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Userdefine @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//*****************************************    按鍵    ********************************************
#define KEY_CAL			_pa4
#define KEY_CAL_IO		_pac4
//*****************************************   數碼管  **********************************************
//#define	LEDCom0			_pb7
//#define	LEDCom0IO		_pbc7
//#define LEDCom1			_pb6
//#define LEDCom1IO		_pbc6
//#define	LEDCom2			_pb5
//#define	LEDCom2IO		_pbc5
//#define LEDCom3			_pa7
//#define LEDCom3IO		_pac7

//#define LEDSeg			_pc
//#define LEDSegIO		_pcc
//#define LEDPoint        _pc7
//********************************************  藍牙  **********************************************
#define BT_Power        _pa6
#define BT_Power_IO     _pac6

#define BT_Status		_pd0
#define BT_Status_IO	_pdc0

#define BT_WakeUp		_pd1
#define BT_WakeUp_IO	_pdc1
//***************************************** IO Config  **********************************************
#define PA_Default		0B00000000
#define PAC_Default		0B00111000
#define PAPU_Default	0B11111101
#define PAWU_Default	0B00000000

#define PB_Default		0B11111111
#define PBC_Default		0B00011111
#define PBPU_Default	0B00011111

#define PC_Default		0B00000000
#define PCC_Default		0B00000000
#define PCPU_Default	0B00000000

#define PD_Default		0B11111111
#define PDC_Default		0B11111101
#define PDPU_Default	0B11111101
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 預處理 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define OUTPUT 			0
#define INPUT			1
#define HIGH   1
#define LOW	   0
#endif

