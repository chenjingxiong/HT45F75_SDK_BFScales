#include "common.h"
u16 gu16v_weigh;
u16 gu16_display_weight;
u16 gu16v_impedence_data;
u8 gu8v_weigh_targeunit;
volatile u8  gu8v_05s_count;//led闪烁时亮灭之间的时间间隔的计数值.
volatile u8  gu8v_howtimes;//led闪烁多少次，计数一亮一灭算2次.
volatile u8  gu8v_led_speed;//led闪烁时亮灭之间的时间间隔,
volatile u8  gu8v_led_delay;//延迟x*100MS后开始执行闪烁功能.
u8  gu8v_timed_shutdown;
u8  gu8v_dismode;
u8 gu8v_worktasks;

//精度:0.1斤;	eg:weigh  =100,代表 100*0.1斤=10斤.
void fun_Unit_Change(u16 weigh)
{
	gu16v_weigh = weigh;

	if(UNIT_KG == gu8v_weigh_targeunit)
	{
		fg_led_unit_kg = 1;
		fg_led_unit_lb = 0;

		#if 1//(_UART_DEBUG == ENABLE)
			gu16_display_weight = (u32)weigh/2;///10; //KG換算 符号强制转换，防止溢出
		#else
			gu16v_Display_Weigh = weigh;
		#endif
	}
	else if(UNIT_LB == gu8v_weigh_targeunit)
	{
		fg_led_unit_kg = 0;
		fg_led_unit_lb = 1;
		gu16_display_weight = ((u32)weigh/2*(u32)22046/1000+5)/10 & 0xfffe; //LB換算
		/*说明:
			10(g) = 0.0220462(lb),0.1(lb) = 45.359237(g)

			 eg:
			 	若:weigh = 55
			 	则:weigh=55=550(g) -->因为此合泰库精度为10g，即1代表1*10=10(g);x*10=10x(g)
			 	计算:weigh*10000/45359=55*10000/45359
			 		取整数位等于12，代表1.2(lb)
		*/
	}
//	else if(UNIT_JIN == gu8v_weigh_targeunit)
//	{
//		fg_led_unit_kg = 0;
//		fg_led_unit_lb = 0;
//		gu16_display_weight = (u32)weigh;///5; 	//JIN  0.1斤  = 5(weigh) ,gu16v_Display_Weigh的个位代表小数点后一位，保留 一位小數點
//		/*说明:
//			eg:
//			   若:weigh = 44
//			   则:weigh=44=440(g) -->因为此合泰库精度为10g，即1代表1*10=10(g);x*10=10x(g)
//			   计算:weigh/5 = 44/5 = 8.8
//					取整数位等于8，代表0.8(斤)
//		*/
//	}
}

//===============================================================
//function: 设置LED闪烁.
//input   :
//			mode: LED显示的模式
//			ledflash: =0 关闭闪烁; =1 开启闪烁
//			count: 闪烁次数: =0 不闪烁, =x 闪烁x次.
//			speed: 闪烁的快慢,取值范围0~255,时间等于x100MS.
//			delay: 延迟 delay*100MS 时间后才开始执行闪烁功能
//			overtime:设置定时休眠时间,时间= 100ms*(overtime+delay)
//output  : NULL
//description: 进入休眠前的设置，和唤醒后的初始化.
//===============================================================
void set_ledflash(u8 mode, u8 ledflash, u8 count, u8 speed, u8 delay, u8 overtime)
{
	static u8 mode_backup = 0xaa;
	if(mode_backup != mode){
		mode_backup = mode;

		if(C_LED_FLASH_ON == ledflash){
			fg_led_delay = 0;
			fg_led_timing = 1;
			gu8v_05s_count = 0;
			gu8v_howtimes = count;
			gu8v_led_speed = speed;
			gu8v_led_delay = delay;
		}else{
			fg_led_timing = 0;
			if(overtime)fg_led_flash = 0;
		}
		fg_led_change = 0;
//		overtime = overtime + delay;
		set_overtime2poweroff(overtime);
	}
}

#if 0
u8 get_ledflash_status(void)
{
	return fg_led_timing;
}
#endif

//===============================================================
//function: 设置超时关机时间.
//input   : overtime:单位为100ms;	eg:overtime=10,则代表1s,即10*100ms=1S.
//output  : NULL
//description: 时间: 100MS*overtime.
//===============================================================
void set_overtime2poweroff(u8 overtime)
{
	gu8v_timed_shutdown = overtime;
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
//	gu16v_impedence_data = SDKImpedance.Data;
//}


void task_bodyfatscales(void)
{
	// 依據gu8v_BodyfatSDKState 完成UI或者其他動作
	switch (BHSDKState)
	{
		case STATE_WEIGHT_PREPARE:	// 稱重準備中
			// 切換為ENTER_WEIGHT_NORMAL后,通常有300ms左右稱重準備時間
			break;
		case STATE_WEIGHT_NOLOAD:	// 空載/當前重量小於最小稱重重量
			//fun_DisplayMode_NoLoad();//此處寫User UI,比如顯示臨時重量SDKWeight.DataCurrent
//			if(is_BHSDKState_change()){
				fun_Unit_Change(SDKWeight.DataCurrent);
				Set_DisplayMode(DISPLAY_NOLOAD);
//			}
			break;
		case STATE_WEIGHT_LOADUP:	// 有上稱動作/鎖定狀態下加載解鎖重量,重量由0點變成>最小稱重重量
			//fun_DisplayMode_LoadUp();//此處寫User UI,比如顯示臨時重量SDKWeight.DataCurrent
			fun_Unit_Change(SDKWeight.DataCurrent);
			Set_DisplayMode(DISPLAY_LOADUP);
			break;
		case STATE_WEIGHT_LOADOK:	// 完成一次稱重測量
	        // fun_DisplayMode_LoadOk();此處寫User UI,比如開始閃爍穩定重量SDKWeight.DataState等
			if(is_BHSDKState_change()){
				fg_loadok = 1;
				fun_Unit_Change(SDKWeight.DataStable);
				Set_DisplayMode(DISPLAY_LOADFIX);
			}
			break;
		case STATE_WEIGHT_LOADFIX:	// 完成一次稱重測量后重量穩定沒有解鎖
			//fun_DisplayMode_LoadFix();//此處寫User UI,,比如開始閃爍穩定重量SDKWeight.DataState等
			if(is_BHSDKState_change()){
				fun_Unit_Change(SDKWeight.DataStable);
				Set_DisplayMode(DISPLAY_LOADFIX);
			}
			break;
		case STATE_WEIGHT_LOADDOWN:	// 下秤動作
	        //fun_DisplayMode_LoadDown();//此處寫User UI,,比如顯示鎖定SDKWeight.DataState等
			if(is_BHSDKState_change()){
				fun_Unit_Change(SDKWeight.DataStable);
//				Set_DisplayMode(DISPLAY_LOADDOWN);
				Set_DisplayMode(DISPLAY_LOADFIX);//test
			}
			break;
		case STATE_WEIGHT_OVERLOAD:	// 超重,當前重量大於最大稱重重量
			//fun_DisplayMode_OverLoad();//此處寫User UI,,比如顯示-OL-等
			Set_DisplayMode(DISPLAY_OVERLOAD);
			break;
		case STATE_IMPEDANCE_REFERENCE1:// 正在量測參考電阻1
		case STATE_IMPEDANCE_REFERENCE2:// 正在量測參考電阻2
		case STATE_IMPEDANCE_CHECKBODY: // 判斷是否有人體
		case STATE_IMPEDANCE_RX:		// 正在量測待測阻抗
			//fun_DisplayMode_Impedanceing();// 此處寫User UI,,比如跑----/oooo提示阻抗測量中
			Set_DisplayMode(DISPLAY_IMPEDANCEING);
			break;
		case STATE_IMPEDANCE_FINISH:	// 阻抗量測結束,此時可以讀取gu16v_CurrentImpedance_ohm
			//fun_DisplayMode_ImpedanceFinish();//
//			BHSDKState = ENTER_IMPEDANCE;
			Set_DisplayMode(DISPLAY_IMPEDANCE_FINISH);
			gu16v_impedence_data = SDKImpedance.Data;
//			gbv_TxSDKImpedanceStatus = 1;
		    break;
		case STATE_AUTOON_FASTMODE:		// 快速ADC自動上稱判斷
		case STATE_AUTOON_SLOWMODE:		// 慢速ADC自動上稱判斷
			// 通常這個時候無任何UI
			break;
		case STATE_AUTOON_PASS:			// 自動上稱OK
			// 此時可以點亮顯示,并切換到普通稱重模式進行MCU的正常工作
			gu8v_worktasks = TASK_WEIGHT_AUTOON;
			break;
		case STATE_AUTOON_FAIL:			// 自動上秤失敗,此時請繼續讓 MCU 進入HALT狀態
//			fun_BodyFatScalesSDK_PowerDown();
//			GCC_HALT();
			gu8v_worktasks = TASK_SLEEP;
			break;
		case STATE_WEIGHT_CAL0:	// 正在標定零點
			// fun_DisplayMode_CAL0();此處寫User UI,,比如提示CAL 0kg

//			if(is_BHSDKState_change()){
				fun_Unit_Change(0);
				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CAL1:	// 正在標定第1點
			// fun_DisplayMode_CAL1();此處寫User UI,,比如提示CAL 50kg

//			if(is_BHSDKState_change()){
				fun_Unit_Change(1000);
				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CAL2:	// 正在標定第2點
			// fun_DisplayMode_CAL2();此處寫User UI,,比如提示CAL 100kg

//			if(is_BHSDKState_change()){
				fun_Unit_Change(2000);
				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CAL3:	// 正在標定第3點
			// fun_DisplayMode_CAL3();此處寫User UI,,比如提示CAL 150kg

//			if(is_BHSDKState_change()){
				fun_Unit_Change(3000);
				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CALPASS:	// 標定成功
			//fun_DisplayMode_CALPASS();//此處寫User UI,,比如提示PASS
			Set_DisplayMode(DISPLAY_CALPASS);
			break;
		case STATE_WEIGHT_CALFAIL:	// 標定失敗
			// fun_DisplayMode_CALFAIL();此處寫User UI,,比如提示FIAS
//			if(is_BHSDKState_change()){
				Set_DisplayMode(DISPLAY_CALFAIL);
//			}
			break;
		default:
			break;
	}

	is_BHSDKState_change();
}

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

//===============================================================
//function: 设置显示模式.
//input   : DISPLAY_POWERON ~ DISPLAY_XXX.
//output  : NULL
//description:.
//===============================================================
void Set_DisplayMode(u8 dismode)
{
	gu8v_dismode = dismode;
}

//===============================================================
//function: 获取显示模式.
//input   : NULL.
//output  : DISPLAY_POWERON ~ DISPLAY_XXX.
//description:.
//===============================================================
u8 Get_DisplayMode(void)
{
	return gu8v_dismode;
}


//===============================================================
//function: LED 显示模式设置.
//input   : NULL.
//output  : NULL
//description: 根据设置的显示模式来更新需要显示的数据等.
//===============================================================
void fun_DiaplsyMode(void)
{
	switch(Get_DisplayMode())
	{
		case DISPLAY_POWERON:
			Set_AllLEDBuffer(0);
			set_ledflash(DISPLAY_POWERON,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;

		case DISPLAY_LOWBATTERY:
			set_ledflash(DISPLAY_LOWBATTERY,C_LED_FLASH_OFF,0,0,0,C_TIME_3S);
			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR_O;
			gu8v_LED_Buffer[2] = LED_CHAR_L;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			fg_led_Byte = 0x00;
			break;

		case DISPLAY_NOLOAD:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_NOLOAD,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;

		case DISPLAY_LOADUP:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_LOADUP,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);//test
			break;

		case DISPLAY_LOADFIX:
			gbv_TxSDKWeightStatus = 1;
			fun_HEX2BCD(gu16_display_weight);
			if(fg_loadok){
				set_ledflash(DISPLAY_LOADFIX,C_LED_FLASH_ON,C_LED_FLASH_CNT,C_TIME_05S,C_LED_FLASH_DELAY,C_TIME_10S);
				if(0 == fg_led_timing){
					fg_loadok = 0;
					set_BHSDKState(ENTER_IMPEDANCE);
				}
			}
			break;

		case DISPLAY_LOADDOWN:
			set_ledflash(DISPLAY_LOADDOWN,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;

		case DISPLAY_OVERLOAD:
			set_ledflash(DISPLAY_OVERLOAD,C_LED_FLASH_OFF,0,0,0,C_TIME_5S);
			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR_L;
			gu8v_LED_Buffer[2] = LED_CHAR_O;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			fg_led_Byte = 0x00;
			break;

		case DISPLAY_IMPEDANCEING:
//			set_ledflash(DISPLAY_IMPEDANCEING,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			//测量体脂不稳定时会超过10s中，所以此时需要更新定时休眠的定时时间.

			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR__;
			gu8v_LED_Buffer[2] = LED_CHAR__;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			fg_led_Byte = 0x00;
			set_ledflash(DISPLAY_IMPEDANCEING,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);
			break;

		case DISPLAY_IMPEDANCE_FINISH:
			gbv_TxSDKImpedanceStatus = 1;
			switch(gu16v_impedence_data){

				case IMPEDANCE_ERR_NOTEST:
				case IMPEDANCE_ERR_ABNORMAL:
				case IMPEDANCE_ERR_OUTRANGE:
					break;

				default:
					set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_ON,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
					if(fg_led_change)
					{
						fun_HEX2BCD(gu16v_impedence_data);
					}else{
						fun_HEX2BCD(gu16_display_weight);
					}
					break;
			}

			if(0 == fg_led_timing){
				gu8v_worktasks = TASK_SLEEP;
				set_BHSDKState(ENTER_WEIGHT_NORMAL);
			}
			break;

		case DISPLAY_CAL:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_CAL,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);
			break;

		case DISPLAY_CALPASS:
//			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_CALPASS,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			if(0 == fg_led_timing){
				while(1){
					fg_led_Byte = 0x00;//unit:ble,pct,kg,lb.
					gu8v_LED_Buffer[NUM_QIAN] = LED_CHAR_P;//'P'
					gu8v_LED_Buffer[NUM_BAI]= LED_CHAR_A;  //'A'
					gu8v_LED_Buffer[NUM_SHI]= LED_CHAR_5;  //'S'
					gu8v_LED_Buffer[NUM_GE] = LED_CHAR_5;  //'S'
					GCC_CLRWDT();
				}
			}
			break;

		case DISPLAY_CALFAIL:

			break;

		default:
		case DISPLAY_ALLOFF:
			Set_AllLEDBuffer(0);
			set_ledflash(DISPLAY_ALLOFF,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;
	}
}

//===============================================================
//function: 是否执行休眠.
//input   : NULL.
//output  : NULL
//description:定时时间到则执行休眠.
//===============================================================
void is_timedshutdown(void)
{
	if(fg_time_10s){
		fg_time_10s = 0;
		gu8v_worktasks = TASK_SLEEP;
	}
}

void task_scales2sleep(void)
{

#if 1
	_t0on  = 0;
	Set_DisplayMode(DISPLAY_ALLOFF);
	LED_Init();
	SETWDTTIME_1000MS();
	if(STATE_WEIGHT_NOLOAD == BHSDKState || STATE_WEIGHT_LOADDOWN == BHSDKState ||STATE_AUTOON_FAIL == BHSDKState){
		_idlen = 0;
		_emi = 0;
//		_t0on  = 0;
//		_t1on  = 0;
//		_t2on  = 0;
//		_tbon = 0;
//		SETCTMP_ISR_ENABLE();
//		SETTIMEBASE0_ISR_Disable();
//		SETTIMEBASE1_ISR_Disable();
		fun_BodyFatScalesSDK_PowerDown();
		GCC_NOP();
		GCC_NOP();
		GCC_NOP();
		GCC_HALT();
	}
#else
	//for test
	gu8v_worktasks = TASK_SCALES;
#endif
}

void task_scaleswakeup(void)
{

//	Set_DisplayMode(DISPLAY_POWERON);

	gu8v_worktasks = TASK_SCALES;
	BHSDKState = ENTER_WEIGHT_NORMAL;

	// TM0
	_tm0c0 = 0x20;		// fsys/16 4us
	_tm0c1 = 0xc1;		// TimeCnt Mode
	_tm0al = 500%256;	// 500*4us = 2ms;
	_tm0ah = 500/256;
	SETCTMA_ISR_ENABLE();
	_t0on  = 1;
	_emi = 1;

}


void fun_ble_task(void)
{
	P_BLE_EN = 0;

}

