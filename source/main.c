#include "common.h"

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

		user_init();

		//显示
		// 上電后調用fun_BodyFatScalesSDK_PowerOn()函數
		fun_BodyFatScalesSDK_PowerOn();
		// 切換為阻抗模式
/*		BHSDKState = ENTER_IMPEDANCE*/;
		// 切換為稱重模式

		//gu8v_worktasks = TASK_SCALES;
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
        _t0on = 0;
        _emi = 0;
	}


	//主循環
	while(1)
	{
		GCC_CLRWDT();

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
				//task_scaleswakeup();//鐪佺┖闂达紝涓嶇敤鍑芥暟鐩存帴鐮佷笂
				gu8v_worktasks = TASK_SCALES;
				BHSDKState = ENTER_WEIGHT_NORMAL;
                P_BLE_EN = LOW;
				SET_UART_ENABLE();
				// TM0
				_tm0c0 = 0x20;		// fsys/16 4us
				_tm0c1 = 0xc1;		// TimeCnt Mode
				_tm0al = 500%256;	// 500*4us = 2ms;
				_tm0ah = 500/256;
				SETCTMA_ISR_ENABLE();
				_t0on  = 1;
				_emi = 1;
				break;

			default:
				gu8v_worktasks = TASK_SCALES;
				break;
		}

		fun_DiaplsyMode();

		is_timedshutdown();

		if(fg_time_1s){
			fg_time_1s = 0;
		}
	}
}
