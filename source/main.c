#include "common.h"

//volatile unsigned char Version[3];
u8 test;
int main(void)
{
	// 判斷是否為上電復位或者非正常情況下的復位
	// 如果是上電復位，執行上電復位初始化，反之執行WDT溢出初始化
	if (_to == 0 || _pdf ==0)
	{

		//初始化//
		fun_PowerOnSysInit();
		//UART配置
		fun_UARTPowerOnInit();
		//显示
		// 上電后調用fun_BodyFatScalesSDK_PowerOn()函數
		fun_BodyFatScalesSDK_PowerOn();
		// 切換為阻抗模式
/*		BHSDKState = ENTER_IMPEDANCE*/;
		// 切換為稱重模式
		BHSDKState = ENTER_WEIGHT_NORMAL;
		 SDKWeight.flag.b.IsNeedTare = 1;			// 上電重量默認為0kg
		// 切換為標定模式
	/*	 BHSDKState = ENTER_WEIGHT_CAL;	*/
	}
	else
	{
		// WDT溢出復位初始化
		GCC_CLRWDT();
		_lvf = 0;
		gu8v_worktasks = TASK_SCALES;
		set_overtime2poweroff(C_TIME_10S);
		fg_time_10s = 0;
	}

	//主循環
	while(1)
	{
		GCC_CLRWDT();
//		Version[0] = SDKVersion[0];
//		Version[1] = SDKVersion[1];
//		Version[2] = SDKVersion[2];
		// 主LOOP循環調用 fun_BodyFatScalesSDK()
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
