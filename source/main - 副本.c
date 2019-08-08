#include "common.h"

#if _UART_DEBUG == ENABLE
extern void UART_SendData(u8* pdata, u8 len);
extern volatile unsigned long haltWeightADCData;
#endif

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
				//fun_UserProtocol();
				break;

			case TASK_SLEEP:
				{
					 gu8v_UartTxBuf[0] = 0xff;
					 gu8v_UartTxBuf[1] = ((SDKADCFilterData.Current) & 0xFF00 )>>8;
					 gu8v_UartTxBuf[2] = (SDKADCFilterData.Current) & 0x00FF;
					 gu8v_UartTxBuf[3] = 0xff;
					 gu8v_UartTxBuf[4] = ((SDKWeight.CalADCData.Cal0) & 0xFF00 )>>8;//;
					 gu8v_UartTxBuf[5] = (SDKWeight.CalADCData.Cal0) & 0x00FF;
					 gu8v_UartTxBuf[6] = 0xff;//0x00;
					 gu8v_UartTxBuf[7] = SDKWeight.Span;//0x00;
					 gu8v_UartTxBuf[8] = 0xff;//0x00;
					 gu8v_UartTxBuf[9] = ((haltWeightADCData) & 0xFF00 )>>8;
					 gu8v_UartTxBuf[10] = (haltWeightADCData) & 0x00FF;
					SET_UART_ENABLE();
					 UART_SendData(&gu8v_UartTxBuf[0],UART_LENGTH_RX);//test
				 }
				task_scales2sleep();
				break;

			case TASK_WEIGHT_AUTOON:
				task_scaleswakeup();
				fg_time_test2 = 1;
				break;

			default:
				gu8_worktasks = TASK_SCALES;
				break;
		}
		/* LED显示内容buffer填充 */
		fun_diaplay_mode();
   #if (_UART_DEBUG == ENABLE)
			 {
				 if(fg_time_test){
					 fg_time_test = 0;
					 gu8v_UartTxBuf[0] = 0xaa;
					 gu8v_UartTxBuf[1] = ((SDKADCFilterData.Current) & 0xFF00 )>>8;
					 gu8v_UartTxBuf[2] = (SDKADCFilterData.Current) & 0x00FF;
					 gu8v_UartTxBuf[3] = 0xaa;
					 gu8v_UartTxBuf[4] = ((SDKWeight.CalADCData.Cal0) & 0xFF00 )>>8;//;
					 gu8v_UartTxBuf[5] = (SDKWeight.CalADCData.Cal0) & 0x00FF;
					 gu8v_UartTxBuf[6] = 0xaa;//0x00;
					 gu8v_UartTxBuf[7] = SDKWeight.Span;//0x00;
					 gu8v_UartTxBuf[8] = 0xaa;//0x00;
					 gu8v_UartTxBuf[9] = ((haltWeightADCData) & 0xFF00 )>>8;
					 gu8v_UartTxBuf[10] = (haltWeightADCData) & 0x00FF;
				}
				 if(fg_time_test2){
				 	fg_time_test2 = 0;
					SET_UART_ENABLE();
					 UART_SendData(&gu8v_UartTxBuf[0],UART_LENGTH_RX);//test
				 }
			}
   #endif

		/* 时间定时每次进入为100MS */
		//fun_timing();

		/* 判断定时关机 */
		is_timedshutdown();

		/* For Debug */
	}
}
