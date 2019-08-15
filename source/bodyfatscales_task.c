#include "common.h"

u8  gu8_worktasks;//��֬�ƹ���ģʽ.
u16 gu16_weigh;    //��ʾ����ǰ��ԭʼ����,����Ϊ0.1jin���������ݡ�
u16 gu16_rememberweigh;//200g�������أ�����Ϊ0.1jin���������ݡ�
u16 gu16_currentweigh;
u16 gu16_impedence_data;//ԭʼ�迹����.
u16 gu16_BodyFatRate;//������������֬��.

/********************************************************************
Function: ���óӹ���״̬.
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
Function: ���� BHSDKState״̬���UI������������.
INPUT	:
OUTPUT	:
NOTE	: �����ڹ���������״̬BHSDKState.
********************************************************************/
void task_bodyfatscales(void)
{
	//����BHSDKState���UI������������
	switch (BHSDKState)
	{
		case STATE_WEIGHT_PREPARE:	// �Q�؜ʂ���
			// �ГQ��ENTER_WEIGHT_NORMAL��,ͨ����300ms���ҷQ�؜ʂ�r�g
			break;

		case STATE_WEIGHT_NOLOAD:// ���d/��ǰ����С���С�Q������
			// fun_DisplayMode_NoLoad();��̎��User UI,�����@ʾ�R�r����SDKWeight.DataCurrent
            if(is_BHSDKState_change()){
                fun_Unit_Change(SDKWeight.DataCurrent);
                Set_DisplayMode(DISPLAY_NOLOAD);

				if(fg_manual_readycal){
					fg_manual_readycal = 0;
					set_BHSDKState(ENTER_WEIGHT_CAL);
				}
            }
			break;

		case STATE_WEIGHT_LOADUP:	// ���ϷQ����/�i����B�¼��d���i����,������0�c׃��>��С�Q������
		// fun_DisplayMode_LoadUp();��̎��User UI,�����@ʾ�R�r����SDKWeight.DataCurrent
		fun_Unit_Change(SDKWeight.DataCurrent);
		Set_DisplayMode(DISPLAY_LOADUP);
		gbv_TxSDKWeightStatus = 1;
		gu8_data_type = DATA_TYPE_ING;
		break;

		case STATE_WEIGHT_LOADOK:	// ���һ�ηQ�؜y��
            // fun_DisplayMode_LoadOk();��̎��User UI,�����_ʼ�W�q��������SDKWeight.DataState��
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
		case STATE_WEIGHT_LOADFIX:	// ���һ�ηQ�؜y�������������]�н��i
			// fun_DisplayMode_LoadFix();��̎��User UI,,�����_ʼ�W�q��������SDKWeight.DataState��
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
					set_overtime2poweroff(C_TIME_10S);//��������б仯����ʱ�䲻ȥ˯��.
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
		case STATE_WEIGHT_LOADDOWN:	// �³ӄ���
            // fun_DisplayMode_LoadDown();��̎��User UI,,�����@ʾ�i��SDKWeight.DataState��
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
		case STATE_WEIGHT_OVERLOAD:	// ����,��ǰ����������Q������
			// fun_DisplayMode_OverLoad();��̎��User UI,,�����@ʾ-OL-��
			fun_Unit_Change(SDKWeight.DataCurrent);
			Set_DisplayMode(DISPLAY_OVERLOAD);
			gbv_TxSDKWeightStatus = 1;
			gu8_data_type = DATA_TYPE_OVERLOAD;
			break;
		case STATE_IMPEDANCE_REFERENCE1:// �������y�������1
		case STATE_IMPEDANCE_REFERENCE2:// �������y�������2
		case STATE_IMPEDANCE_CHECKBODY: // �Д��Ƿ������w
		case STATE_IMPEDANCE_RX:		// �������y���y�迹
			// fun_DisplayMode_Impedanceing();��̎��User UI,,������----/oooo��ʾ�迹�y����
			Set_DisplayMode(DISPLAY_IMPEDANCEING);
            gu16_impedence_data= SDKImpedance.Data;
			fg_bodyfatrate_rec_ok = 0;
			break;
		case STATE_IMPEDANCE_FINISH:	// �迹���y�Y��,�˕r�����xȡgu16v_CurrentImpedance_ohm
			// fun_DisplayMode_ImpedanceFinish();
			//BHSDKState = ENTER_IMPEDANCE;

			gu8_data_type = DATA_TYPE_LOCK;
			gu16_impedence_data = SDKImpedance.Data;
			Set_DisplayMode(DISPLAY_IMPEDANCE_FINISH);
		    break;
		case STATE_AUTOON_FASTMODE:		// ����ADC�Ԅ��ϷQ�Д�
		case STATE_AUTOON_SLOWMODE:		// ����ADC�Ԅ��ϷQ�Д�
			// ͨ���@���r��o�κ�UI
			break;
		case STATE_AUTOON_PASS: 		// �Ԅ��ϷQOK
			// �˕r�����c���@ʾ,���ГQ����ͨ�Q��ģʽ�M��MCU����������
			gu8_worktasks = TASK_WEIGHT_AUTOON;
			break;
		case STATE_AUTOON_FAIL: 		// �Ԅ��ϳ�ʧ��,�˕rՈ�^�m׌ MCU �M��HALT��B
			//fun_BodyFatScalesSDK_PowerDown();
			//GCC_HALT();
			gu8_worktasks = TASK_SLEEP;
			break;
		case STATE_WEIGHT_CAL0: // ���ژ˶����c
			// fun_DisplayMode_CAL0();��̎��User UI,,������ʾCAL 0kg
			fun_Unit_Change(0);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CAL1:	// ���ژ˶���1�c
			// fun_DisplayMode_CAL1();��̎��User UI,,������ʾCAL 50kg
			fun_Unit_Change(1000);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CAL2:	// ���ژ˶���2�c
			// fun_DisplayMode_CAL2();��̎��User UI,,������ʾCAL 100kg
			fun_Unit_Change(2000);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CAL3:	// ���ژ˶���3�c
			// fun_DisplayMode_CAL3();��̎��User UI,,������ʾCAL 150kg
			fun_Unit_Change(3000);
			Set_DisplayMode(DISPLAY_CAL0);
			break;
		case STATE_WEIGHT_CALPASS:	// �˶��ɹ�
			// fun_DisplayMode_CALPASS();��̎��User UI,,������ʾPASS
			Set_DisplayMode(DISPLAY_CALPASS);
			break;
		case STATE_WEIGHT_CALFAIL:	// �˶�ʧ��
			// fun_DisplayMode_CALFAIL();��̎��User UI,,������ʾFIAS
			Set_DisplayMode(DISPLAY_CALFAIL);
			break;
		default:
			break;
	}

	is_BHSDKState_change();
}

/********************************************************************
Function: �ϵ翪���Ž�������ģʽ.
INPUT	:
OUTPUT	:
NOTE	:LEDȫ��3S,3S��ֱ���ϵ��ӹ�����ɲ��л�����������ģʽ.
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
Function: �͵�ģʽ.
INPUT	:
OUTPUT	:
NOTE	:���ڽ��յ������͵�����,�ͽ���͵�ģʽ,��ʾ3S��ػ�.
********************************************************************/
void task_scaleslowbattry(void)
{
	Set_DisplayMode(DISPLAY_LOWBATTERY);
}

/********************************************************************
Function: ˯��ģʽ.
INPUT	:
OUTPUT	:
NOTE	:˯�߳�ʼ�������򿪿��Ź�ÿ1S���Ѽ���Ƿ��ϳƳɹ����ϳ�ʧ�������˯��.
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
Function: ����ģʽ.
INPUT	:
OUTPUT	:
NOTE	:˯�߻����ϳƳɹ���,���뻽��ģʽ����һЩ����,�ٽ�����������ģʽ.
********************************************************************/
void task_scaleswakeup(void)
{
	gu8_worktasks = TASK_SCALES;
	BHSDKState = ENTER_WEIGHT_NORMAL;

	P_BLE_EN = LOW;//������.
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

