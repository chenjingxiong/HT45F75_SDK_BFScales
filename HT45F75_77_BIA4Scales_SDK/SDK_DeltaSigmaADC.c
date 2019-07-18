asm(" message' **************************************************** ' ");
asm(" message' *       2018 BY BEST HEALTH ELECTRONIC INC         * ' ");
asm(" message' *__________________________________________________* ' ");
asm(" message' *        SDK  NAME  : 	SDK_DeltaSigmaADC.c	      * ' ");
asm(" message' *   Compiler   Ver. :      V3.52                   * ' ");
asm(" message' *   IDE3000    Ver. :      V7.96                   * ' ");
asm(" message' *   RELEASE    Ver. :      1.0.0                   * ' ");
asm(" message' *   RELEASE   DATA  :     2018/05/09               * ' ");
asm(" message' *__________________________________________________* ' ");
asm (" message' *    MCU / CFG Ver. :   HT45F75 / 1.6             * ' ");
asm (" message' *                       HT45F77 / 2.2             * ' ");
asm(" message' **************************************************** ' ");

#include "SDK_Interface.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  功能說明                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
1. 利用 ADC 中斷獲取 ADC 數據
2. ADC 開始
3. ADC 關閉
*/

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  封庫設置                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include "HT45F75.h"
#define ADC_VECTOR 0x010 // MCU ADC中斷地址
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  依賴參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define SET_LDO_EN2_4()	  	        { _pwrc = 0x80; }
#define SET_LDO_EN2_6()		        { _pwrc = 0x81; }
#define SET_LDO_EN2_9()		        { _pwrc = 0x82; }
#define SET_LDO_EN3_3()		        { _pwrc = 0x83; }
#define SET_LDO_ENBYPASS()	        { _pwrc = 0x84; }
#define SET_LDO_DISABLE()	        { _enldo = 0; }
#define SET_LDO_ENABLE()	        { _enldo = 1; }
#define SET_ADCINTERRUPT_ENABLE()	{ _adf = 0; _ade = 1; _emi = 1;}
#define SET_ADCINTERRUPT_DISABLE()	{ _ade = 0;}
#define SET_ADCPOWER_ON()			{ _adoff = 0; }
#define SET_ADCPOWER_OFF()			{ _adoff = 1; }
#define SET_ADCMODE_SLEEP()			{ _adslp = 1; }
#define SET_ADCMODE_NORMAL()		{ _adslp = 0; }
#define SET_ADCVREF_VREFPVREFN()	{ _vrefs = 1; }
#define SET_ADCVREF_VCMAVSS()		{ _vrefs = 0; }
#define SET_ADCBUF_ENABLE()		    { _vrbufp = 1; _vrbufp = 1;}
#define SET_ADCBUF_DISABLE()		{ _vrbufp = 1; _vrbufp = 1;}
#define SET_ADC_STARTCONVERT()		{ _adrst = 0; _adrst = 1; _adrst = 0; _adcdl = 0; _eoc = 0; }
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  對外參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
typedef struct
{
	unsigned char ByteHigh;	// R ADC 原始數據高位
	unsigned char ByteMid;	 // R ADC 原始數據中位
	unsigned char ByteLow;	 // R ADC 原始數據低位
	unsigned char SamplingCnt; // R ADC 採集次數計數
	union {
		struct
		{
			unsigned char IsReady : 1; // R 有一筆原始ADC數據
		} b;
		unsigned char byte;
	} flag;
} ADCSource_t;
*/
extern ADCSource_t SDKADCSourceData;
void fun_ADCStop();
void fun_ADCStart();

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  源代碼                                             @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
ADCSource_t SDKADCSourceData; // 原始ADC數據,詳細參考SDK_typedef.h ADCSource_t
/*************************************************
 Function: 開始ADC 採樣
 INPUT	:
 OUTPUT	:
 NOTE	:
 *************************************************/
void fun_ADCStart()
{
	SET_LDO_EN2_6();
	SET_ADCPOWER_ON();
	SET_ADCMODE_NORMAL();
	SET_ADCINTERRUPT_ENABLE();
	SET_ADC_STARTCONVERT();
	SDKADCSourceData.SamplingCnt = 0;
	SDKADCSourceData.flag.b.IsReady = 0;
}
/*************************************************
 Function: 停止ADC 採樣
 INPUT	:
 OUTPUT	:
 NOTE	: 關閉ADC模塊耗電
 *************************************************/
void fun_ADCStop()
{
	SET_LDO_DISABLE();
	SET_ADCPOWER_OFF();
}
//@------------ADC 中斷入口函數---------------@
DEFINE_ISR(ADC_ISR, ADC_VECTOR)
{
	if (_eoc == 1)
	{ //ADC 轉換完成
		SDKADCSourceData.SamplingCnt++;
		//前三筆數據丟掉，第四筆才開始處理
		if (SDKADCSourceData.SamplingCnt > 3)
		{
			// Read current ADC
			_adcdl = 1;
			SDKADCSourceData.ByteLow  = _adrl;
			SDKADCSourceData.ByteMid  = _adrm;
			SDKADCSourceData.ByteHigh = _adrh;
			_adcdl = 0; //數據讀取完成，取消鎖存
			SDKADCSourceData.flag.b.IsReady = 1;
		}
		_eoc = 0;
	}
}