#include "common.h"

u8  gu8_worktasks;//体脂称工作模式.
u16 gu16_weigh;    //显示体重前的原始体重,精度为0.1jin的体重数据。
u16 gu16_rememberweigh;//200g记忆体重，精度为0.1jin的体重数据。
u16 gu16_currentweigh;
u16 gu16_impedence_data;//原始阻抗数据.
u16 gu16_BodyFatRate;//来自蓝牙的体脂率.

/********************************************************************
Function: 设置秤工作状态.
INPUT	:
OUTPUT	:
NOTE	:.
********************************************************************/
void set_BHSDKState(u8 state)
{
	BHSDKState = state;
}

u8 is_BHSDKState_change(void)
{
	static u8 BHSDKState_old = ENTER_SDK_NONE;
	if(BHSDKState_old != BHSDKState){
		BHSDKState_old = BHSDKState;
		return 1;
	}
	return 0;
}


//void fun_DisplayMode_LoadFix(void)
//{
//	if(is_BHSDKState_change()){
//		fun_Unit_Change(SDKWeight.DataStable);
//		Set_DisplayMode(DISPLAY_LOADFIX);
//	}
//}


//void fun_DisplayMode_LoadDown(void)
//{
//	fun_Unit_Change(SDKWeight.DataStable);
//	Set_DisplayMode(DISPLAY_LOADDOWN);
//}

//void fun_DisplayMode_OverLoad(void)
//{
//	Set_DisplayMode(DISPLAY_OVERLOAD);
//}

//void fun_DisplayMode_ImpedanceFinish(void)
//{
//	Set_DisplayMode(DISPLAY_IMPEDANCE_FINISH);
//	gu16_impedence_data = SDKImpedance.Data;
//}


/********************************************************************
Function: 依据 BHSDKState状态完成UI或者其他動作.
INPUT	:
OUTPUT	:
NOTE	: 秤正在工作的所有状态BHSDKState.
********************************************************************/
void task_bodyfatscales(void)
{
	//依據BHSDKState完成UI或者其他動作
	switch (BHSDKState)
	{
		case STATE_WEIGHT_PREPARE:	// 稱重準備中
			// 切換為ENTER_WEIGHT_NORMAL后,通常有300ms左右稱重準備時間
			break;

		case STATE_WEIGHT_NOLOAD:// 空載/當前重量小於最小稱重重量
			// fun_DisplayMode_NoLoad();此處寫User UI,比如顯示臨時重量SDKWeight.DataCurrent
            if(is_BHSDKState_change()){
                fun_Unit_Change(SDKWeight.DataCurrent);
                Set_DisplayMode(DISPLAY_NOLOAD);

				if(fg_manual_readycal){
					fg_manual_readycal = 0;
					set_BHSDKState(ENTER_WEIGHT_CAL);
				}
            }
			break;

		case STATE_WEIGHT_LOADUP:	// 有上稱動作/鎖定狀態下加載解鎖重量,重量由0點變成>最小稱重重量
		// fun_DisplayMode_LoadUp();此處寫User UI,比如顯示臨時重量SDKWeight.DataCurrent
		fun_Unit_Change(SDKWeight.DataCurrent);
		Set_DisplayMode(DISPLAY_LOADUP);
		gbv_TxSDKWeightStatus = 1;
		gu8_data_type = DATA_TYPE_ING;
		break;

		case STATE_WEIGHT_LOADOK:	// 完成一次稱重測量
            // fun_DisplayMode_LoadOk();此處寫User UI,比如開始閃爍穩定重量SDKWeight.DataState等
			if(C_UNLOCK_WEIGHT >= SDKWeight.DataStable){
				fun_Unit_Change(SDKWeight.DataCurrent);
				Set_DisplayMode(DISPLAY_UNLOCK_WEIGHT);
			}else{
				if(is_BHSDKState_change()){
					fg_loadok = 1;
					fun_Unit_Change(SDKWeight.DataStable);
					Set_DisplayMode(DISPLAY_LOADFIX);
				}
			}
			break;
		case STATE_WEIGHT_LOADFIX:	// 完成一次稱重測量后重量穩定沒有解鎖
			// fun_DisplayMode_LoadFix();此處寫User UI,,比如開始閃爍穩定重量SDKWeight.DataState等
			gu8_time_30s = C_TIME_30S;
			if(!fg_remember_200g){
				fg_remember_200g = 1;
				gu16_rememberweigh = SDKWeight.DataStable;
			}else{
				if(fun_unsigned32BitABS(gu16_rememberweigh,SDKWeight.DataStable) <= C_REMEBER_200G){
					GCC_NOP();
				}else{
					gu16_rememberweigh = SDKWeight.DataStable;
				}
			}
			if(C_UNLOCK_WEIGHT >= SDKWeight.DataStable){
				if(fun_unsigned32BitABS(SDKWeight.DataCurrent,gu16_currentweigh) > C_REMEBER_200G){
					gu16_currentweigh = SDKWeight.DataCurrent;
					fun_Unit_Change(SDKWeight.DataCurrent);
					set_overtime2poweroff(C_TIME_10S);//如果重量有变化更新时间不去睡眠.
				}else{
//					fun_Unit_Change(SDKWeight.DataStable);
					fun_Unit_Change(gu16_rememberweigh);

				}
				Set_DisplayMode(DISPLAY_UNLOCK_WEIGHT);
			}else{
				if(is_BHSDKState_change()){
					gbv_TxSDKWeightStatus = 1;
					gu8_data_type = DATA_TYPE_LOCK;
				}
				fun_Unit_Change(gu16_rememberweigh);
				Set_DisplayMode(DISPLAY_LOADFIX);

				//Ready to manual cal
				if(fg_manual_cal){
					if(C_WEIGHT_MANUALCAL < gu16_rememberweigh){
						fg_manual_cal = 0;
						fg_manual_readycal = 1;
					}
				}
			}

			break;
		case STATE_WEIGHT_LOADDOWN:	// 下秤動作
            // fun_DisplayMode_LoadDown();此處寫User UI,,比如顯示鎖定SDKWeight.DataState等
			if(C_UNLOCK_WEIGHT >= SDKWeight.DataStable){
				fun_Unit_Change(SDKWeight.DataCurrent);
				Set_DisplayMode(DISPLAY_UNLOCK_WEIGHT);
				if(is_BHSDKState_change()){
					gbv_TxSDKWeightStatus = 1;
				}
			}else{
				if(is_BHSDKState_change()){
					fun_Unit_Change(gu16_rememberweigh);
					Set_DisplayMode(DISPLAY_LOADDOWN);
				}
			}

			if(fg_manual_readycal){
				fg_manual_readycal = 0;
				set_BHSDKState(ENTER_WEIGHT_CAL);
			}
			break;
		case STATE_WEIGHT_OVERLOAD:	// 超重,當前重量大於最大稱重重量
			// fun_DisplayMode_OverLoad();此處寫User UI,,比如顯示-OL-等
			fun_Unit_Change(SDKWeight.DataCurrent);
			Set_DisplayMode(DISPLAY_OVERLOAD);
			gbv_TxSDKWeightStatus = 1;
			gu8_data_type = DATA_TYPE_OVERLOAD;
			break;
		case STATE_IMPEDANCE_REFERENCE1:// 正在量測參考電阻1
		case STATE_IMPEDANCE_REFERENCE2:// 正在量測參考電阻2
		case STATE_IMPEDANCE_CHECKBODY: // 判斷是否有人體
		case STATE_IMPEDANCE_RX:		// 正在量測待測阻抗
			// fun_DisplayMode_Impedanceing();此處寫User UI,,比如跑----/oooo提示阻抗測量中
			Set_DisplayMode(DISPLAY_IMPEDANCEING);
            gu16_impedence_data= SDKImpedance.Data;
			fg_bodyfatrate_rec_ok = 0;
			break;
		case STATE_IMPEDANCE_FINISH:	// 阻抗量測結束,此時可以讀取gu16v_CurrentImpedance_ohm
			// fun_DisplayMode_ImpedanceFinish();
			//BHSDKState = ENTER_IMPEDANCE;

			gu8_data_type = DATA_TYPE_LOCK;
			gu16_impedence_data = SDKImpedance.Data;
			Set_DisplayMode(DISPLAY_IMPEDANCE_FINISH);
		    break;
		case STATE_AUTOON_FASTMODE:		// 快速ADC自動上稱判斷
		case STATE_AUTOON_SLOWMODE:		// 慢速ADC自動上稱判斷
			// 通常這個時候無任何UI
			break;
		case STATE_AUTOON_PASS: 		// 自動上稱OK
			// 此時可以點亮顯示,并切換到普通稱重模式進行MCU的正常工作
			gu8_worktasks = TASK_WEIGHT_AUTOON;
			break;
		case STATE_AUTOON_FAIL: 		// 自動上秤失敗,此時請繼續讓 MCU 進入HALT狀態
			//fun_BodyFatScalesSDK_PowerDown();
			//GCC_HALT();
			gu8_worktasks = TASK_SLEEP;
			break;
		case STATE_WEIGHT_CAL0: // 正在標定零點
			// fun_DisplayMode_CAL0();此處寫User UI,,比如提示CAL 0kg
			fun_Unit_Change(0);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CAL1:	// 正在標定第1點
			// fun_DisplayMode_CAL1();此處寫User UI,,比如提示CAL 50kg
			fun_Unit_Change(1000);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CAL2:	// 正在標定第2點
			// fun_DisplayMode_CAL2();此處寫User UI,,比如提示CAL 100kg
			fun_Unit_Change(2000);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CAL3:	// 正在標定第3點
			// fun_DisplayMode_CAL3();此處寫User UI,,比如提示CAL 150kg
			fun_Unit_Change(3000);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CALPASS:	// 標定成功
			// fun_DisplayMode_CALPASS();此處寫User UI,,比如提示PASS
			Set_DisplayMode(DISPLAY_CALPASS);
			break;
		case STATE_WEIGHT_CALFAIL:	// 標定失敗
			// fun_DisplayMode_CALFAIL();此處寫User UI,,比如提示FIAS
			Set_DisplayMode(DISPLAY_CALFAIL);
			break;
		default:
			break;
	}

	is_BHSDKState_change();
}

/********************************************************************
Function: 上电开机才进入启动模式.
INPUT	:
OUTPUT	:
NOTE	:LED全显3S,3S后直到上电后秤归零完成才切换到正常称重模式.
********************************************************************/
void task_scalesstartup(void)
{
	Set_DisplayMode(DISPLAY_POWERON);
	if(fg_time_3s){
		if(!SDKWeight.flag.b.IsNeedTare){
			Set_AllLEDBuffer(0);
			gu8_worktasks = TASK_SCALES;
			BHSDKState = ENTER_WEIGHT_NORMAL;
		}
	}
}

/********************************************************************
Function: 低电模式.
INPUT	:
OUTPUT	:
NOTE	:串口接收到蓝牙低电命令,就进入低电模式,显示3S后关机.
********************************************************************/
void task_scaleslowbattry(void)
{
	Set_DisplayMode(DISPLAY_LOWBATTERY);
}

/********************************************************************
Function: 睡眠模式.
INPUT	:
OUTPUT	:
NOTE	:睡眠初始化，并打开看门狗每1S唤醒检测是否上称成功，上称失败则继续睡眠.
********************************************************************/
void task_scales2sleep(void)
{
	_t0on  = 0;
	_t1on  = 0;
	_t2on  = 0;
	_tbon = 0;
	Set_DisplayMode(DISPLAY_ALLOFF);
	LED_Init();
	SETWDTTIME_1000MS();
	SET_UART_DISABLE();

    P_BLE_EN = HIGH;

	flag0_time_Byte = 0x00;
	flag1_Byte = 0x00;
	flag2_Byte = 0x00;
	flag_led_Byte = 0x00;

	set_overtime2poweroff(C_TIME_10S);

	if(gu8_time_30s){
		gu8_time_30s--;
	}else{
		fg_remember_200g = 0;
	}
	fun_BodyFatScalesSDK_PowerDown();
	_idlen = 0;
	GCC_NOP();
	GCC_NOP();
	GCC_HALT();
}

/********************************************************************
Function: 唤醒模式.
INPUT	:
OUTPUT	:
NOTE	:睡眠唤醒上称成功后,进入唤醒模式进行一些设置,再进入正常称重模式.
********************************************************************/
void task_scaleswakeup(void)
{
	gu8_worktasks = TASK_SCALES;
	BHSDKState = ENTER_WEIGHT_NORMAL;

	P_BLE_EN = LOW;//打开蓝牙.
	gbv_UartRxSuccess = 0;
	gbv_IsBusyUartTx = 0;
	SET_UART_ENABLE();

	gu8_time_30s = C_TIME_30S;
	gu8_ble_count = 0;

	// TM0
	_tm0c0 = 0x20;		// fsys/16 4us
	_tm0c1 = 0xc1;		// TimeCnt Mode
	_tm0al = 500%256;	// 500*4us = 2ms;
	_tm0ah = 500/256;
	SETCTMA_ISR_ENABLE();
	_t0on  = 1;
	_emi = 1;
}

