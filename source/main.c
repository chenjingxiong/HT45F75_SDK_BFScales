#include "common.h"

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

		/* 用户初始化变量 */
		user_init();

		//显示
		//上電后調用fun_BodyFatScalesSDK_PowerOn()函數
		fun_BodyFatScalesSDK_PowerOn();
		// 切換為阻抗模式
		/*BHSDKState = ENTER_IMPEDANCE;*/
		// 切換為稱重模式
		BHSDKState = ENTER_WEIGHT_NORMAL;
		SDKWeight.flag.b.IsNeedTare = 1; //上電重量默認為0kg
		// 切換為標定模式
		/*BHSDKState = ENTER_WEIGHT_CAL;	*/
	}
	else
	{
		// WDT溢出復位初始化
		GCC_CLRWDT();
		/* 睡眠唤醒后进入TASK_SCALES模式检测上称状态 */
		gu8_worktasks = TASK_SCALES;
	}


	//主循環
	while(1)
	{
		GCC_CLRWDT();

		// 主LOOP循環調用 fun_BodyFatScalesSDK()
		fun_BodyFatScalesSDK();

		switch(gu8_worktasks)
		{
			case TASK_STARTUP:
				task_scalesstartup();
				break;

			case TASK_LOWBATTERY:
				task_scaleslowbattry();
				break;

			case TASK_SCALES:
				task_bodyfatscales();
				fun_UserProtocol();
				break;

			case TASK_SLEEP:
				task_scales2sleep();
				break;

			case TASK_WEIGHT_AUTOON:
				task_scaleswakeup();
				break;

			default:
				gu8_worktasks = TASK_SCALES;
				break;
		}
		/* LED显示内容buffer填充 */
		fun_diaplay_mode();

		/* 时间定时每次进入为100MS */
		//fun_timing();

		/* 判断定时关机 */
		is_timedshutdown();

		/* For Debug */
	}
}
