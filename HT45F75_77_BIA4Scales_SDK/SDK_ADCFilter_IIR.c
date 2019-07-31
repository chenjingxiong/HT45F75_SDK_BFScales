asm(" message' **************************************************** ' ");
asm(" message' *       2018 BY BEST HEALTH ELECTRONIC INC         * ' ");
asm(" message' *__________________________________________________* ' ");
asm(" message' *        SDK  NAME  : 	SDK_ADCFilter_IIR.c	      * ' ");
asm(" message' *   Compiler   Ver. :      V3.52                   * ' ");
asm(" message' *   IDE3000    Ver. :      V7.96                   * ' ");
asm(" message' *   RELEASE    Ver. :      1.0.0                   * ' ");
asm(" message' *   RELEASE   DATA  :     2018/05/09               * ' ");
asm(" message' *__________________________________________________* ' ");
asm(" message' *    MCU / CFG Ver. :       All MCU                * ' ");
asm(" message' **************************************************** ' ");

#include "SDK_Interface.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  功能說明                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
1. IIR 濾波數據
2. 原始數據為24/20Bit
3. 濾波輸出結果可以為20/16Bit
4. 濾波穩定參數可動態調整
*/

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  封庫設置                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define ADCSourceUseBit 20 // 24 or 20  ADC 原始 ADC 數據Bit數,主要為匹配早期20Bit MCU 比如HT45F7x
#define ADCFilterUseBit 16 // 16 or 20  20精度更高,但ROM & RAM 佔用更大

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  依賴參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
extern ADCSource_t SDKADCSourceData; // 原始ADC數據,詳細參考SDK_typedef.h ADCSource_t

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  對外參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
typedef struct
{
	unsigned long Current;			// R 當前濾波ADC數據
	unsigned long BufMax;			// R 濾波 buf 中最大的ADC值,如果為0x00000000,說明跳動值超過設置濾波範圍
	unsigned long BufMin;			// R 濾波 buf 中最小的ADC值,如果為0xFFFFFFFF,說明跳動值超過設置濾波範圍
	unsigned int StableThreshold;   // RW 目標穩度閥值,用戶在SDK_UserSetting.h設定值
	unsigned char StableCntTag;		// RW 目標穩度次數,用戶在SDK_UserSetting.h設定值
	unsigned char StableCntCurrent; // R 當前穩度次數
	union {
		struct
		{
			unsigned char IsReady : 1;  // R 有一筆濾波ADC數據
			unsigned char IsStable : 1; // R 濾波ADC數據處於穩定狀態
		} b;
		unsigned char byte;
	} flag;
} ADCFilter_t;
*/
extern ADCFilter_t SDKADCFilterData;
void fun_FilterInit();
void fun_Filtering();



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  源代碼                                             @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
typedef char s8;
typedef int s16;
typedef long s32;
typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef union {
	struct
	{
		unsigned char byte0;
		unsigned char byte1;
	} byte;
	int s16;
	unsigned int u16;
} __16_type;
typedef union {
	struct
	{
		unsigned char byte0;
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
	} byte;
	unsigned long u32;
	long s32;
} __32_type;


ADCFilter_t SDKADCFilterData;
unsigned long SDKADCSourceData20BitCurrent;
unsigned long SDKADCSourceData20BitLast;
/***********************************************
 Function: 濾波參數初始化
 INPUT	:
 OUTPUT	:
 NOTE	:
 **********************************************/
void fun_FilterInit()
{
	SDKADCFilterData.BufMax = 0;
	SDKADCFilterData.BufMin = 0xFFFF;
	SDKADCFilterData.Current = 0;
	SDKADCFilterData.StableCntCurrent = 0;
	SDKADCFilterData.flag.b.IsStable = 0;
	SDKADCFilterData.flag.b.IsReady = 0;
}
/***********************************************
 Function: 對採樣的ADC濾波處理
 INPUT	:
 OUTPUT	:
 NOTE	: IIR
 **********************************************/
void fun_Filtering()
{
	if (SDKADCSourceData.flag.b.IsReady)
	{
		SDKADCSourceData.flag.b.IsReady = 0;
		// 去除正負轉換為單向線性
	#if ADCSourceUseBit == 24
		__32_type temp;
		temp.byte.byte0 = SDKADCSourceData.ByteLow;
		temp.byte.byte1 = SDKADCSourceData.ByteMid;
		temp.byte.byte2 = SDKADCSourceData.ByteHigh + 0x80;
		temp.byte.byte3 = 0x00;
		SDKADCSourceData20BitCurrent = temp.u32 >> 4;
	#endif
	#if ADCSourceUseBit == 20
		__32_type temp;
		temp.byte.byte0 = SDKADCSourceData.ByteLow;
		temp.byte.byte1 = SDKADCSourceData.ByteMid;
		temp.byte.byte2 = SDKADCSourceData.ByteHigh + 0x08;
		temp.byte.byte2 &= 0x0F;
		temp.byte.byte3 = 0x00;
		SDKADCSourceData20BitCurrent = temp.u32;
	#endif
		u32 SDKADCFilterDataCurrentemp;
		SDKADCSourceData20BitCurrent=SDKADCSourceData20BitCurrent/16;
		//快速穩定數據，縮短濾波器滯後的時間參數
		if (fun_unsigned32BitABS(SDKADCSourceData20BitCurrent, SDKADCFilterData.Current) > SDKADCFilterData.StableThreshold * 8)
		{
			fun_FilterInit();
			SDKADCSourceData20BitLast = SDKADCSourceData20BitCurrent;
			SDKADCFilterData.Current =  SDKADCSourceData20BitCurrent;
		}
		//濾波器的實現 最新濾波數據 = (a*(上一筆原始ADC值+當前筆原始ADC值)+b*上一次濾波數據)/(a*2+b)
		SDKADCFilterDataCurrentemp = 8 * (SDKADCSourceData20BitCurrent + SDKADCSourceData20BitLast) +112 * (u32)SDKADCFilterData.Current;
		SDKADCFilterDataCurrentemp /= 128;
		SDKADCFilterData.Current=(int)SDKADCFilterDataCurrentemp;
		SDKADCSourceData20BitLast = SDKADCSourceData20BitCurrent;
		//取得變化的最大值和最小值
		if (SDKADCFilterData.Current > SDKADCFilterData.BufMax)
		{
			SDKADCFilterData.BufMax = SDKADCFilterData.Current;
		}
		if (SDKADCFilterData.Current < SDKADCFilterData.BufMin)
		{
			SDKADCFilterData.BufMin = SDKADCFilterData.Current;
		}
		//如果連續多次範圍內最大值和最小值差值較小(變化平衡)，鎖定該值
		if ((SDKADCFilterData.BufMax - SDKADCFilterData.BufMin) < SDKADCFilterData.StableThreshold)
		{
			SDKADCFilterData.StableCntCurrent++;
			if (SDKADCFilterData.StableCntCurrent > SDKADCFilterData.StableCntTag)
			{
				SDKADCFilterData.StableCntCurrent = 0;
				SDKADCFilterData.flag.b.IsStable = 1;
			}
		}
		else
		{
			SDKADCFilterData.flag.b.IsStable = 0;
			SDKADCFilterData.StableCntCurrent = 0;
			SDKADCFilterData.BufMax = 0;
			SDKADCFilterData.BufMin = 0xFFFF;
		}
//#if ADCFilterUseBit == 16
//		SDKADCFilterData.Current = SDKADCFilterData.Current >> 4; // 20Bit 取16bit
/*#endif*/
		SDKADCFilterData.flag.b.IsReady = 1;
	}
}