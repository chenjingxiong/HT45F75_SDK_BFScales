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
//                                    �����f��    				             		    @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
1. �ԄӘ˶�,�˶������̶���50Kg/50Kg/50Kg
2. �քӘ˶�,�˶������̶���50Kg/50Kg/50Kg
3. �����������i,�����Ñ��������O��
4. �R�r�������i,�����Ñ��������O��
5. �Ԅӆ���,�����Ñ��������O��
6. ���m���V������16 / 20Bit ADC��̎��
6. ���m��ԭʼ����20 / 24Bit ADC��̎��,���m��HT45F7x����20Bit MCU Ҳ�m�����ڵ�24Bit MCU

1. ���Ñ�����Ҫ�ԄӘ˶� CalADCDataTemp ����CalADCData����,��Ȼ����
*/

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  ����O��                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include "HT45F75.h"
#define SET_ADCGAIN_WEIGHT()        { _pgac0 = 0x27;}   // �Q��ADC�Ŵ��O��,VGS=0.5,ADGN =1,PGA=128
#define SET_DCSET_WEIGHT()          { _pgac1 = 0x00;}   // �Q��DCSET�O��,DCSET = 0V
#define SET_ADCCHAN_WEIGHT()        { _pgacs = 0x00;}   // �Q��ADC ͨ��AN0&AN1
#define SET_ADCVREF_WEIGHT()	    { _vrefs = 1; _vcms=1;}     // 稱重ADC 外部參考電�?

#define CALSPANCHECK 16				// �����˶��Y�������a֮�g�����S������ֵ16*Span
#define WEIGHT_CAL1 (50 * 20)		// ����У���c1
#define WEIGHT_CAL2 (50 * 20)		// ����У���c2
#define WEIGHT_CAL3 (50 * 20)		// ����У���c3
#define EEPROM_ADDR_START 0x01		// EEPROM �惦�_ʼλ��
#define EEPROM_DATA_CALID_CODE 0xAA // У�ʳɹ����I����,�ɜy���˔���,�J��У�ʳɹ�
#define WeightADCFilterUseBit 16	// 20 or 16 ���� ADC ����ʹ�õ�Bit��
#define ADCSourceUseBit 20			// 24 or 20  ADC ԭʼ ADC ����Bit��,��Ҫ��ƥ������20Bit MCU ����HT45F7x

#if WeightADCFilterUseBit == 16
	typedef unsigned int typedefWeight;
	#define EEPROM_ADDR_END (4 * 2 + EEPROM_ADDR_START + 2)
	#define EEPROM_SPAN_ADDR (4 * 2 + EEPROM_ADDR_START+1 )
	#define CAL_Diff_ADC_DEFAULT 1500
	//**************��У�ʅ����A�O***********
	#define CAL0DATA_DEFAULT 32600		// 0�cĬ�JУ��ֵ
	#define CAL1DATA_DEFAULT 5000		// ��1�cĬ�JУ��ֵ
	#define CAL2DATA_DEFAULT 5000		// ��2�cĬ�JУ��ֵ
	#define CAL3DATA_DEFAULT 5000		// ��3�cĬ�JУ��ֵ
	#define CALDATASPAN_DEFAULT 5		// Ĭ�J�ֱ��ʌ���ADCֵ
#endif
#if WeightADCFilterUseBit == 20
	typedef unsigned long typedefWeight;
	#define EEPROM_ADDR_END (4 * 4 + EEPROM_ADDR_START + 2)
	#define EEPROM_SPAN_ADDR (4 * 4 + EEPROM_ADDR_START+1 )
	#define CAL_Diff_ADC_DEFAULT 25000
	//**************��У�ʅ����A�O***********
	#define CAL0DATA_DEFAULT 555556		// 0�cĬ�JУ��ֵ
	#define CAL1DATA_DEFAULT 80000		// ��1�cĬ�JУ��ֵ
	#define CAL2DATA_DEFAULT 80000		// ��2�cĬ�JУ��ֵ
	#define CAL3DATA_DEFAULT 80000		// ��3�cĬ�JУ��ֵ
	#define CALDATASPAN_DEFAULT 80		// Ĭ�J�ֱ��ʌ���ADCֵ
#endif

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  ��ه����                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void fun_ADCStart();
void fun_ADCStop();
void fun_Filtering();
void fun_FilterInit();
void fun_LoadWeightSetting();
void fun_LoadAutoOnSetting();
void fun_LoadUserSetting();
unsigned int fun_unsigned32BitABS(unsigned int a, unsigned int b); // ȡ32λ�o��̖��ֵ
void Write_EEPROMByte(unsigned char addr, unsigned char WriteData);   // ��һ��Byte����
unsigned char Read_EEPROMByte(unsigned char addr);					  // �xһ��Byte����
extern unsigned char BHSDKState;     // RW   ������B�xȡ�c�ГQ,���� BodyfatSDKState ö�e
extern ADCSource_t SDKADCSourceData; // ԭʼADC����,Ԕ������SDK_typedef.h ADCSource_t
extern ADCFilter_t SDKADCFilterData; // �V��ADC����,Ԕ������SDK_typedef.h ADCFilter_t
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  ���Ⅲ��                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
typedef struct
{
    struct
    {
        unsigned long Cal0;	    // R    ADCֵ - ���c����
        unsigned long Cal1;	    // R    ADCֵ - ��1���˶�����
        unsigned long Cal2;	    // R    ADCֵ - ��2���˶�����
        unsigned long Cal3;	    // R    ADCֵ - ��3���˶�����
    }CalADCData;// У�ʔ���
    struct
    {
        unsigned long Cal0;	    // R    ADCֵ - ���c����
        unsigned long Cal1;	    // R    ADCֵ - ��1���˶�����
        unsigned long Cal2;	    // R    ADCֵ - ��2���˶�����
        unsigned long Cal3;	    // R    ADCֵ - ��3���˶�����
    }CalADCDataTemp; // У���Д���,for debug
    unsigned char Span;	        // R    ADCֵ - 1���ֱ���
	union {
		struct
		{
			unsigned char IsNeedTare : 1;  // RW ȥƤ,��ǰ�����鷀�������r��ǰ������0
		} b;
		unsigned char byte;
	} flag;
    unsigned int  DataCurrent;  // R    ��ǰ����ֵ,��λ��jin,�Ŵ�10��, ��500,��ʾ������50.0��
    unsigned int  DataStable;   // R    ��������ֵ,��λ��jin,�Ŵ�10��, ��500,��ʾ������50.0��
}Weight_t;

typedef struct
{
    unsigned long WeightMax;          // RW ���y������
    unsigned int WeightMin;           // RW ��С�y������
    unsigned int WeightAutoOn;        // RW �Զ���������
    unsigned int WeightUnLockTemp;    // RW �Ԅӽ��i����-�R�r
    unsigned char WeightUnLockStable; // RW �Ԅӽ��i����-����
    union {
        struct
        {
            unsigned char IsAutoCalOn : 1;    // �Ԅ�У�ʿ���
            unsigned char IsAutoUnlockOn : 1; // �Ԅӽ��i����
        } b;
        unsigned char byte;
    } flag;
} BodyScalesSetting_t;
*/
extern Weight_t SDKWeight;
extern BodyScalesSetting_t SDKWeightSetting; // �Ñ��O��ֵ


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
//                                  Դ���a                                             @
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
//                  У�� ���a	    	    @
// ========================================@
#define weightCalADCDataLast SDKWeight.CalADCDataTemp.Cal3 // ����һ�����õ�buf3,��̎����RAM
volatile unsigned int *WeightPointer; // ָ�ʹ���؄eע�⹠��,��ֹԽ�����ֵ������

/****************************************
 Function: �˶��Д�
 INPUT	:
 OUTPUT	:
NOTE	:  �B�m���������������ȥ
*****************************************/
void fun_weightAutoCal()
{
	if (SDKADCFilterData.Current > (weightCalADCDataLast + CAL_Diff_ADC_DEFAULT)) //�@��ADCֵ���ϴδ�
	{
		WeightPointer++;
		gu8v_WeighCalStep++;
		BHSDKState = gu8v_WeighCalStep;					 // �ԄӘ˶���,�o�������˶����g�^��.�˾����׃ԭ��Ġ�B,�������������·�ֵ
		// weightCalADCDataLast = SDKADCFilterData.Current; // ����
		*WeightPointer = SDKADCFilterData.Current;
		if (gu8v_WeighCalStep == STATE_WEIGHT_CALCHECK)
		{
			WeightPointer = &SDKWeight.CalADCDataTemp.Cal0; // ���η�������,���ܽY��ָᘻ؏ͳ�ֵ,��ֹ������ֵ
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
			// ÿ��б��֮�g��ֵ���ܳ��^8��Span
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
				// �˲����xֵ�õ�,�@����ʹ���a��Ҫ��늺��ڿ�����
				// ��ֹEEPROMδ��������,���a��δ��늶�����
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
		weightCalADCDataLast = SDKADCFilterData.Current; // ����
	}
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ========================================@
//               �����Q�� ���a	     	    @
// ========================================@
// �ӿ�׃��
BodyScalesSetting_t SDKWeightSetting;	  // �Ñ��O��ֵ
volatile typedefWeight haltWeightADCData;  // ���ߚ���ADCֵ������Ԅӆ���,����ǰ�������ܝM����,��Ҫʹ��typedefWeight
volatile unsigned int AutoOnWeightADCData; // �Ԅ��_�CADCֵ,�Ԅ��_�C�����^С,int �ͼ���

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
		// ��ַ��������span������
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
Function: �Q��Ӳ������
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
Function: ����ADCֵ�õ��w��ֵ
INPUT	:
OUTPUT	:
NOTE	:
***********************************/
void fun_GetWeighData()
{
	if (SDKADCFilterData.Current < SDKWeight.CalADCData.Cal0) // TODO ֱ���ڴ���С춳�̨��������0
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
		// �Ē�����
		// SDKWeight.DataCurrent = (SDKWeight.DataCurrent + 5) / 10;
	}
}
/***********************************
Function: ��ͨ�Q��ģʽ
INPUT	:
OUTPUT	:
NOTE	: ����������늵�У��
************************************/
void fun_Weight_Normal()
{
	fun_Filtering();
	if (SDKADCFilterData.flag.b.IsReady)
	{
		SDKADCFilterData.flag.b.IsReady = 0;
		fun_GetWeighData();
		// �˞�������c,�����c��늵�һ������ֱ�Ӟ����c����
		if (SDKWeight.flag.b.IsNeedTare && SDKADCFilterData.flag.b.IsStable)
		{
			SDKWeight.flag.b.IsNeedTare = 0;
			SDKWeight.CalADCData.Cal0 = SDKADCFilterData.Current;
			SDKWeight.DataCurrent = 0x00;
			// TODO ���c����EEPROM
		}
		// С춷Q�ع���
		if (SDKWeight.DataCurrent < SDKWeightSetting.WeightMin)
		{
			// С����׷�����
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
				BHSDKState = STATE_WEIGHT_LOADDOWN; // ��ǰ������0�ҵõ�һ�P��Ч����,���³ӄ���
			}
			else
			{
				BHSDKState = STATE_WEIGHT_NOLOAD; //	��ǰ������0�қ]�еõ���Ч����,����d
			}
		}
		// ��춷Q�ع���
		else if (SDKWeight.DataCurrent > SDKWeightSetting.WeightMax /*&& !SDKWeightSetting.flag.b.IsAutoCalOn*/) // �_���Ԅ�У�ʲ������d̎��
		{
			BHSDKState = STATE_WEIGHT_OVERLOAD;
		}
		// �����Q�ع���
		else
		{
			if (SDKADCFilterData.flag.b.IsStable) // ��ǰ�����鷀����
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
					if (SDKWeightSetting.flag.b.IsAutoCalOn) // �����Ԅ�У��
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
			else //  ��ǰ�����鲻����
			{
				if (BHSDKState == STATE_WEIGHT_LOADOK || BHSDKState == STATE_WEIGHT_LOADFIX)
				{
					// �Ԅӽ��i���� 3KG
					if (SDKWeightSetting.flag.b.IsAutoUnlockOn)
					{
						if ((SDKWeight.DataCurrent > SDKWeight.DataStable) && (SDKWeight.DataCurrent - SDKWeight.DataStable > SDKWeightSetting.WeightUnLockTemp)) // �³Ӽ��w��
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
		gu8v_WeighWorkStep = BHSDKState; // ���鲻ͬ�ГQ�r�ı���ֵ
	}
}
/**************************************
Function: �Ԅ��ϷQ�Д�
INPUT	:
OUTPUT	:
NOTE	: ��춿��ن���
***************************************/
volatile u32 SDKADCFilterDatatemp;
volatile unsigned int SDKADCData[4];
void fun_Weight_AutoOn()
{
	if (SDKADCSourceData.flag.b.IsReady)
	{
		SDKADCSourceData.flag.b.IsReady = 0;
		// ȥ����ؓ�D�Q�������,���yһ��20Bit
	#if ADCSourceUseBit == 24
		__32_type temp;
		temp.byte.byte0 = SDKADCSourceData.ByteLow;
		temp.byte.byte1 = SDKADCSourceData.ByteMid;
		temp.byte.byte2 = SDKADCSourceData.ByteHigh + 0x80;
		temp.byte.byte3 = 0x00;
		#if WeightADCFilterUseBit == 16
			temp.u32 = temp.u32 >> 8; // ȡ16bit
		#endif
		#if WeightADCFilterUseBit == 20
			temp.u32 = temp.u32 >> 4; // ȡ20bit
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
			temp.u32 = temp.u32 >> 4; // ȡ16bit
		#endif
		#if WeightADCFilterUseBit == 20
			temp.u32 = temp.u32 >> 0; // ȡ20bit
		#endif
	#endif
		if (BHSDKState == STATE_AUTOON_SLOWMODE)
		{
			SDKADCFilterData.Current = 0;
		}
		/*if (SDKADCSourceData.SamplingCnt)*/
		SDKADCData[SDKADCSourceData.SamplingCnt-4] =  temp.u32;

		//自動上秤累加數據4筆求平均
		SDKADCFilterDatatemp = SDKADCFilterDatatemp + temp.u32;
		if (SDKADCSourceData.SamplingCnt == 7)
		{

			if (BHSDKState == STATE_AUTOON_FASTMODE)
			{
				if((SDKADCFilterDatatemp/ 4)<65535)
				{
					SDKADCFilterData.Current =(int)(SDKADCFilterDatatemp/ 4); // �����l��,�ĹP��ƽ��
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
			// �Ƿ��_���Ԅ��ϷQ�����Д�
			if (SDKADCFilterData.Current > SDKWeight.CalADCData.Cal0)
			{
				if (((SDKADCFilterData.Current - SDKWeight.CalADCData.Cal0) > AutoOnWeightADCData) && (haltWeightADCData < (AutoOnWeightADCData-5*SDKWeight.Span)))
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
						BHSDKState = STATE_AUTOON_SLOWMODE; // �@���������O�����ƣ���Ҫ�؄eע��
						SDKADCSourceData.SamplingCnt = 3;   // ����ֻ��Ҫһ�P��������,�@��ȥ��ǰ���P���ĹP����
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
	// ��ֹ���gADCģʽ�����лؕr��B�Gʧ
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
//	GCC_DELAY(800);
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
