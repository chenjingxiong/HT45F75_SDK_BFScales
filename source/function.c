#include "common.h"
u16 gu16v_weigh;
u16 gu16v_lockweigh;
u16 gu16_display_weight;
u16 gu16v_impedence_data;
u16 gu16v_pct_data;

u8 gu8v_weigh_targeunit;
volatile u8  gu8v_05s_count;//led闪烁时亮灭之间的时间间隔的计数值.
volatile u8  gu8v_howtimes;//led闪烁多少次，计数一亮一灭算2次.
volatile u8  gu8v_led_speed;//led闪烁时亮灭之间的时间间隔,
volatile u8  gu8v_led_delay;//延迟x*100MS后开始执行闪烁功能.
volatile u8  gu8v_led_delay3S;

u8  gu8v_timed_shutdown;
u8  gu8v_dismode;
u8  gu8v_worktasks;

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
            if(is_BHSDKState_change()){
                fun_Unit_Change(SDKWeight.DataCurrent);
                Set_DisplayMode(DISPLAY_NOLOAD);
            }
			break;
		case STATE_WEIGHT_LOADUP:	// 有上稱動作/鎖定狀態下加載解鎖重量,重量由0點變成>最小稱重重量
			//fun_DisplayMode_LoadUp();//此處寫User UI,比如顯示臨時重量SDKWeight.DataCurrent
			fun_Unit_Change(SDKWeight.DataCurrent);
			Set_DisplayMode(DISPLAY_LOADUP);
			gbv_TxSDKWeightStatus = 1;
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
			//fun_DisplayMode_LoadFix();//此處寫User UI,,比如開始閃爍穩定重量SDKWeight.DataState等
			if(C_UNLOCK_WEIGHT >= SDKWeight.DataStable){
				if(SDKWeight.DataCurrent != SDKWeight.DataStable){
					set_overtime2poweroff(C_TIME_10S);//濡傛灉閲嶉噺鏈夊彉鍖?鏇存柊鏃堕棿涓嶅幓鐫＄湢.
				}
				fun_Unit_Change(SDKWeight.DataCurrent);
				Set_DisplayMode(DISPLAY_UNLOCK_WEIGHT);
			}else{
				gu8v_time_30s = C_TIME_30S;
				if(is_BHSDKState_change()){
					gbv_TxSDKWeightStatus = 1;
					if(!fg_remeber_200g){
						fg_remeber_200g = 1;
						gu16v_lockweigh = SDKWeight.DataStable;
					}else{
						if(fun_unsigned32BitABS(gu16v_lockweigh,SDKWeight.DataStable) <= C_REMEBER_200G){
							GCC_NOP();
						}else{
							gu16v_lockweigh = SDKWeight.DataStable;
						}
					}
					fun_Unit_Change(gu16v_lockweigh);
					Set_DisplayMode(DISPLAY_LOADFIX);
				}
			}

			break;
		case STATE_WEIGHT_LOADDOWN:	// 下秤動作
	        //fun_DisplayMode_LoadDown();//此處寫User UI,,比如顯示鎖定SDKWeight.DataState等
			if(C_UNLOCK_WEIGHT >= SDKWeight.DataStable){
				fun_Unit_Change(SDKWeight.DataCurrent);
				Set_DisplayMode(DISPLAY_UNLOCK_WEIGHT);
			}else{
				if(is_BHSDKState_change()){
					fun_Unit_Change(SDKWeight.DataStable);
					Set_DisplayMode(DISPLAY_LOADDOWN);
					//Set_DisplayMode(DISPLAY_LOADFIX);
				}
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
            gu16v_impedence_data= 0;
			fg_pct_ok = 0;
			break;
		case STATE_IMPEDANCE_FINISH:	// 阻抗量測結束,此時可以讀取gu16v_CurrentImpedance_ohm
			//fun_DisplayMode_ImpedanceFinish();//
//			BHSDKState = ENTER_IMPEDANCE;
			gu16v_impedence_data = SDKImpedance.Data;
			Set_DisplayMode(DISPLAY_IMPEDANCE_FINISH);
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
//			fun_Unit_Change(0);
//			Set_DisplayMode(DISPLAY_CAL);
			break;
		case STATE_WEIGHT_CAL1:	// 正在標定第1點
			// fun_DisplayMode_CAL1();此處寫User UI,,比如提示CAL 50kg
//			fun_Unit_Change(1000);
//			Set_DisplayMode(DISPLAY_CAL);
			break;
		case STATE_WEIGHT_CAL2:	// 正在標定第2點
			// fun_DisplayMode_CAL2();此處寫User UI,,比如提示CAL 100kg

//			if(is_BHSDKState_change()){
//				fun_Unit_Change(2000);
//				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CAL3:	// 正在標定第3點
			// fun_DisplayMode_CAL3();此處寫User UI,,比如提示CAL 150kg

//			if(is_BHSDKState_change()){
//				fun_Unit_Change(3000);
//				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CALPASS:	// 標定成功
			//fun_DisplayMode_CALPASS();//此處寫User UI,,比如提示PASS
			Set_DisplayMode(DISPLAY_CALPASS);
			break;
		case STATE_WEIGHT_CALFAIL:	// 標定失敗
			// fun_DisplayMode_CALFAIL();此處寫User UI,,比如提示FIAS
//			if(is_BHSDKState_change()){
//				Set_DisplayMode(DISPLAY_CALFAIL);
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
//u8 Get_DisplayMode(void)
//{
//	return gu8v_dismode;
//}


//===============================================================
//function: LED 显示模式设置.
//input   : NULL.
//output  : NULL
//description: 根据设置的显示模式来更新需要显示的数据等.
//===============================================================
void fun_DiaplsyMode(void)
{
	switch(gu8v_dismode)
	{
		case DISPLAY_POWERON:
            Set_AllLEDBuffer(1);
            if(fg_time_3s){
                Set_AllLEDBuffer(0);
                gu8v_worktasks = TASK_SCALES;
                BHSDKState = ENTER_WEIGHT_NORMAL;
                set_ledflash(DISPLAY_POWERON,C_LED_FLASH_OFF,0,0,C_TIME_3S,C_TIME_10S);
            }
			break;

		case DISPLAY_LOWBATTERY:
			set_ledflash(DISPLAY_LOWBATTERY,C_LED_FLASH_OFF,0,0,0,C_TIME_3S);
			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR_O;
			gu8v_LED_Buffer[2] = LED_CHAR_L;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			fg_led_Byte &= 0;//0x40;
			break;

		case DISPLAY_NOLOAD:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_NOLOAD,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;

		case DISPLAY_LOADUP:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_LOADUP,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);//涓嶇ǔ瀹氭椂涓嶅叧鏈?
			break;

		case DISPLAY_LOADFIX:
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
			fg_led_Byte &= 0;//0x40;
			do{
                gu8v_worktasks = TASK_SLEEP;
                GCC_CLRWDT();
            }while(!fg_time_10s);
			break;

		case DISPLAY_IMPEDANCEING:
//			set_ledflash(DISPLAY_IMPEDANCEING,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			//测量体脂不稳定时会超过10s中，所以此时需要更新定时休眠的定时时间.

			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR__;
			gu8v_LED_Buffer[2] = LED_CHAR__;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			fg_led_Byte &= 0x40;
			set_ledflash(DISPLAY_IMPEDANCEING,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);
			break;

		case DISPLAY_IMPEDANCE_FINISH:

			switch(gu16v_impedence_data)
			{
				case IMPEDANCE_ERR_NOTEST:
				case IMPEDANCE_ERR_ABNORMAL:
				case IMPEDANCE_ERR_OUTRANGE:
                    gu16v_impedence_data = 0x00;
					fg_pct_ok = 0;
					fg_led_unit_pct = 0;
                    if(0 == fg_led_timing){
                        set_BHSDKState(ENTER_WEIGHT_NORMAL);//test
                        set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_OFF,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
                    }
					break;

				default:
					gbv_TxSDKImpedanceStatus = 1;
                    if(fg_pct_ok){
                        set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_ON,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
                        if(fg_led_change)
                        {
                            fun_HEX2BCD(gu16v_pct_data);
                            fg_led_unit_pct = 1;
							fg_led_unit_kg = 0;
							fg_led_unit_lb = 0;
                        }else{
                            fun_HEX2BCD(gu16_display_weight);
                            fg_led_unit_pct = 0;
							if(UNIT_KG == gu8v_weigh_targeunit){
								fg_led_unit_kg = 1;
								fg_led_unit_lb = 0;
							}else if(UNIT_LB == gu8v_weigh_targeunit){
								fg_led_unit_kg = 0;
								fg_led_unit_lb = 1;
							}
                        }

                        if(0 == fg_led_timing){
                            set_BHSDKState(ENTER_WEIGHT_NORMAL);
                            fg_pct_ok = 0;
							fg_led_unit_pct = 0;
                        }
                    }else{
                    	 fg_led_unit_pct = 0;
						 if(UNIT_KG == gu8v_weigh_targeunit){
							 fg_led_unit_kg = 1;
							 fg_led_unit_lb = 0;
						 }else if(UNIT_LB == gu8v_weigh_targeunit){
							 fg_led_unit_kg = 0;
							 fg_led_unit_lb = 1;
						 }
                         fun_HEX2BCD(gu16_display_weight);
#if 0
                        if(!fg_time_test2){
                            fg_time_test2 = 1;
                            fg_time_test = 0;
                            gu8v_time_test = C_TIME_10S;
                        }else{
                            if(fg_time_test){
                                fg_time_test = 0;
                                fg_time_test2 = 0;
                                set_BHSDKState(ENTER_WEIGHT_NORMAL);
                                set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_OFF,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
                            }
                        }
#endif
                    }
					break;
			}
			break;

        case DISPLAY_UNLOCK_WEIGHT:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_UNLOCK_WEIGHT,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
            break;

//		case DISPLAY_CAL:
//			fun_HEX2BCD(gu16_display_weight);
//			set_ledflash(DISPLAY_CAL,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
//			set_overtime2poweroff(C_TIME_10S);
//			break;

		case DISPLAY_CALPASS:
			while(1){
				fg_led_Byte &= 0x40;//unit:ble,pct,kg,lb.
				gu8v_LED_Buffer[NUM_QIAN] = LED_CHAR_P;//'P'
				gu8v_LED_Buffer[NUM_BAI]= LED_CHAR_A;  //'A'
				gu8v_LED_Buffer[NUM_SHI]= LED_CHAR_5;  //'S'
				gu8v_LED_Buffer[NUM_GE] = LED_CHAR_5;  //'S'
				GCC_CLRWDT();
			}
			break;

//		case DISPLAY_CALFAIL:

//			break;

		default:
		case DISPLAY_ALLOFF:
			Set_AllLEDBuffer(0);
			set_ledflash(DISPLAY_ALLOFF,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;
	}

	if(DISPLAY_POWERON != gu8v_dismode && DISPLAY_ALLOFF != gu8v_dismode){
		//钃濈墮鍥炬爣鏄剧ず
		if(!P_BT_Status & !P_BLE_EN)
			fg_led_ble = 1;
		else
			fg_led_ble = 0;
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
    P_BLE_EN = HIGH;
	_t0on  = 0;
	Set_DisplayMode(DISPLAY_ALLOFF);
	LED_Init();
	SETWDTTIME_1000MS();
	SET_UART_DISABLE();
	fg_led_Byte = 0x00;
	flag0_time_Byte = 0x00;
	flag1_Byte = 0x00;
	flag2_Byte = 0x00;
//	if(STATE_WEIGHT_NOLOAD == BHSDKState || STATE_WEIGHT_LOADDOWN == BHSDKState\
//	   ||STATE_AUTOON_FAIL == BHSDKState){

		if(gu8v_time_30s){
			gu8v_time_30s--;
		}else{
			fg_remeber_200g = 0;
		}
		_t0on  = 0;
		_t1on  = 0;
		_t2on  = 0;
		_tbon = 0;
		fun_BodyFatScalesSDK_PowerDown();
		_idlen = 0;
		GCC_NOP();
		GCC_NOP();
		GCC_NOP();
		GCC_HALT();
//	}
}

void task_scaleswakeup(void)
{
	gu8v_worktasks = TASK_SCALES;
	BHSDKState = ENTER_WEIGHT_NORMAL;

	P_BLE_EN = LOW;//鎵撳紑钃濈墮.

	gbv_IsBusyUartTx = 0;
	SET_UART_ENABLE();

	gu8v_time_30s = C_TIME_30S;

	// TM0
	_tm0c0 = 0x20;		// fsys/16 4us
	_tm0c1 = 0xc1;		// TimeCnt Mode
	_tm0al = 500%256;	// 500*4us = 2ms;
	_tm0ah = 500/256;
	SETCTMA_ISR_ENABLE();
	_t0on  = 1;
	_emi = 1;
}

void fun_timing(void)
{
	if(!fg_time_3s){
		gu8v_led_delay3S++;
		if(C_TIME_3S <= gu8v_led_delay3S){
			gu8v_led_delay3S = 0;
			fg_time_3s = 1;
		}
	}



	if(C_TIMEING_CYCLE100MS >= gu8v_UartTxCycle) gu8v_UartTxCycle++;

	//???????????byte?????
	if(!gbv_UartRxSuccess && fg_uart_rec_start){
		if(gu8v_TBRxTimeOutCnt) gu8v_TBRxTimeOutCnt--;
		if(0 == gu8v_TBRxTimeOutCnt){
//				  gbv_UartRxSuccess = 1;
			fg_uart_rec_start = 0;
		}
	}

	if(C_TIME_1S <= ++gu8v_time_1s){
		gu8v_time_1s = 0;
		fg_time_1s = 1;
	}

	/* LED显示闪烁切换定时 */
	if(fg_led_timing){
		//先延??后执行LED闪烁功能
		if(!fg_led_delay){
			if(gu8v_led_delay)
				gu8v_led_delay--;
			else
				fg_led_delay = 1;
		}
		// 延时时间到后执行LED显示闪烁切换标志??
		if(fg_led_delay){
			gu8v_05s_count++;
			if(gu8v_led_speed <= gu8v_05s_count){
				gu8v_05s_count = 0;
				if(gu8v_howtimes){
					fg_led_flash = !fg_led_flash;//控制LED一亮一灭闪??
					//fg_led_change:可以用来控制闪烁时体脂与体重的轮流闪烁??
					//注意:最好在fg_led_flash=1??即LED处于熄灭状??
					if(fg_led_flash){
						fg_led_change = !fg_led_change;
					}
					gu8v_howtimes--;
				}else{
					fg_led_delay = 0;
					fg_led_timing = 0;
					fg_led_flash = 0;
					fg_led_change = 0;
				}
			}
		}
	}else{
		/* 定时休眠关机,延迟时间过后才开始计?? */
		if(gu8v_timed_shutdown){
			gu8v_timed_shutdown--;
			fg_time_10s = 0;
			//当定时休眠时间没到，也没执行LED闪烁功能??
			//fg_led_flash设为0，进行所有显示LED扫描，防止fg_led_flash=1但还没到定时时间就关闭所有LED了??
			fg_led_flash = 0;//防止显示出现错误关闭LED.
		}else{
			fg_time_10s = 1;
		}
	}
}
