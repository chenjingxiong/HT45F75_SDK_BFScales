#include "common.h"
u16 gu16v_weigh;
u16 gu16_display_weight;
u16 gu16v_impedence_data;
u16 gu16v_pct_data;

u8 gu8v_weigh_targeunit;
volatile u8  gu8v_05s_count;//led��˸ʱ����֮���ʱ�����ļ���ֵ.
volatile u8  gu8v_howtimes;//led��˸���ٴΣ�����һ��һ����2��.
volatile u8  gu8v_led_speed;//led��˸ʱ����֮���ʱ����,
volatile u8  gu8v_led_delay;//�ӳ�x*100MS��ʼִ����˸����.
volatile u8  gu8v_led_delay3S;

u8  gu8v_timed_shutdown;
u8  gu8v_dismode;
u8  gu8v_worktasks;

//����:0.1��;	eg:weigh  =100,���� 100*0.1��=10��.
void fun_Unit_Change(u16 weigh)
{

	if(UNIT_KG == gu8v_weigh_targeunit)
	{
		fg_led_unit_kg = 1;
		fg_led_unit_lb = 0;

		#if 1//(_UART_DEBUG == ENABLE)
			gu16_display_weight = (u32)weigh/2;///10; //KG�Q�� ����ǿ��ת������ֹ���
		#else
			gu16v_Display_Weigh = weigh;
		#endif
	}
	else if(UNIT_LB == gu8v_weigh_targeunit)
	{
		fg_led_unit_kg = 0;
		fg_led_unit_lb = 1;
		gu16_display_weight = ((u32)weigh/2*(u32)22046/1000+5)/10 & 0xfffe; //LB�Q��
		/*˵��:
			10(g) = 0.0220462(lb),0.1(lb) = 45.359237(g)

			 eg:
			 	��:weigh = 55
			 	��:weigh=55=550(g) -->��Ϊ�˺�̩�⾫��Ϊ10g����1����1*10=10(g);x*10=10x(g)
			 	����:weigh*10000/45359=55*10000/45359
			 		ȡ����λ����12������1.2(lb)
		*/
	}
//	else if(UNIT_JIN == gu8v_weigh_targeunit)
//	{
//		fg_led_unit_kg = 0;
//		fg_led_unit_lb = 0;
//		gu16_display_weight = (u32)weigh;///5; 	//JIN  0.1��  = 5(weigh) ,gu16v_Display_Weigh�ĸ�λ����С�����һλ������ һλС���c
//		/*˵��:
//			eg:
//			   ��:weigh = 44
//			   ��:weigh=44=440(g) -->��Ϊ�˺�̩�⾫��Ϊ10g����1����1*10=10(g);x*10=10x(g)
//			   ����:weigh/5 = 44/5 = 8.8
//					ȡ����λ����8������0.8(��)
//		*/
//	}
}

//===============================================================
//function: ����LED��˸.
//input   :
//			mode: LED��ʾ��ģʽ
//			ledflash: =0 �ر���˸; =1 ������˸
//			count: ��˸����: =0 ����˸, =x ��˸x��.
//			speed: ��˸�Ŀ���,ȡֵ��Χ0~255,ʱ�����x100MS.
//			delay: �ӳ� delay*100MS ʱ���ſ�ʼִ����˸����
//			overtime:���ö�ʱ����ʱ��,ʱ��= 100ms*(overtime+delay)
//output  : NULL
//description: ��������ǰ�����ã��ͻ��Ѻ�ĳ�ʼ��.
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
//function: ���ó�ʱ�ػ�ʱ��.
//input   : overtime:��λΪ100ms;	eg:overtime=10,�����1s,��10*100ms=1S.
//output  : NULL
//description: ʱ��: 100MS*overtime.
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
	// ����gu8v_BodyfatSDKState ���UI������������
	switch (BHSDKState)
	{
		case STATE_WEIGHT_PREPARE:	// �Q�؜ʂ���
			// �ГQ��ENTER_WEIGHT_NORMAL��,ͨ����300ms���ҷQ�؜ʂ�r�g
			break;
		case STATE_WEIGHT_NOLOAD:	// ���d/��ǰ����С���С�Q������
			//fun_DisplayMode_NoLoad();//��̎��User UI,�����@ʾ�R�r����SDKWeight.DataCurrent
            if(is_BHSDKState_change()){
                fun_Unit_Change(SDKWeight.DataCurrent);
                Set_DisplayMode(DISPLAY_NOLOAD);
            }
			break;
		case STATE_WEIGHT_LOADUP:	// ���ϷQ����/�i����B�¼��d���i����,������0�c׃��>��С�Q������
			//fun_DisplayMode_LoadUp();//��̎��User UI,�����@ʾ�R�r����SDKWeight.DataCurrent
			fun_Unit_Change(SDKWeight.DataCurrent);
			Set_DisplayMode(DISPLAY_LOADUP);
			gbv_TxSDKWeightStatus = 1;
			break;
		case STATE_WEIGHT_LOADOK:	// ���һ�ηQ�؜y��
	        // fun_DisplayMode_LoadOk();��̎��User UI,�����_ʼ�W�q��������SDKWeight.DataState��
			if(C_UNLOCK_WEIGHT >= SDKWeight.DataStable){
				fun_Unit_Change(SDKWeight.DataCurrent);
				Set_DisplayMode(DISPLAY_UNLOCK_WEIGHT);
			}else{
				if(is_BHSDKState_change()){
					fg_loadok = 1;
//					fun_Unit_Change(SDKWeight.DataStable);
					Set_DisplayMode(DISPLAY_LOADFIX);
				}
			}
			break;
		case STATE_WEIGHT_LOADFIX:	// ���һ�ηQ�؜y�������������]�н��i
			//fun_DisplayMode_LoadFix();//��̎��User UI,,�����_ʼ�W�q��������SDKWeight.DataState��
			if(C_UNLOCK_WEIGHT >= SDKWeight.DataStable){
				if(SDKWeight.DataCurrent != SDKWeight.DataStable){
					set_overtime2poweroff(C_TIME_10S);//如果重量有变化,更新时间不去睡眠.
				}
				fun_Unit_Change(SDKWeight.DataCurrent);
				Set_DisplayMode(DISPLAY_UNLOCK_WEIGHT);
			}else{
				gu8v_time_30s = C_TIME_30S;
				if(is_BHSDKState_change()){
					gbv_TxSDKWeightStatus = 1;
					if(!fg_remeber_200g){
						fg_remeber_200g = 1;
						gu16v_weigh = SDKWeight.DataStable;
//						fun_Unit_Change(gu16v_weigh);
					}else{
						if(fun_unsigned32BitABS(gu16v_weigh,SDKWeight.DataStable) <= C_REMEBER_200G){
//							fun_Unit_Change(gu16v_weigh);
							GCC_NOP();
						}else{
							gu16v_weigh = SDKWeight.DataStable;
//							fun_Unit_Change(gu16v_weigh);
						}
					}
					fun_Unit_Change(gu16v_weigh);
					Set_DisplayMode(DISPLAY_LOADFIX);
				}
			}

			break;
		case STATE_WEIGHT_LOADDOWN:	// �³ӄ���
	        //fun_DisplayMode_LoadDown();//��̎��User UI,,�����@ʾ�i��SDKWeight.DataState��
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
		case STATE_WEIGHT_OVERLOAD:	// ����,��ǰ����������Q������
			//fun_DisplayMode_OverLoad();//��̎��User UI,,�����@ʾ-OL-��
			Set_DisplayMode(DISPLAY_OVERLOAD);
			break;
		case STATE_IMPEDANCE_REFERENCE1:// �������y�������1
		case STATE_IMPEDANCE_REFERENCE2:// �������y�������2
		case STATE_IMPEDANCE_CHECKBODY: // �Д��Ƿ������w
		case STATE_IMPEDANCE_RX:		// �������y���y�迹
			//fun_DisplayMode_Impedanceing();// ��̎��User UI,,������----/oooo��ʾ�迹�y����
			Set_DisplayMode(DISPLAY_IMPEDANCEING);
			break;
		case STATE_IMPEDANCE_FINISH:	// �迹���y�Y��,�˕r�����xȡgu16v_CurrentImpedance_ohm
			//fun_DisplayMode_ImpedanceFinish();//
//			BHSDKState = ENTER_IMPEDANCE;
			gu16v_impedence_data = SDKImpedance.Data;
			Set_DisplayMode(DISPLAY_IMPEDANCE_FINISH);
		    break;
		case STATE_AUTOON_FASTMODE:		// ����ADC�Ԅ��ϷQ�Д�
		case STATE_AUTOON_SLOWMODE:		// ����ADC�Ԅ��ϷQ�Д�
			// ͨ���@���r��o�κ�UI
			break;
		case STATE_AUTOON_PASS:			// �Ԅ��ϷQOK
			// �˕r�����c���@ʾ,���ГQ����ͨ�Q��ģʽ�M��MCU����������
			gu8v_worktasks = TASK_WEIGHT_AUTOON;
			break;
		case STATE_AUTOON_FAIL:			// �Ԅ��ϳ�ʧ��,�˕rՈ�^�m׌ MCU �M��HALT��B
//			fun_BodyFatScalesSDK_PowerDown();
//			GCC_HALT();
			gu8v_worktasks = TASK_SLEEP;
			break;
		case STATE_WEIGHT_CAL0:	// ���ژ˶����c
			// fun_DisplayMode_CAL0();��̎��User UI,,������ʾCAL 0kg
//			fun_Unit_Change(0);
//			Set_DisplayMode(DISPLAY_CAL);
			break;
		case STATE_WEIGHT_CAL1:	// ���ژ˶���1�c
			// fun_DisplayMode_CAL1();��̎��User UI,,������ʾCAL 50kg
//			fun_Unit_Change(1000);
//			Set_DisplayMode(DISPLAY_CAL);
			break;
		case STATE_WEIGHT_CAL2:	// ���ژ˶���2�c
			// fun_DisplayMode_CAL2();��̎��User UI,,������ʾCAL 100kg

//			if(is_BHSDKState_change()){
//				fun_Unit_Change(2000);
//				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CAL3:	// ���ژ˶���3�c
			// fun_DisplayMode_CAL3();��̎��User UI,,������ʾCAL 150kg

//			if(is_BHSDKState_change()){
//				fun_Unit_Change(3000);
//				Set_DisplayMode(DISPLAY_CAL);
//			}
			break;
		case STATE_WEIGHT_CALPASS:	// �˶��ɹ�
			//fun_DisplayMode_CALPASS();//��̎��User UI,,������ʾPASS
			Set_DisplayMode(DISPLAY_CALPASS);
			break;
		case STATE_WEIGHT_CALFAIL:	// �˶�ʧ��
			// fun_DisplayMode_CALFAIL();��̎��User UI,,������ʾFIAS
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
//function: ������ʾģʽ.
//input   : DISPLAY_POWERON ~ DISPLAY_XXX.
//output  : NULL
//description:.
//===============================================================
void Set_DisplayMode(u8 dismode)
{
	gu8v_dismode = dismode;
}

//===============================================================
//function: ��ȡ��ʾģʽ.
//input   : NULL.
//output  : DISPLAY_POWERON ~ DISPLAY_XXX.
//description:.
//===============================================================
//u8 Get_DisplayMode(void)
//{
//	return gu8v_dismode;
//}


//===============================================================
//function: LED ��ʾģʽ����.
//input   : NULL.
//output  : NULL
//description: �������õ���ʾģʽ��������Ҫ��ʾ�����ݵ�.
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
			set_overtime2poweroff(C_TIME_10S);//不稳定时不关机.
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
			while(1){
				is_timedshutdown();
			}
			break;

		case DISPLAY_IMPEDANCEING:
//			set_ledflash(DISPLAY_IMPEDANCEING,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			//������֬���ȶ�ʱ�ᳬ��10s�У����Դ�ʱ��Ҫ���¶�ʱ���ߵĶ�ʱʱ��.

			gu8v_LED_Buffer[0] = LED_CHAR_OFF;
			gu8v_LED_Buffer[1] = LED_CHAR__;
			gu8v_LED_Buffer[2] = LED_CHAR__;
			gu8v_LED_Buffer[3] = LED_CHAR_OFF;
			fg_led_Byte &= 0x40;
			set_ledflash(DISPLAY_IMPEDANCEING,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
			set_overtime2poweroff(C_TIME_10S);
			break;

		case DISPLAY_IMPEDANCE_FINISH:

			switch(gu16v_impedence_data){

				case IMPEDANCE_ERR_NOTEST:
				case IMPEDANCE_ERR_ABNORMAL:
				case IMPEDANCE_ERR_OUTRANGE:
                    gu16v_impedence_data = 0x00;

                    if(0 == fg_led_timing){
                        set_BHSDKState(ENTER_WEIGHT_NORMAL);//test
                        set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_OFF,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
                        fg_pct_ok = 0;
                    }

					break;

				default:
					gbv_TxSDKImpedanceStatus = 1;
                    if(fg_pct_ok){
                        set_ledflash(DISPLAY_IMPEDANCE_FINISH,C_LED_FLASH_ON,C_LED_FLASH_IMP,C_TIME_05S,0,C_TIME_10S);
                        if(fg_led_change)
                        {
                            //fun_HEX2BCD(gu16v_impedence_data);//test
                            fun_HEX2BCD(gu16v_pct_data);//test
                            //fg_led_piont1 = 0;
                            //fg_led_piont2 = 0;
                            fg_led_unit_pct = 1;
                        }else{
                            fun_HEX2BCD(gu16_display_weight);
                            fg_led_unit_pct = 0;
                        }

                        if(0 == fg_led_timing){
                            set_BHSDKState(ENTER_WEIGHT_NORMAL);
                            fg_pct_ok = 0;
                        }
                    }else{
                         fun_HEX2BCD(gu16_display_weight);
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
		//蓝牙图标显示
		if(!P_BT_Status & !P_BLE_EN)
			fg_led_ble = 1;
		else
			fg_led_ble = 0;
	}
}

//===============================================================
//function: �Ƿ�ִ������.
//input   : NULL.
//output  : NULL
//description:��ʱʱ�䵽��ִ������.
//===============================================================
void is_timedshutdown(void)
{
	if(fg_time_10s){
		fg_time_10s = 0;
		gu8v_worktasks = TASK_SLEEP;
        set_BHSDKState(ENTER_WEIGHT_NORMAL);
	}
}

void task_scales2sleep(void)
{

#if 1
    P_BLE_EN = HIGH;
	_t0on  = 0;
	Set_DisplayMode(DISPLAY_ALLOFF);
	LED_Init();
	SETWDTTIME_1000MS();
	SET_UART_DISABLE();
	SETLVD_DISABLE();
	if(STATE_WEIGHT_NOLOAD == BHSDKState || STATE_WEIGHT_LOADDOWN == BHSDKState\
	   ||STATE_AUTOON_FAIL == BHSDKState){
		_idlen = 0;
//		_emi = 0;
		if(gu8v_time_30s)
			gu8v_time_30s--;
		else
			fg_remeber_200g = 0;

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
    BHSDKState = ENTER_WEIGHT_NORMAL;
#endif
}

#if 0
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
#endif


