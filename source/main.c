#include "common.h"

//volatile unsigned char Version[3];
u8 test;
int main(void)
{
	// �Д��Ƿ����늏�λ���߷�������r�µď�λ
	// �������늏�λ��������늏�λ��ʼ������֮����WDT�����ʼ��
	if (_to == 0 || _pdf ==0)
	{

		//��ʼ��//
		fun_PowerOnSysInit();
		//UART����
		fun_UARTPowerOnInit();
		//��ʾ
		// ��늺��{��fun_BodyFatScalesSDK_PowerOn()����
		fun_BodyFatScalesSDK_PowerOn();
		// �ГQ���迹ģʽ
/*		BHSDKState = ENTER_IMPEDANCE*/;
		// �ГQ��Q��ģʽ
		BHSDKState = ENTER_WEIGHT_NORMAL;
		 SDKWeight.flag.b.IsNeedTare = 1;			// �������Ĭ�J��0kg
		// �ГQ��˶�ģʽ
	/*	 BHSDKState = ENTER_WEIGHT_CAL;	*/
	}
	else
	{
		// WDT�����λ��ʼ��
		GCC_CLRWDT();
		_lvf = 0;
		gu8v_worktasks = TASK_SCALES;
		set_overtime2poweroff(C_TIME_10S);
		fg_time_10s = 0;
	}

	//��ѭ�h
	while(1)
	{
		GCC_CLRWDT();
//		Version[0] = SDKVersion[0];
//		Version[1] = SDKVersion[1];
//		Version[2] = SDKVersion[2];
		// ��LOOPѭ�h�{�� fun_BodyFatScalesSDK()
		fun_BodyFatScalesSDK();
		fun_UserProtocol();
		test = BHSDKState;

		switch(gu8v_worktasks)
		{
			case TASK_STARTUP:
				Set_DisplayMode(DISPLAY_POWERON);
				break;

			case TASK_LOWBATTERY:
				Set_DisplayMode(DISPLAY_LOWBATTERY);
				break;

			case TASK_SCALES:
				task_bodyfatscales();
				break;

			case TASK_SLEEP:
				task_scales2sleep();
				break;

			case TASK_WEIGHT_AUTOON:
				task_scaleswakeup();
				break;

			default:
				gu8v_worktasks = TASK_SCALES;
				break;
		}

		fun_DiaplsyMode();

		is_timedshutdown();

		fun_ble_task();

		if(fg_time_1s){
			fg_time_1s = 0;
		}
	}
}
