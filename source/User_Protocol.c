#include "common.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ׃�����x @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
volatile bit gbv_TxSDKWeightStatus;		// �l��SDK��B�Ę��I,����Ҫ�l�͵ĵط���λfalg����
volatile bit gbv_TxSDKImpedanceStatus;
volatile bit gbv_TxFinishStatus;

//===============================================================
//function: ���У���.
//input   : *psurce������ָ�룻     length:����ָ���е����ݸ���.
//output  : ���У���:
//description: ���У���:          eg: FD 34 00 00.  ��:XOR_checksum= 0xFD ^ 0x34 ^ 0x00 ^ 0x00 = 0xC9
//===============================================================
u8 get_XOR_Checksum(volatile u8 * psurce, u8 length)
{

	u8 XOR_checksum = 0;
	while(length--){
		XOR_checksum ^= *psurce;
		psurce++;
	}

	return XOR_checksum;
}

/*
void fun_TxExample()
{
	// Ҫ�l�͵Ĕ�������gu8v_UartTxBuf[x],0 > N
	gu8v_UartTxBuf[0] = 0x55;
	gu8v_UartTxBuf[1] = 0x11;
 	gu8v_UartTxBuf[2] = 0x22;
 	gu8v_UartTxBuf[3] = 0x33;
 	gu8v_UartTxBuf[4] = 0x44;
	// -----
 	gu8v_UartTxBuf[N]] = 0xAA;
	fun_UartStartTx(N);�_ʼ�l���L�Ȟ�N�Ĕ�����
}
*/
/********************************************************************
Function: �l���迹���y��B
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_TxSDKImpedanceStatus()
{
	gu8v_UartTxBuf[0] = 0xCF;
	gu8v_UartTxBuf[1] = (gu16v_impedence_data*10) & 0x00FF;//gu16v_impedence_data & 0x00FF;
	gu8v_UartTxBuf[2] = ((gu16v_impedence_data*10) & 0xFF00 )>>8;//;
 	gu8v_UartTxBuf[3] = (gu16_display_weight*10) & 0x00FF;//((SDKWeight.DataStable)& 0xFF00 )>>8;//gu16v_weigh & 0x00FF;//0x00;//gu16_display_weight & 0x00FF;
 	gu8v_UartTxBuf[4] = ((gu16_display_weight*10)& 0xFF00 )>>8;//(SDKWeight.DataStable) & 0x00FF;//(gu16v_weigh& 0xFF00 )>>8;//0x14;//(gu16_display_weight& 0xFF00 )>>8;
	gu8v_UartTxBuf[5] = 0x00;//0x00;
	gu8v_UartTxBuf[6] = 0x50;//0x00;
	gu8v_UartTxBuf[7] = 0x00;//0x00;
	gu8v_UartTxBuf[8] = gu8v_weigh_targeunit;
	gu8v_UartTxBuf[9] = 0x00;//��������.
	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}

/********************************************************************
Function: ͬ���r�g��λ������
INPUT	:
OUTPUT	:
NOTE	: MCU���Ѻ�ɜy�B�ӷ�������������ȡ��ǰ��λ���r�g
********************************************************************/
void fun_TxSDKWeightStatus()
{
	//FD 34 00 00 00 00 00 00 00 00 C9
	gu8v_UartTxBuf[0] = 0xCF;
	gu8v_UartTxBuf[1] = 0x00;
	gu8v_UartTxBuf[2] = 0x00;
 	gu8v_UartTxBuf[3] = (gu16_display_weight*10) & 0x00FF;//((SDKWeight.DataStable)& 0xFF00 )>>8;//gu16v_weigh & 0x00FF;//0x00;//gu16_display_weight & 0x00FF;
 	gu8v_UartTxBuf[4] = ((gu16_display_weight*10)& 0xFF00 )>>8;//(SDKWeight.DataStable) & 0x00FF;//(gu16v_weigh& 0xFF00 )>>8;//0x14;//(gu16_display_weight& 0xFF00 )>>8;
 	gu8v_UartTxBuf[5] = 0x00;
 	gu8v_UartTxBuf[6] = 0x00;
 	gu8v_UartTxBuf[7] = 0x00;
 	gu8v_UartTxBuf[8] = 0x00;//gu8v_weigh_targeunit;
 	gu8v_UartTxBuf[9] = 0x01;//��ʾ��������.
 	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}
/********************************************************************
Function: ͬ���r�g��λ������
INPUT	:
OUTPUT	:
NOTE	: MCU���Ѻ�ɜy�B�ӷ�������������ȡ��ǰ��λ���r�g
********************************************************************/
void fun_TxFinishStatus()
{
#if 0
	gu8v_UartTxBuf[0] = 0x55;
	gu8v_UartTxBuf[1] = 0x00;
 	//gu8v_UartTxBuf[2] = (SDKADCFilterData.Current & 0xFF000000 )>>24;
 	gu8v_UartTxBuf[4] = (SDKADCFilterData.Current & 0x00FF0000 )>>16;
 	gu8v_UartTxBuf[3] = (SDKADCFilterData.Current & 0x0000FF00 )>>8;
 	gu8v_UartTxBuf[2] =  SDKADCFilterData.Current & 0x000000FF;
 	gu8v_UartTxBuf[5] = gu8v_UartTxBuf[0] + gu8v_UartTxBuf[1] + gu8v_UartTxBuf[2] + gu8v_UartTxBuf[3] + gu8v_UartTxBuf[4];
	fun_UartStartTx(5);
#endif
}

#if 0
void UART_SendData(u8* pdata, u8 len)
{
	volatile u8 i = 0;
	while(i < len){
		GCC_CLRWDT();
		if((_txif == 1)&&(_tidle == 1)){
			_acc = _usr;
			_txrrxr = *pdata;
			i++;
			pdata++;
		}
	}
}
#endif

/********************************************************************
Function: uart ����
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_UserProtocol()
{
#if 1
    #if 0
	// �������� Time Out�r�g
	if (gu8v_TBRxTimeOutCnt >= C_TIMEING_TIMEOUT)
	{
		gu8v_TBRxTimeOutCnt = 0;
		lu8v_RxBufoffset = 0;
		lu8v_RxBufLength = 0;
		gbv_UartRxSuccess = 0;
	}
    #endif
	//	UART TX
	//	if(!gbv_IsBusyUartTx)
	if((gu8v_UartTxCycle >= C_TIMEING_CYCLE2MS) && (!gbv_IsBusyUartTx))	// ���h���붨�r�l�ͣ���ֹ�����l��̫��
	{
		 gu8v_UartTxCycle = 0;
		if (gbv_TxSDKWeightStatus)
		{
			gbv_TxSDKWeightStatus = 0;
			fun_TxSDKWeightStatus();
		}
		else if (gbv_TxSDKImpedanceStatus)
		{
			gbv_TxSDKImpedanceStatus = 0;
			fun_TxSDKImpedanceStatus();
//			BHSDKState = ENTER_IMPEDANCE;
		}
		else if (gbv_TxFinishStatus)
		{
			gbv_TxFinishStatus = 0;
			//fun_TxFinishStatus();
		}
//        if (SDKADCSourceData.flag.b.IsReady)
//		{
//			SDKADCSourceData.flag.b.IsReady = 0;
//			fun_TxSDKADCSourceData();
//		}
	}

	// UART RX
	if (gbv_UartRxSuccess)	// UART RX
	{
		gbv_UartRxSuccess = 0;
		// У��Д�,�ǅf�h�_�J�Ƿ���Ҫ
		// volatile unsigned char i,CheckSum=0;
		// for(i=0; i<gu8v_UartRxBuf[2]-2;i++)
		// {
		// 	CheckSum ^= gu8v_UartRxBuf[i];
		// }
		// ��������
		/*
		if(gu8v_UartRxBuf[0] == RXFRAME_HEADER && gu8v_UartRxBuf[gu8v_UartRxBuf[2]-1]==RXFRAME_END)
		// if(gu8v_UartRxBuf[0] == RXFRAME_HEADER)
		{
			if(CheckSum ==  gu8v_UartRxBuf[gu8v_UartRxBuf[2]-2] )
			{
				// UART RX ��������
				// WIFI STATUS
				if(gu8v_UartRxBuf[1] == CM_WIFI_STATUS  )
				{
					gu8v_WIFIStatus = gu8v_UartRxBuf[3];
					if (gu8v_WIFIStatus == WIFI_STATUS_AIRKSS_PASS)
					{
						gu8v_AIRKISSStatus  = AIRKISSSTATUS_PASS;
					}
					else if (gu8v_WIFIStatus == WIFI_STATUS_AIRKSS_FAIL)
					{
						gbv_TxSDKImpedanceStatus = 1;	// ��Wʧ�������M����W,֪���@��60S�L�ڵ�
					}
				}
				// AIRKISS ACK
				else if(gu8v_UartRxBuf[1] == CM_WIFI_AIRKISS  )
				{
					gbv_TxSDKImpedanceStatus = 0; // �յ�AIRKISS����,������ٰl��
				}
			}
			else
			{
				lu8v_RxBufoffset = 0;
			}
		}
		else
		{
			lu8v_RxBufoffset =0;
		}
		*/
		
       // XOR_checksum = get_XOR_Checksum(&R_UartData_Buf[0],POS_CHECKSUM-1);//ע��:���һλΪ���У���,�������.
        switch(gu8v_UartRxBuf[0])
        {
            case CMD_HEARD:
                switch (gu8v_UartRxBuf[1])
                {
                    case 0x00://ͬ����λ
                            gu8v_weigh_targeunit = gu8v_UartRxBuf[2];
                            //fun_Unit_Change(gu16v_weigh);               
                        break;
                        
                    case 0x37://����֬����Ϣ������
                    
                        break;
                        
                    default:
                    
                        break;
                }
                gu16v_pct_data = (gu8v_UartRxBuf[4]&0x00FF)+((gu8v_UartRxBuf[5]<<8)&0xFF00);
                if(gu16v_pct_data != 0){
                    fg_pct_ok = 1;
                    gu8v_UartRxBuf[4] = 0;
                    gu8v_UartRxBuf[5] = 0;
                }else{
                    fg_pct_ok = 0;
                }
                break;

            default:
                break;
        }
        
	}
#endif
}


