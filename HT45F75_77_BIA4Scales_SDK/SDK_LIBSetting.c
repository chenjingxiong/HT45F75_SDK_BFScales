asm(" message' **************************************************** ' ");
asm(" message' *       2018 BY BEST HEALTH ELECTRONIC INC         * ' ");
asm(" message' *__________________________________________________* ' ");
asm(" message' *        SDK  NAME  : 	SDK_LIBSetting.c         * ' ");
asm(" message' *   Compiler   Ver. :      V3.52                   * ' ");
asm(" message' *   IDE3000    Ver. :      V7.96                   * ' ");
asm(" message' *   RELEASE    Ver. :      2.0.3                   * ' ");
asm(" message' *   RELEASE   DATA  :     2018/06/22               * ' ");
asm(" message' *__________________________________________________* ' ");
asm(" message' *    MCU / CFG Ver. :   BH66F2650 / 1.8            * ' ");
asm(" message' *                       BH66F2660 / 1.2            * ' ");
asm(" message' **************************************************** ' ");

#include "SDK_Interface.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  功能說明                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
1.實現體重、8電極體阻抗、心率量測
2.
*/

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  封庫設置                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
unsigned char const SDKVersion[] = {'2','0','4'};
#define HEART_FUNCTION 0 // 是否有心率功能
#define IMPEDANCE_FUNCTION 1 // 是否有阻抗功能
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  依賴參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void fun_WeightPowerOn();
void fun_WeightPowerDown();
void fun_Enter_Weight_Normal();
void fun_Enter_Weight_AutoOn();
void fun_ImpedancePowerDown();
void fun_Enter_Weight_Cal();
void fun_Enter_Impedance();
void fun_Enter_Heatrate();

void fun_Weight_Normal();
void fun_weight_Cal();
void fun_Impedance();
void fun_Heartrate();
void fun_Weight_AutoOn();
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  對外參數                                            @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  源代碼                                             @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 變量定義  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
unsigned char BHSDKState;
/***********************************************
 Function: 取得兩個值之間的差值
 INPUT	:
 OUTPUT	:
 NOTE	:
 **********************************************/
unsigned long fun_unsigned32BitABS(unsigned long a, unsigned long b)
{
	if (a > b)	{ return (a - b); }
	else		{ return (b - a); }
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                                  SDK 對外總接口				             	       @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/********************************************************************
Function: SDK上電初始化
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_BodyFatScalesSDK_PowerOn()
{
	fun_WeightPowerOn();
	BHSDKState = STATE_SDK_NONE;
}
/********************************************************************
Function: SDK掉電/休眠初始化
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_BodyFatScalesSDK_PowerDown()
{
	fun_WeightPowerDown();
	fun_ImpedancePowerDown();
}

/********************************************************************
Function: 工作模式調用
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
unsigned char gu8v_ADCState;	 // ADC工作模式
void fun_BodyFatScalesSDK()
{
	// 切換工作模式
	switch (BHSDKState)
	{
		case ENTER_SDK_NONE:
			gu8v_ADCState		 = BHSDKState;
			BHSDKState = STATE_SDK_NONE;
			break;
		case ENTER_WEIGHT_NORMAL:
			gu8v_ADCState        = BHSDKState;
			fun_Enter_Weight_Normal();
			break;
		case ENTER_WEIGHT_AUTOON:
			gu8v_ADCState 		 = BHSDKState;
			fun_Enter_Weight_AutoOn();
			break;
		case ENTER_WEIGHT_CAL:
			gu8v_ADCState 		 = BHSDKState;
			fun_Enter_Weight_Cal();
			break;
		#if IMPEDANCE_FUNCTION == 1
		case ENTER_IMPEDANCE:
			gu8v_ADCState 		 = BHSDKState;
			fun_Enter_Impedance();
			break;
		#endif
		#if HEART_FUNCTION == 1
		case ENTER_HEATRATE:
			gu8v_ADCState 		 = BHSDKState;
			fun_Enter_Heatrate();
			break;
		#endif
		default:
			break;
	}
	// 每種量測量具體處理
	switch (gu8v_ADCState)
	{
		case ENTER_WEIGHT_NORMAL:
			fun_Weight_Normal();
			break;
		case ENTER_WEIGHT_CAL:
			fun_weight_Cal();
			break;
		case ENTER_WEIGHT_AUTOON:
			fun_Weight_AutoOn();
			break;
#if IMPEDANCE_FUNCTION == 1
		case ENTER_IMPEDANCE:
			fun_Impedance();
			break;
#endif
#if HEART_FUNCTION == 1
		case ENTER_HEATRATE:
			fun_Heartrate();
			break;
#endif
		default:
			break;

	}
}
