//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2017 BY HOLTEK SEMICONDUCTOR INC
//  File Name : sys_init.c
// Description: ϵ�y��ʼ�����P����
//   Customer : Holtek Demo Code
//Targer Board: HT45F75
//     Note   :
//___________________________________________________________________
//___________________________________________________________________
#include "common.h"

volatile unsigned char gu8v_HaltTime ;
/********************************************************************
Function: Ram_Init
IO_IS_INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
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
Function: GPIO��ʼ��
IO_IS_INPUT	: none
OUTPUT	: none
NOTE	: ����IO config��ݔ�� ���چ��}
********************************************************************/
void  fun_GPIOPowerOnInit()
{
// PA0-Unused      PA1-LVDIN
// PA2-Unused      PA3-UNIT Select
// PA4-UNIT Select PA5-Uart-Tx
// PA6-Uart-Rx     PA7-BLE_State
	_pa   = 0B11111111;
	_pac  = 0B11111111;
	_papu = 0B11111111;	// BC7601 ��IO����̫���࣬Ϊ�˷�����ֲ�����ٴ������ԆΪ��� BC7601.h�e���O�� 
	_pawu = 0B00000000;
// PB0-BLE prower  PB1-TEMP
// PB2-LEDCOM3     PB3-LEDCOM2
// PB4-Unused      PB5-Unused
// PB6-Unused      PB7-Unused
	_pb   = 0B00000000;
	_pbc  = 0B11111111;
	_pbpu = 0B11111111;
// PC0-LED_SEG      PC1-LED_SEG
// PC2-LED_SEG      PC3-LED_SEG
// PC4-LED_SEG      PC5-LED_SEG
// PC6-LED_SEG      PC7-LED_SEG
	_pc   = 0B00000000;
	_pcc  = 0B11111111;
	_pcpu = 0B11111111;

// PD0-Unused      PD1-Unused
// PD2-LED COM4    PD3-LED COM5
// PD4-Unused      PD5-Unused
// PD6-Unused      PD7-Unused
	_pd   = 0B11111111;
	_pdc  = 0B11111111;
	_pdpu = 0B11111111;
// PTP2I-Unused      PTP1I-Unused
// PTCK2PS-Unused    STCKPS-Unused
// SCSA-Unused      SDIA-Unused
// SCKA-Unused      RUnused
	_sledc0 = 0xff;
	_sledc1 = 0xff;
}
/********************************************************************
Function: GPIO��ʼ��
IO_IS_INPUT	: none
OUTPUT	: none
NOTE	: ����IO config��ݔ�� 
********************************************************************/
void  fun_GPIOPowerDownInit()
{
// PA0-Unused      PA1-LVDIN
// PA2-Unused      PA3-UNIT Select
// PA4-UNIT Select PA5-Uart-Tx
// PA6-Uart-Rx     PA7-BLE_State
	_pa   = 0B10000000;
	_pac  = 0B00000000;
	_papu = 0B00000000;			// BLE���ֆΪ��� BC7601.h�e���O�� 
	_pawu = 0B00000000;
//	_pas1 = 0B00111100;   //uart ����    
// PB0-BLE prower  PB1-TEMP
// PB2-LEDCOM3     PB3-LEDCOM2
// PB4-Unused      PB5-Unused
// PB6-Unused      PB7-Unused
	_pb   = 0B00000000;
	_pbc  = 0B00000000;
	_pbpu = 0B00000000;
// PC0-LED_SEG      PC1-LED_SEG
// PC2-LED_SEG      PC3-LED_SEG
// PC4-LED_SEG      PC5-LED_SEG
// PC6-LED_SEG      PC7-LED_SEG
	_pc   = 0B00000000;
	_pcc  = 0B00000000;
	_pcpu = 0B00000000;

// PD0-Unused      PD1-Unused
// PD2-LED COM4    PD3-LED COM5
// PD4-Unused      PD5-Unused
// PD6-Unused      PD7-Unused
	_pd   = 0B00000000;
	_pdc  = 0B00000000;
	_pdpu = 0B00000000;
// PTP2I-Unused      PTP1I-Unused
// PTCK2PS-Unused    STCKPS-Unused
// SCSA-Unused      SDIA-Unused
// SCKA-Unused      RUnused
}
/********************************************************************
Function: ����ϵ�y�l��
IO_IS_INPUT	: none
OUTPUT	: none
NOTE	: �m��HIRC��2%��IP �� MCU
********************************************************************/
void fun_SysClockInit()
{
    #ifdef _HIRC_4_8M_
	_fhs   = 0;
	_hircc = 0x03;
	while (!_hircf);    // Wait HIRC Oscillator Stable
	#endif // _HIRC_4_8M_
    #ifdef _HIRC_9_6M_
        _fhs   = 0;
        _hircc = 0x05;
        while (!_hircf);    // Wait HIRC Oscillator Stable
    #endif // _HIRC_9_6M_
    #ifdef _HIRC_14_4M_
        _fhs   = 0;
        _hircc = 0x09;
        while (!_hircf);    // Wait HIRC Oscillator Stable
    #endif // _HIRC_14_4M_
    #ifdef _HXT_10M_BELOW_
        _pas11 = 1;         // SET PA4 TO OSC1
        _pas10 = 1;
        _pas07 = 1;         // SET PA4 TO OSC2
        _pas06 = 1;
        _fhs  = 1;
        _hxtc = 0x01;       // HXTM = 1��HXT Frequency<10MHz,HXT Oscillator Enable
        while (!_hxtf);     // Wait HXT Oscillator Stable
    #endif // _HXT_10M_BELOW_
    #ifdef _HXT_10M_OVER_
        _pas11 = 1;         // SET PA4 TO OSC1
        _pas10 = 1;
        _pas07 = 1;         // SET PA4 TO OSC2
        _pas06 = 1;
        _fhs  = 1;
        _hxtc = 0x05;       // HXTM = 1��HXT Frequency>10MHz,HXT Oscillator Enable
        while (!_hxtf);     // Wait HXT Oscillator Stable
    #endif // _HXT_10M_OVER_
    #ifdef _LXT_
        _pas01 = 1;			// SET PA0 TO LXT1
        _pas00 = 1;
        _pas05 = 1;			// SET PA2 TO LXT2
        _pas04 = 1;
        _fss   = 1;
        _lxten = 1;
        while (!_lxtf);     // Wait LXT Oscillator Stable
    #endif // _LXT_
    #ifdef _LIRC_
        _fss   = 0;
        _lxten = 0;
    #endif // _LIRC_
}
/********************************************************************
Function: MCU��늳�ʼ��
IO_IS_INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_SysPowerOnInit()
{
	//IAP not need	to init
	//EEPROM not need to init
	fun_GPIOPowerOnInit();	// GPIO
	fun_SysClockInit();	// Sys Clock
	fun_RamInit();		// RAM
	SETWDTTIME_500MS(); // WDT
	SETLVR_2_55V();		// LVR
	SETLVD_LVDIN();		// LVD
	// STM  unused
	// PTM0 unused
	// PTM1 unused
	SETPTM2_TIMER2MS();	// PTM2
	SETPTM2_ISRENABLE();
	SETTIMEBASE0_8MS();	// Timebase0
	SETTIMEBASE0_ENABLE();
	SETTIMEBASE1_500MS();// Timebase1
	SETTIMEBASE1_ENABLE();
	
	// SIM Unused
	// SPIA
	// UART
	// ADC
	// BodyFat
	//LVDIN
	//	SETLVD_ENABLE();
	//EMI 
	SET_EMI_ENABLE();// 
}

/********************************************************************
Function: �P�]����ģ�K�M��HALTģʽ
IO_IS_INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_PowerDownInit()
{
	//IAP
	//EEPROM
	// Sys Clock
	// RAM
	SETWDTTIME_1000MS() // WDT For WeightAutoOn
	// LVR
	SETLVD_DISABLE()		// LVD	
	// STM  Unused
	// PTM0 Unused
	// PTM1 Unused
	SETPTM2_ISRDISABLE();	// PTM2
	//Timebasde
	SETTIMEBASE0_DISABLE();	// Timebase0
	SETTIMEBASE1_DISABLE(); // Timebase1
	// SIM Unused
	SET_INT1EDISABLE();
	// SPIA
	SET_SPIA_DISABLE();
	// MF0E
	// MF1E
	// MF2E
	// ADC
	SET_ADC_IN_DISABLE();
	// BodyFat SDK ����
	fun_GPIOPowerDownInit();// GPIO
	gu8v_HaltTime = 0;
	SET_DAC_DSIABLE();
	SET_MF1E_DISABLE();
	SET_EMI_DISABEL();
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

}
//@------------ADC �Д���ں���---------------@
//DEFINE_ISR(ADC_ISR, ADC_VECTOR)
//{
//
//}
//@----------Timebase0 �Д���ں���-----------@
DEFINE_ISR(Timebase0_ISR, Timebase0_VECTOR)
{
	Timebase_2S_LVD_Check++;
	gu16v_Updata_Display_Time++;
	gu16v_Test_OutTime++;
	gbv_8ms_Using_Key_Scan = 1;
}
//@----------Timebase1 �Д���ں���-----------@
DEFINE_ISR(Timebase1_ISR, Timebase1_VECTOR)
{
	gu8v_HaltTime++;
}
//@-------MuFunction1 �Д���ں���-----------@
// LVD&EEPROM&UART&SIM
DEFINE_ISR(MuFunction1_ISR, MuFunction1_VECTOR)
{
	_ptm2af = 0;
	gu8v_Uart_Tx_Time_Out_Cnt++;
	fun_LEDBufScan();
}
//@-------MuFunction2 �Д���ں���-----------@
// I2C &TM1
DEFINE_ISR(MuFunction2_ISP, MuFunction2_VECTOR)
{

}
//@-------MuFunction3 �Д���ں���-----------@
// TM2
DEFINE_ISR(MuFunction3_ISP, MuFunction3_VECTOR)
{

}