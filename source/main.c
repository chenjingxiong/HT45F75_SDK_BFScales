#include "common.h"

//*******************************************************************************************
//																							*
//                    ProductName:	HT45F75_48LQFP_����������֬��_NW1973               	        *
//                    MCU Body:		HT45F75-48LQFP 			               	       		    *
//                    HT-IDE3000: 	V7.99	                                           		*				*
//                    Date:         2019/07/17                                    	   		*				*
//                    Author:       ghg                                      				*
//                    Company:		New Wave Eelectronics (ShenZhen) LTD.					*
//																							*
//*******************************************************************************************
//																							*
//              	 	Files Required: Omitted												*
//																							*
//*******************************************************************************************
//   Configuration Option Setting:                                                     		*					*
//*******************************************************************************************
//																							*
//   		 		Notes: 	1.Fsys = 4MHz(Extern) @3.3V											    *
//						   	2.WDT ENABLE (1S)												*
//																							*
//																							*
//*******************************************************************************************
//
//	Revision History:
//			1.Initial on 2019.07.17

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
		//��늺��{��fun_BodyFatScalesSDK_PowerOn()����
		fun_BodyFatScalesSDK_PowerOn();

		/* �û���ʼ������ */
		user_init();

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
		/* LED��ʾ����buffer��� */
		fun_diaplay_mode();

		/* �ж϶�ʱ�ػ� */
		is_timedshutdown();

		/* For Debug */
	}

	return 0;
}
