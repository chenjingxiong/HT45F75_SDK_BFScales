#include "common.h"

u8  gu8_dismode;//LED显示模式.
u16 gu16_display_weight;//已经转换单位后的显示体重数据.
u8 gu8_weigh_targeunit;//目标体重单位.

/* LED 闪烁，显示，定时等变量*/
volatile u8  gu8v_time_dalay; //延时gu8v_time_dalay*100Ms后开始执行闪烁.
volatile u8  gu8v_howtimes;  //LED闪烁次数.
volatile u8  gu8v_led_speed; //闪烁的快慢.
volatile u8  gu8v_led_delay; //延时delay时间后才执行闪烁.
volatile u8  gu8v_led_delay3S;//开机LED全显3s计时.
volatile u8  gu8v_timed_shutdown;//定时关机计时.

/********************************************************************
Function: 重量单位切换.
INPUT	:
OUTPUT	:
NOTE	://精度:0.1jin;	eg:weigh  =100,计算:100*0.1jin=10jin.
********************************************************************/
void fun_Unit_Change(u16 weigh)
{
	gu16_weigh = weigh;
	if(UNIT_KG == gu8_weigh_targeunit)
	{
		fg_led_unit_kg = 1;
		fg_led_unit_lb = 0;
		gu16_display_weight = (u32)weigh/2;
	}
	else if(UNIT_LB == gu8_weigh_targeunit)
	{
		fg_led_unit_kg = 0;
		fg_led_unit_lb = 1;
		gu16_display_weight = ((u32)weigh/2*(u32)22046/1000+5)/10 & 0xfffe;
	}
}

//===============================================================
//function: LED显示闪烁和定时时间更新.
//input   :
//			mode: LED显示模式
//			ledflash: =0 不闪烁; =1 闪烁.
//			count: 闪烁次数˸=0 不闪烁˸ =x 闪烁x次.一亮一灭count为2.
//			speed: 闪烁亮灭的速度,=X*100MS.
//			delay: 延时delay*100MS才开始后执行闪烁.
//			overtime:= LED显示时间(定时关机时间). =100ms*(overtime+delay)
//output  : NULL
//description:可以设置：1.LED是否闪烁; 2.LED闪烁次数; 3.闪烁的快慢;
//						4.延时delay*100MS时间后开始执行闪烁; 5.更新定时关机时间.
//===============================================================
void set_ledflash(u8 mode, u8 ledflash, u8 count, u8 speed, u8 delay, u8 overtime)
{
	static u8 mode_backup = 0xaa;
	if(mode_backup != mode){
		mode_backup = mode;

		if(C_LED_FLASH_ON == ledflash){
			fg_led_delay = 0;
			fg_led_timing = 1;
			gu8v_time_dalay = 0;
			gu8v_howtimes = count;
			gu8v_led_speed = speed;
			gu8v_led_delay = delay;
		}else{
			fg_led_timing = 0;
			if(overtime)fg_led_flash = 0;
		}
		fg_led_change = 0;
		set_overtime2poweroff(overtime);
	}
}

/********************************************************************
Function: LED闪烁状态.
INPUT	:
OUTPUT	:
NOTE	:获取LED闪烁状态.
********************************************************************/
u8 get_ledflash_status(void)
{
	return fg_led_timing;
}


//===============================================================
//function: 设置显示模式
//input   : DISPLAY_POWERON ~ DISPLAY_XXX.
//output  : NULL
//description:.
//===============================================================
void Set_DisplayMode(u8 dismode)
{
	gu8_dismode = dismode;
}

//===============================================================
//function: 获取显示模式.
//input   : NULL.
//output  : DISPLAY_POWERON ~ DISPLAY_XXX.
//description:.
//===============================================================
//u8 Get_DisplayMode(void)
//{
//	return gu8_dismode;
//}


/********************************************************************
Function: LED需要显示状态，显示内容buffer填充.
INPUT	:
OUTPUT	:
NOTE	:LED显示和显示时间定时设置.
********************************************************************/
void fun_diaplay_mode(void)
{
	switch(gu8_dismode)
	{
		case DISPLAY_POWERON:
            Set_AllLEDBuffer(1);
			//更新10s时间，TASK_STARTUP状态当3s时间到但秤归零没完成就不让scale进入睡眠.
			set_overtime2poweroff(C_TIME_10S);
			set_ledflash(DISPLAY_POWERON,C_LED_FLASH_OFF,0,0,C_TIME_3S,C_TIME_10S);
			break;

		case DISPLAY_LOWBATTERY:
			set_ledflash(DISPLAY_LOWBATTERY,C_LED_FLASH_OFF,0,0,0,C_TIME_3S);
			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR_O;
			gu8v_LED_Buffer[2] = LED_CHAR_L;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			flag_led_Byte &= 0x40;
			break;

		case DISPLAY_NOLOAD:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_NOLOAD,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;

		case DISPLAY_LOADUP:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_LOADUP,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);//不稳定时不关�?
			break;

		case DISPLAY_LOADFIX:
			fun_HEX2BCD(gu16_display_weight);
			if(fg_loadok){
				set_ledflash(DISPLAY_LOADFIX,C_LED_FLASH_ON,C_LED_FLASH_CNT,C_TIME_05S,C_LED_FLASH_DELAY,C_TIME_10S);
				if(0 == get_ledflash_status()){
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
			flag_led_Byte &= 0x00;//0x40;
			if(!gbv_TxSDKWeightStatus){//直到启动发送超重信息.
				while(!fg_time_10s){
					gu8_worktasks = TASK_SLEEP;
					GCC_CLRWDT();
				}
			}
			break;

		case DISPLAY_IMPEDANCEING:
			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR__;
			gu8v_LED_Buffer[2] = LED_CHAR__;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			flag_led_Byte &= 0x40;
			set_ledflash(DISPLAY_IMPEDANCEING,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);
			break;

		case DISPLAY_IMPEDANCE_FINISH:

			switch(gu16_impedence_data)
			{
				case IMPEDANCE_ERR_NOTEST:
				case IMPEDANCE_ERR_ABNORMAL:
				case IMPEDANCE_ERR_OUTRANGE:
                    gu16_impedence_data = 0x00;
					fg_bodyfatrate_rec_ok = 0;
					fg_led_unit_pct = 0;
                    set_BHSDKState(ENTER_WEIGHT_NORMAL);
                    set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_OFF,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
					break;

				default:
					gbv_TxSDKImpedanceStatus = 1;
					if(fg_led_ble){
						//蓝牙有连接,发送完阻抗并等待接收体脂率.
	                    if(fg_bodyfatrate_rec_ok){
	                        set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_ON,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
	                        if(fg_led_change)
	                        {
	                        	if(C_BODYFATRATE_MIN > gu16_BodyFatRate){
									flag_led_Byte &= 0x40;
									gu8v_LED_Buffer[0] = LED_CHAR_OFF;
									gu8v_LED_Buffer[1] = LED_CHAR_L;
									gu8v_LED_Buffer[2] = LED_CHAR_OFF;
									gu8v_LED_Buffer[3] = LED_CHAR_OFF;
								}else if(C_BODYFATRATE_MAX < gu16_BodyFatRate){
									flag_led_Byte &= 0x40;
									gu8v_LED_Buffer[0] = LED_CHAR_OFF;
									gu8v_LED_Buffer[1] = LED_CHAR_H;
									gu8v_LED_Buffer[2] = LED_CHAR_OFF;
									gu8v_LED_Buffer[3] = LED_CHAR_OFF;
								}else{
									fg_led_unit_pct = 1;
									fg_led_unit_kg = 0;
									fg_led_unit_lb = 0;
									fun_HEX2BCD(gu16_BodyFatRate);
								}
	                        }else{
	                            fun_HEX2BCD(gu16_display_weight);
	                            fg_led_unit_pct = 0;
								if(UNIT_KG == gu8_weigh_targeunit){
									fg_led_unit_kg = 1;
									fg_led_unit_lb = 0;
								}else if(UNIT_LB == gu8_weigh_targeunit){
									fg_led_unit_kg = 0;
									fg_led_unit_lb = 1;
								}
	                        }

	                        if(0 == get_ledflash_status()){
	                            set_BHSDKState(ENTER_WEIGHT_NORMAL);
	                            fg_bodyfatrate_rec_ok = 0;
								fg_led_unit_pct = 0;
	                        }
	                    }else{
	                    	 fg_led_unit_pct = 0;
							 if(UNIT_KG == gu8_weigh_targeunit){
								 fg_led_unit_kg = 1;
								 fg_led_unit_lb = 0;
							 }else if(UNIT_LB == gu8_weigh_targeunit){
								 fg_led_unit_kg = 0;
								 fg_led_unit_lb = 1;
							 }
	                         fun_HEX2BCD(gu16_display_weight);
	                    }
					}else{
						//蓝牙没有连接,直接转到体重状态.
						if(0 == get_ledflash_status()){
							set_BHSDKState(ENTER_WEIGHT_NORMAL);
							set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_OFF,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
						}
					}
					break;
			}
			break;

        case DISPLAY_UNLOCK_WEIGHT:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_UNLOCK_WEIGHT,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
            break;

		case DISPLAY_CAL0:
			fun_HEX2BCD(gu16_display_weight);
			set_ledflash(DISPLAY_CAL0,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);
			break;

//		case DISPLAY_CAL1:
//			fun_HEX2BCD(gu16_display_weight);
//			set_ledflash(DISPLAY_CAL1,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
//			break;

//		case DISPLAY_CAL2:
//			fun_HEX2BCD(gu16_display_weight);
//			set_ledflash(DISPLAY_CAL2,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
//			break;

//		case DISPLAY_CAL3:
//			fun_HEX2BCD(gu16_display_weight);
//			set_ledflash(DISPLAY_CAL3,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
//			break;

		case DISPLAY_CALPASS:
			while(1){
				flag_led_Byte &= 0x00;//unit:ble,pct,kg,lb.
				gu8v_LED_Buffer[NUM_QIAN] = LED_CHAR_P;//'P'
				gu8v_LED_Buffer[NUM_BAI]= LED_CHAR_A;  //'A'
				gu8v_LED_Buffer[NUM_SHI]= LED_CHAR_5;  //'S'
				gu8v_LED_Buffer[NUM_GE] = LED_CHAR_5;  //'S'
				GCC_CLRWDT();
			}
			break;

		case DISPLAY_CALFAIL:
			while(1){
				flag_led_Byte &= 0x00;//unit:ble,pct,kg,lb.
				gu8v_LED_Buffer[NUM_QIAN] = LED_CHAR_F;//'F'
				gu8v_LED_Buffer[NUM_BAI]= LED_CHAR_I;  //'I'
				gu8v_LED_Buffer[NUM_SHI]= LED_CHAR_A;  //'A'
				gu8v_LED_Buffer[NUM_GE] = LED_CHAR_5;  //'S'
				GCC_CLRWDT();
			}
			break;

		default:
		case DISPLAY_ALLOFF:
			Set_AllLEDBuffer(0);
			set_ledflash(DISPLAY_ALLOFF,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			break;
	}

	fun_bluetooth_detected();

}

/********************************************************************
Function: 检测蓝牙连接.
INPUT	:
OUTPUT	:
NOTE	:检测app是否连接上蓝牙.
********************************************************************/
void fun_bluetooth_detected(void)
{
	if((DISPLAY_POWERON != gu8_dismode) && (DISPLAY_ALLOFF != gu8_dismode)){
		if(fg_time_100ms){
			fg_time_100ms = 0;
			//连接蓝牙图标显示
			if(!P_BT_Status && !P_BLE_EN){
				if(C_BLE_COUNT <= gu8_ble_count++){
					gu8_ble_count = 0;
					fg_led_ble = 1;
				}
			}else{
				gu8_ble_count = 0;
				fg_led_ble = 0;
			}
		}
	}
}

//===============================================================
//function: 定时关机.
//input   : overtime:overtime*100Ms
//output  : NULL
//description: timing: 100MS*overtime.
//===============================================================
void set_overtime2poweroff(u8 overtime)
{
	gu8v_timed_shutdown = overtime;
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
		gu8_worktasks = TASK_SLEEP;
	}
}

