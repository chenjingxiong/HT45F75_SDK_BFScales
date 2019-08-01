asm (" message' **************************************************** ' ");
asm (" message' *       2018 BY BEST HEALTH ELECTRONIC INC         * ' ");
asm (" message' *__________________________________________________* ' ");
asm (" message' *        SDK  NAME  : 	SDK_BodyScales.c	       * ' ");
asm (" message' *   Compiler   Ver. :      V3.52                   * ' ");
asm (" message' *   IDE3000    Ver. :      V7.96                   * ' ");
asm (" message' *   RELEASE    Ver. :      1.0.0                   * ' ");
asm (" message' *   RELEASE   DATA  :     2018/05/09               * ' ");
asm (" message' *__________________________________________________* ' ");
asm (" message' *    MCU / CFG Ver. :   HT45F75 / 1.6              * ' ");
asm (" message' *                       HT45F77 / 2.2              * ' ");
asm (" message' **************************************************** ' ");

#include "SDK_Interface.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                    功能說明    				             		    @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
1. 自動標定,標定重量固定為50Kg/50Kg/50Kg
2. 手動標定,標定重量固定為50Kg/50Kg/50Kg
3. 穩定重量解鎖,重量用戶可自行設置
4. 臨時重量解鎖,重量用戶可自行設置
5. 自動喚醒,重量用戶可自行設置
6. 自適應濾波數據16 / 20Bit ADC的處理
6. 自適應原始數據20 / 24Bit ADC的處理,可適用HT45F7x早期20Bit MCU 也適用後期的24Bit MCU

1. 若用戶不需要自動標定 CalADCDataTemp 可用CalADCData共用,不然不行
*/

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  封庫設置                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include "HT45F75.h"
#define SET_ADCGAIN_WEIGHT()        { _pgac0 = 0x27;}   // 稱重ADC放大設置,VGS=0.5,ADGN =1,PGA=128
#define SET_DCSET_WEIGHT()          { _pgac1 = 0x00;}   // 稱重DCSET設置,DCSET = 0V
#define SET_ADCCHAN_WEIGHT()        { _pgacs = 0x00;}   // 稱重ADC 通道AN0&AN1
#define SET_ADCVREF_WEIGHT()	    { _vrefs = 1; }     // 稱重ADC 外部參考電壓

#define CALSPANCHECK 16				// 重量標定結束，砝碼之間的允許差異的最大值16*Span
#define WEIGHT_CAL1 (50 * 20)		// 重量校準點1
#define WEIGHT_CAL2 (50 * 20)		// 重量校準點2
#define WEIGHT_CAL3 (50 * 20)		// 重量校準點3
#define EEPROM_ADDR_START 0x01		// EEPROM 存儲開始位置
#define EEPROM_DATA_CALID_CODE 0xAA // 校準成功標誌數據,偵測到此數據,認為校準成功
#define WeightADCFilterUseBit 16	// 20 or 16 重量 ADC 數據使用的Bit數
#define ADCSourceUseBit 20			// 24 or 20  ADC 原始 ADC 數據Bit數,主要為匹配早期20Bit MCU 比如HT45F7x

#if WeightADCFilterUseBit == 16
	typedef unsigned int typedefWeight;
	#define EEPROM_ADDR_END (4 * 2 + EEPROM_ADDR_START + 2)
	#define EEPROM_SPAN_ADDR (4 * 2 + EEPROM_ADDR_START+1 )
	#define CAL_Diff_ADC_DEFAULT 1500
	//**************量校準參數預設***********
	#define CAL0DATA_DEFAULT 32600		// 0點默認校準值
	#define CAL1DATA_DEFAULT 5000		// 第1點默認校準值
	#define CAL2DATA_DEFAULT 5000		// 第2點默認校準值
	#define CAL3DATA_DEFAULT 5000		// 第3點默認校準值
	#define CALDATASPAN_DEFAULT 5		// 默認分辨率對應ADC值
#endif
#if WeightADCFilterUseBit == 20
	typedef unsigned long typedefWeight;
	#define EEPROM_ADDR_END (4 * 4 + EEPROM_ADDR_START + 2)
	#define EEPROM_SPAN_ADDR (4 * 4 + EEPROM_ADDR_START+1 )
	#define CAL_Diff_ADC_DEFAULT 25000
	//**************量校準參數預設***********
	#define CAL0DATA_DEFAULT 555556		// 0點默認校準值
	#define CAL1DATA_DEFAULT 80000		// 第1點默認校準值
	#define CAL2DATA_DEFAULT 80000		// 第2點默認校準值
	#define CAL3DATA_DEFAULT 80000		// 第3點默認校準值
	#define CALDATASPAN_DEFAULT 80		// 默認分辨率對應ADC值
#endif

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  依賴參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void fun_ADCStart();
void fun_ADCStop();
void fun_Filtering();
void fun_FilterInit();
void fun_LoadWeightSetting();
void fun_LoadAutoOnSetting();
void fun_LoadUserSetting();
unsigned int fun_unsigned32BitABS(unsigned int a, unsigned int b); // 取32位無符號差值
void Write_EEPROMByte(unsigned char addr, unsigned char WriteData);   // 寫一個Byte數據
unsigned char Read_EEPROMByte(unsigned char addr);					  // 讀一個Byte數據
extern unsigned char BHSDKState;     // RW   工作狀態讀取與切換,參考 BodyfatSDKState 枚舉
extern ADCSource_t SDKADCSourceData; // 原始ADC數據,詳細參考SDK_typedef.h ADCSource_t
extern ADCFilter_t SDKADCFilterData; // 濾波ADC數據,詳細參考SDK_typedef.h ADCFilter_t
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  對外參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
typedef struct
{
    struct
    {
        unsigned long Cal0;	    // R    ADC值 - 零點重量
        unsigned long Cal1;	    // R    ADC值 - 第1個標定重量
        unsigned long Cal2;	    // R    ADC值 - 第2個標定重量
        unsigned long Cal3;	    // R    ADC值 - 第3個標定重量
    }CalADCData;// 校準數據
    struct
    {
        unsigned long Cal0;	    // R    ADC值 - 零點重量
        unsigned long Cal1;	    // R    ADC值 - 第1個標定重量
        unsigned long Cal2;	    // R    ADC值 - 第2個標定重量
        unsigned long Cal3;	    // R    ADC值 - 第3個標定重量
    }CalADCDataTemp; // 校準中數據,for debug
    unsigned char Span;	        // R    ADC值 - 1個分辨率
	union {
		struct
		{
			unsigned char IsNeedTare : 1;  // RW 去皮,當前重量為穩定重量時當前重量為0
		} b;
		unsigned char byte;
	} flag;
    unsigned int  DataCurrent;  // R    当前重量值,單位為jin,放大10倍, 若500,表示重量為50.0斤
    unsigned int  DataStable;   // R    穩定重量值,單位為jin,放大10倍, 若500,表示重量為50.0斤
}Weight_t;

typedef struct
{
    unsigned long WeightMax;          // RW 最大測量重量
    unsigned int WeightMin;           // RW 最小測量重量
    unsigned int WeightAutoOn;        // RW 自动唤醒重量
    unsigned int WeightUnLockTemp;    // RW 自動解鎖重量-臨時
    unsigned char WeightUnLockStable; // RW 自動解鎖重量-穩定
    union {
        struct
        {
            unsigned char IsAutoCalOn : 1;    // 自動校準开启
            unsigned char IsAutoUnlockOn : 1; // 自動解鎖开启
        } b;
        unsigned char byte;
    } flag;
} BodyScalesSetting_t;
*/
extern Weight_t SDKWeight;
extern BodyScalesSetting_t SDKWeightSetting; // 用戶設置值


void fun_WeightPowerDown();
void fun_WeightPowerOn();

void fun_Enter_Weight_Normal();
void fun_Enter_Weight_AutoOn();
void fun_Enter_Weight_Cal();

void fun_Weight_Normal();
void fun_Weight_AutoOn();
void fun_weight_Cal();
void fun_weightAutoCal();
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


Weight_t SDKWeight;
volatile unsigned char gu8v_WeighWorkStep;
volatile unsigned char gu8v_WeighCalStep;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ========================================@
//                  校準 代碼	    	    @
// ========================================@
#define weightCalADCDataLast SDKWeight.CalADCDataTemp.Cal3 // 最後一步才用到buf3,此處共用RAM
volatile unsigned int *WeightPointer; // 指針使用特別注意範圍,防止越界更改值！！！

/****************************************
 Function: 標定判斷
 INPUT	:
 OUTPUT	:
NOTE	:  連續三次相等重量放上去
*****************************************/
void fun_weightAutoCal()
{
	if (SDKADCFilterData.Current > (weightCalADCDataLast + CAL_Diff_ADC_DEFAULT)) //這次ADC值比上次大
	{
		WeightPointer++;
		gu8v_WeighCalStep++;
		BHSDKState = gu8v_WeighCalStep;					 // 自動標定下,無法看到標定中間過程.此句會改變原來的狀態,但是外面在重新幅值
		// weightCalADCDataLast = SDKADCFilterData.Current; // 更新
		*WeightPointer = SDKADCFilterData.Current;
		if (gu8v_WeighCalStep == STATE_WEIGHT_CALCHECK)
		{
			WeightPointer = &SDKWeight.CalADCDataTemp.Cal0; // 三次放置重物,不管結果指針回復初值,防止異常寫值
			SDKWeight.CalADCDataTemp.Cal3 = SDKWeight.CalADCDataTemp.Cal3 - SDKWeight.CalADCDataTemp.Cal2;
			SDKWeight.CalADCDataTemp.Cal2 = SDKWeight.CalADCDataTemp.Cal2 - SDKWeight.CalADCDataTemp.Cal1;
			SDKWeight.CalADCDataTemp.Cal1 = SDKWeight.CalADCDataTemp.Cal1 - SDKWeight.CalADCData.Cal0;
			SDKWeight.CalADCDataTemp.Cal0 = SDKWeight.CalADCData.Cal0;
			SDKWeight.Span = SDKWeight.CalADCDataTemp.Cal1 / WEIGHT_CAL1;
			unsigned long ABS1,ABS2;
			unsigned int temp;
			ABS1=fun_unsigned32BitABS(SDKWeight.CalADCDataTemp.Cal3, SDKWeight.CalADCDataTemp.Cal2);
			ABS2=fun_unsigned32BitABS(SDKWeight.CalADCDataTemp.Cal2, SDKWeight.CalADCDataTemp.Cal1);
			temp=(unsigned int)SDKWeight.Span * CALSPANCHECK;
			// 每段斜率之間差值不能超過8倍Span
			if (fun_unsigned32BitABS(SDKWeight.CalADCDataTemp.Cal3, SDKWeight.CalADCDataTemp.Cal2) < (unsigned int)SDKWeight.Span * CALSPANCHECK &&
				fun_unsigned32BitABS(SDKWeight.CalADCDataTemp.Cal2, SDKWeight.CalADCDataTemp.Cal1) < (unsigned int)SDKWeight.Span * CALSPANCHECK)
			{
				unsigned char *p;
				unsigned char eepromAddress = EEPROM_ADDR_START + 1;
				p = &SDKWeight.CalADCDataTemp.Cal0;
				for (; eepromAddress < EEPROM_ADDR_END; eepromAddress++)
				{
					Write_EEPROMByte(eepromAddress, *p);
					p++;
				}
				Write_EEPROMByte(EEPROM_ADDR_START, EEPROM_DATA_CALID_CODE);
				// 此部分賦值拿掉,這樣迫使生產需要掉電后在看數據
				// 防止EEPROM未正常寫入,但產線未掉電而流出
				// weightCalADCData1 = weightCalADCData1Temp;
				// weightCalADCData2 = weightCalADCData2Temp;
				// weightCalADCData3 = weightCalADCData3Temp;
				BHSDKState = STATE_WEIGHT_CALPASS;
			}
			else
			{
				BHSDKState = STATE_WEIGHT_CALFAIL;
			}
		}
		weightCalADCDataLast = SDKADCFilterData.Current; // 更新
	}
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ========================================@
//               正常稱重 代碼	     	    @
// ========================================@
// 接口變量
BodyScalesSetting_t SDKWeightSetting;	  // 用戶設置值
volatile typedefWeight haltWeightADCData;  // 休眠殘留ADC值，用於自動喚醒,休眠前重量可能滿量程,需要使用typedefWeight
volatile unsigned int AutoOnWeightADCData; // 自動開機ADC值,自動開機重量較小,int 型即可

void fun_WeightPowerOn()
{
	fun_LoadUserSetting();
	// weight PowerOn config
	if (Read_EEPROMByte(EEPROM_ADDR_START) == EEPROM_DATA_CALID_CODE)
	{
		unsigned char *p;
		unsigned char eepromAddress;
		eepromAddress = EEPROM_ADDR_START + 1;
		p = &SDKWeight.CalADCData.Cal0;
		for (; eepromAddress < EEPROM_SPAN_ADDR; eepromAddress++)
		{
			*p = Read_EEPROMByte(eepromAddress);
			p++;
		}
		// 地址不连续，span单独读
		SDKWeight.Span = Read_EEPROMByte(EEPROM_SPAN_ADDR);
	}
	else
	{
		SDKWeight.CalADCData.Cal0 = CAL0DATA_DEFAULT;
		SDKWeight.CalADCData.Cal1 = CAL1DATA_DEFAULT;
		SDKWeight.CalADCData.Cal2 = CAL2DATA_DEFAULT;
		SDKWeight.CalADCData.Cal3 = CAL3DATA_DEFAULT;
		SDKWeight.Span = CALDATASPAN_DEFAULT;
	}
	AutoOnWeightADCData = SDKWeight.Span * SDKWeightSetting.WeightAutoOn;
	gu8v_WeighWorkStep = STATE_WEIGHT_PREPARE;
}
void fun_WeightPowerDown()
{
	gu8v_WeighWorkStep = STATE_WEIGHT_PREPARE;
	SDKWeight.DataCurrent = 0;
	SDKWeight.DataStable = 0;
	SDKWeightSetting.flag.b.IsAutoCalOn = 0;
	BHSDKState = ENTER_WEIGHT_AUTOON;
	fun_ADCStop();
}
/***********************************
Function: 稱重硬件配置
INPUT	:
OUTPUT	:
NOTE	:
***********************************/
void fun_WeightHardwareSetting()
{
	SET_ADCCHAN_WEIGHT();
	SET_ADCGAIN_WEIGHT();
	SET_DCSET_WEIGHT();
	SET_ADCVREF_WEIGHT();
}
/***********************************
Function: 利用ADC值得到體重值
INPUT	:
OUTPUT	:
NOTE	:
***********************************/
void fun_GetWeighData()
{
	if (SDKADCFilterData.Current < SDKWeight.CalADCData.Cal0) // TODO 直接在此做小於秤台重量更新為0
	{
		SDKWeight.DataCurrent = 0;
		haltWeightADCData = 0;
	}
	else
	{
		haltWeightADCData = SDKADCFilterData.Current - SDKWeight.CalADCData.Cal0;
		if (haltWeightADCData <= SDKWeight.CalADCData.Cal1)
		{
			SDKWeight.DataCurrent = WEIGHT_CAL1 * (u32)haltWeightADCData / SDKWeight.CalADCData.Cal1;
		}
		else if (haltWeightADCData <= (SDKWeight.CalADCData.Cal1 + SDKWeight.CalADCData.Cal2))
		{
			SDKWeight.DataCurrent = WEIGHT_CAL1 + WEIGHT_CAL2 * (u32)(haltWeightADCData - SDKWeight.CalADCData.Cal1) / (u32)SDKWeight.CalADCData.Cal2;
		}
		else
		{
			SDKWeight.DataCurrent = WEIGHT_CAL1 + WEIGHT_CAL2 + WEIGHT_CAL3 * (u32)(haltWeightADCData - SDKWeight.CalADCData.Cal2 - SDKWeight.CalADCData.Cal1) / (u32)SDKWeight.CalADCData.Cal3;
		}
		// 四捨五入
		// SDKWeight.DataCurrent = (SDKWeight.DataCurrent + 5) / 10;
	}
}
/***********************************
Function: 普通稱重模式
INPUT	:
OUTPUT	:
NOTE	: 包含初次上電的校準
************************************/
void fun_Weight_Normal()
{
	fun_Filtering();
	if (SDKADCFilterData.flag.b.IsReady)
	{
		SDKADCFilterData.flag.b.IsReady = 0;
		fun_GetWeighData();
		// 人為更新零點,常用與上電第一次重量直接為零點重量
		if (SDKWeight.flag.b.IsNeedTare && SDKADCFilterData.flag.b.IsStable)
		{
			SDKWeight.flag.b.IsNeedTare = 0;
			SDKWeight.CalADCData.Cal0 = SDKADCFilterData.Current;
			SDKWeight.DataCurrent = 0x00;
			// TODO 零點寫入EEPROM
		}
		// 小於稱重範圍
		if (SDKWeight.DataCurrent < SDKWeightSetting.WeightMin)
		{
			// 小重量追零動作
			SDKWeight.DataCurrent = 0;
			if (SDKADCFilterData.flag.b.IsStable)
			{
				SDKWeight.CalADCData.Cal0 = SDKADCFilterData.Current;
				// for auto weight cal
				WeightPointer = &SDKWeight.CalADCDataTemp.Cal0;
				gu8v_WeighCalStep = STATE_WEIGHT_CAL1;
			}
			if (BHSDKState == STATE_WEIGHT_LOADOK || BHSDKState == STATE_WEIGHT_LOADFIX || BHSDKState == STATE_WEIGHT_LOADDOWN)
			{
				BHSDKState = STATE_WEIGHT_LOADDOWN; // 當前重量為0且得到一筆有效重量,為下秤動作
			}
			else
			{
				BHSDKState = STATE_WEIGHT_NOLOAD; //	當前重量為0且沒有得到有效重量,為空載
			}
		}
		// 大於稱重範圍
		else if (SDKWeight.DataCurrent > SDKWeightSetting.WeightMax && !SDKWeightSetting.flag.b.IsAutoCalOn) // 開啟自動校準不做超載處理
		{
			BHSDKState = STATE_WEIGHT_OVERLOAD;
		}
		// 正常稱重範圍
		else
		{
			if (SDKADCFilterData.flag.b.IsStable) // 當前重量為穩定的
			{
				if (BHSDKState == STATE_WEIGHT_LOADOK || BHSDKState == STATE_WEIGHT_LOADFIX)
				{
					BHSDKState = STATE_WEIGHT_LOADFIX;
					if (fun_unsigned32BitABS(SDKWeight.DataCurrent, SDKWeight.DataStable) > SDKWeightSetting.WeightUnLockStable)
					{
						SDKWeight.DataStable = SDKWeight.DataCurrent;
						BHSDKState = STATE_WEIGHT_LOADOK;
					}
				}
				if (BHSDKState == STATE_WEIGHT_LOADUP || BHSDKState == STATE_WEIGHT_OVERLOAD)
				{
					if (SDKWeightSetting.flag.b.IsAutoCalOn) // 啟用自動校準
					{
						fun_weightAutoCal();
					}
					SDKWeight.DataStable = SDKWeight.DataCurrent;
					if (BHSDKState != STATE_WEIGHT_CALPASS)
					{
						BHSDKState = STATE_WEIGHT_LOADOK;
					}
				}
			}
			else //  當前重量為不穩定
			{
				if (BHSDKState == STATE_WEIGHT_LOADOK || BHSDKState == STATE_WEIGHT_LOADFIX)
				{
					// 自動解鎖功能 3KG
					if (SDKWeightSetting.flag.b.IsAutoUnlockOn)
					{
						if ((SDKWeight.DataCurrent > SDKWeight.DataStable) && (SDKWeight.DataCurrent - SDKWeight.DataStable > SDKWeightSetting.WeightUnLockTemp)) // 下秤即歸零
						{
							BHSDKState = STATE_WEIGHT_LOADUP;
						}
					}
				}
				else
				{
					BHSDKState = STATE_WEIGHT_LOADUP;
				}
			}
		}
		gu8v_WeighWorkStep = BHSDKState; // 作為不同切換時的保存值
	}
}
/**************************************
Function: 自動上稱判斷
INPUT	:
OUTPUT	:
NOTE	: 用於快速喚醒
***************************************/
volatile u32 SDKADCFilterDatatemp;
void fun_Weight_AutoOn()
{
	if (SDKADCSourceData.flag.b.IsReady)
	{
		SDKADCSourceData.flag.b.IsReady = 0;
		// 去除正負轉換為單向線性,并統一為20Bit
	#if ADCSourceUseBit == 24
		__32_type temp;
		temp.byte.byte0 = SDKADCSourceData.ByteLow;
		temp.byte.byte1 = SDKADCSourceData.ByteMid;
		temp.byte.byte2 = SDKADCSourceData.ByteHigh + 0x80;
		temp.byte.byte3 = 0x00;
		#if WeightADCFilterUseBit == 16
			temp.u32 = temp.u32 >> 8; // 取16bit
		#endif
		#if WeightADCFilterUseBit == 20
			temp.u32 = temp.u32 >> 4; // 取20bit
		#endif
	#endif

	#if ADCSourceUseBit == 20
		__32_type temp;
		temp.byte.byte0 = SDKADCSourceData.ByteLow;
		temp.byte.byte1 = SDKADCSourceData.ByteMid;
		temp.byte.byte2 = SDKADCSourceData.ByteHigh + 0x08;
		temp.byte.byte2 &= 0x0F;
		temp.byte.byte3 = 0x00;
		#if WeightADCFilterUseBit == 16
			temp.u32 = temp.u32 >> 4; // 取16bit
		#endif
		#if WeightADCFilterUseBit == 20
			temp.u32 = temp.u32 >> 0; // 取20bit
		#endif
	#endif
		if (BHSDKState == STATE_AUTOON_SLOWMODE)
		{
			SDKADCFilterData.Current = 0;
		}
		//自動上秤累加數據4筆求平均
		SDKADCFilterDatatemp = SDKADCFilterDatatemp + temp.u32;
		if (SDKADCSourceData.SamplingCnt == 7)
		{
		
			if (BHSDKState == STATE_AUTOON_FASTMODE)
			{
				if((SDKADCFilterDatatemp/ 4)<65535)
				{
					SDKADCFilterData.Current =(int)(SDKADCFilterDatatemp/ 4); // 快速頻率,四筆求平均
				}
				else
				{
					SDKADCFilterData.Current =0xffff;
				}
			}
			else
			{
				SDKADCFilterData.Current=(int)temp.u32;
			}
			SDKADCFilterDatatemp=0;
			// 是否達到自動上稱重量判斷
			if (SDKADCFilterData.Current > SDKWeight.CalADCData.Cal0)
			{
				if (((SDKADCFilterData.Current - SDKWeight.CalADCData.Cal0) > AutoOnWeightADCData) && (haltWeightADCData < AutoOnWeightADCData))
				{
					if (BHSDKState == STATE_AUTOON_SLOWMODE)
					{
						BHSDKState = STATE_AUTOON_PASS;
						fun_ADCStop();
					}
					else
					{
						fun_WeightHardwareSetting();
						fun_ADCStart();
						BHSDKState = STATE_AUTOON_SLOWMODE; // 這個數據會極大限制，需要特別注意
						SDKADCSourceData.SamplingCnt = 3;   // 低速只需要一筆數據即可,這樣去除前三筆第四筆即可
					}
				}
				else
				{
					haltWeightADCData = SDKADCFilterData.Current - SDKWeight.CalADCData.Cal0;
					BHSDKState = STATE_AUTOON_FAIL;
					fun_ADCStop();
				}
			}
			else
			{
				haltWeightADCData = 0;
				BHSDKState = STATE_AUTOON_FAIL;
				fun_ADCStop();
			}
		}
	}
}
void fun_Enter_Weight_Normal()
{
	BHSDKState = gu8v_WeighWorkStep;
	// 防止中間ADC模式，在切回時狀態丟失
	if (BHSDKState == STATE_WEIGHT_LOADOK)
	{
		BHSDKState = STATE_WEIGHT_LOADFIX;
	}
	fun_WeightHardwareSetting();
	fun_LoadWeightSetting();
	fun_ADCStart();
	fun_FilterInit();
}
void fun_Enter_Weight_AutoOn()
{
	BHSDKState = STATE_AUTOON_FASTMODE;
	fun_WeightHardwareSetting();
	fun_LoadAutoOnSetting();
	GCC_DELAY(800);
	fun_ADCStart();
	fun_FilterInit();
}
void fun_Enter_Weight_Cal()
{
	BHSDKState = STATE_WEIGHT_CAL0;
	gu8v_WeighCalStep = STATE_WEIGHT_CAL0;
	WeightPointer = &SDKWeight.CalADCData.Cal0;
	SDKWeight.CalADCData.Cal0 = 0;
	SDKWeight.CalADCData.Cal1 = 0;
	SDKWeight.CalADCData.Cal2 = 0;
	SDKWeight.CalADCData.Cal3 = 0;
	fun_WeightHardwareSetting();
	fun_LoadWeightSetting();
	fun_ADCStart();
	fun_FilterInit();
}
void fun_weight_Cal()
{
	fun_Filtering();
	if (SDKADCFilterData.flag.b.IsStable)
	{
		SDKADCFilterData.flag.b.IsStable = 0;
		if (gu8v_WeighCalStep == STATE_WEIGHT_CAL0)
		{
			SDKWeight.CalADCData.Cal0 = SDKADCFilterData.Current;
			// for weight cal
			WeightPointer = &SDKWeight.CalADCDataTemp.Cal0;
			weightCalADCDataLast = SDKWeight.CalADCData.Cal0;
			gu8v_WeighCalStep = STATE_WEIGHT_CAL1;
			BHSDKState = gu8v_WeighCalStep;
		}
		else
		{
			fun_weightAutoCal();
		}
	}
}