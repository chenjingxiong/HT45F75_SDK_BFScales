#include "common.h"

#if _UART_DEBUG == ENABLE
extern void UART_SendData(u8* pdata, u8 len);
extern volatile unsigned long haltWeightADCData;
#endif

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

		/* �û���ʼ������ */
		user_init();

		//��ʾ
		//��늺��{��fun_BodyFatScalesSDK_PowerOn()����
		fun_BodyFatScalesSDK_PowerOn();
		// �ГQ���迹ģʽ
		/*BHSDKState = ENTER_IMPEDANCE;*/
		// �ГQ��Q��ģʽ
		BHSDKState = ENTER_WEIGHT_NORMAL;
		SDKWeight.flag.b.IsNeedTare = 1; //�������Ĭ�J��0kg
		// �ГQ��˶�ģʽ
		/*BHSDKState = ENTER_WEIGHT_CAL;	*/
	}
	else
	{
		// WDT�����λ��ʼ��
		GCC_CLRWDT();
		/* ˯�߻��Ѻ����TASK_SCALESģʽ����ϳ�״̬ */
		gu8_worktasks = TASK_SCALES;
	}


	//��ѭ�h
	while(1)
	{
		GCC_CLRWDT();

		// ��LOOPѭ�h�{�� fun_BodyFatScalesSDK()
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
		/* LED��ʾ����buffer��� */
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

		/* ʱ�䶨ʱÿ�ν���Ϊ100MS */
		//fun_timing();

		/* �ж϶�ʱ�ػ� */
		is_timedshutdown();

		/* For Debug */
	}
}
