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
	gu8v_UartTxBuf[1] = (gu16_impedence_data*10) & 0x00FF;
	gu8v_UartTxBuf[2] = ((gu16_impedence_data*10) & 0xFF00 )>>8;//;
 	gu8v_UartTxBuf[3] = (gu16_display_weight*10) & 0x00FF;
 	gu8v_UartTxBuf[4] = ((gu16_display_weight*10)& 0xFF00 )>>8;
	gu8v_UartTxBuf[5] = 0x00;//0x00;
	gu8v_UartTxBuf[6] = 0x00;//0x00;
	gu8v_UartTxBuf[7] = 0x00;//0x00;
	gu8v_UartTxBuf[8] = gu8_weigh_targeunit;
	gu8v_UartTxBuf[9] = 0x00;//��������.
	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}

#if 0
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
 	gu8v_UartTxBuf[3] = (gu16_display_weight*10) & 0x00FF;
 	gu8v_UartTxBuf[4] = ((gu16_display_weight*10)& 0xFF00 )>>8;
 	gu8v_UartTxBuf[5] = 0x00;
 	gu8v_UartTxBuf[6] = 0x00;
 	gu8v_UartTxBuf[7] = 0x00;
 	gu8v_UartTxBuf[8] = 0x00;//gu8_weigh_targeunit;
 	gu8v_UartTxBuf[9] = 0x01;//��ʾ��������.
 	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}
#endif

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

/********************************************************************
Function: uart发送数据和接收到的数据处理.
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_UserProtocol()
{
	//	UART TX
	if((gu8v_UartTxCycle >= C_TIMEING_CYCLE100MS) && (!gbv_IsBusyUartTx))	// ���h���붨�r�l�ͣ���ֹ�����l��̫��
	{
		 gu8v_UartTxCycle = 0;
		if (gbv_TxSDKWeightStatus)
		{
			gbv_TxSDKWeightStatus = 0;
			fun_TxSDKImpedanceStatus();
		}
		else if (gbv_TxSDKImpedanceStatus)
		{
			gbv_TxSDKImpedanceStatus = 0;
			fun_TxSDKImpedanceStatus();
            gu16_impedence_data = 0;
		}
		else if (gbv_TxFinishStatus)
		{
			gbv_TxFinishStatus = 0;
		}
	}

	// UART RX
	if (gbv_UartRxSuccess)	// UART RX
	{
		gbv_UartRxSuccess = 0;

		if(REQ_UNITSYN == gu8v_UartRxBuf[POS_HEARD]){
			u8 XOR_checksum = get_XOR_Checksum(&gu8v_UartRxBuf[0],POS_CHECKSUM-1);
			if(XOR_checksum == gu8v_UartRxBuf[POS_CHECKSUM]){

				switch(gu8v_UartRxBuf[POS_CMDTYPE])
				{
					case CMDTYPE_CALIBRATION:

						break;

					case CMDTYPE_SHUTDOWN:
//						gu8_worktasks = TASK_SLEEP;
//						set_BHSDKState(ENTER_WEIGHT_NORMAL);
						break;

					case CMDTYPE_LO:
						gu8_worktasks = TASK_LOWBATTERY;
						break;

					case CMDTYPE_USEROK:

						break;

					case CMDTYPE_MOMBABY:
						break;

					default:
						break;
				}

				//单位转换.
				if( UNIT_KG == gu8v_UartRxBuf[POS_UNIT])
					gu8_weigh_targeunit = UNIT_KG;
				else if( UNIT_LB == gu8v_UartRxBuf[POS_UNIT])
					gu8_weigh_targeunit = UNIT_LB;
				fun_Unit_Change(gu16_weigh);

				//体脂率.
				gu16_BodyFatRate = (gu8v_UartRxBuf[4]&0x00FF)+((gu8v_UartRxBuf[5]<<8)&0xFF00);
				if((gu16_BodyFatRate != 0) && (C_BODYFAT_PERCENTAGE >= gu16_BodyFatRate)){
					fg_bodyfatrate_rec_ok = 1;
					gu8v_UartRxBuf[4] = 0;
					gu8v_UartRxBuf[5] = 0;
				}else{
					fg_bodyfatrate_rec_ok = 0;
				}
			}
		}

        u8 i = 0;
        for(i = 0; i < UART_LENGTH_RX; i++){
            gu8v_UartRxBuf[i] = 0;
        }
	}
}


