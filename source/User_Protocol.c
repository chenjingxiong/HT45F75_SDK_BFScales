#include "common.h"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 變量定義 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
volatile bit gbv_TxSDKWeightStatus;		// 發送SDK狀態的標誌,在需要發送的地方置位falg即可
volatile bit gbv_TxSDKImpedanceStatus;
volatile bit gbv_TxFinishStatus;

//===============================================================
//function: 异或校验和.
//input   : *psurce：数据指针；     length:数据指针中的数据个数.
//output  : 异或校验和:
//description: 异或校验和:          eg: FD 34 00 00.  则:XOR_checksum= 0xFD ^ 0x34 ^ 0x00 ^ 0x00 = 0xC9
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
	// 要發送的數據寫入gu8v_UartTxBuf[x],0 > N
	gu8v_UartTxBuf[0] = 0x55;
	gu8v_UartTxBuf[1] = 0x11;
 	gu8v_UartTxBuf[2] = 0x22;
 	gu8v_UartTxBuf[3] = 0x33;
 	gu8v_UartTxBuf[4] = 0x44;
	// -----
 	gu8v_UartTxBuf[N]] = 0xAA;
	fun_UartStartTx(N);開始發送長度為N的數據幀
}
*/
/********************************************************************
Function: 發送阻抗量測狀態
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_TxSDKImpedanceStatus()
{
	gu8v_UartTxBuf[0] = 0xCF;
	gu8v_UartTxBuf[1] = (gu16v_impedence_data*10) & 0x00FF;
	gu8v_UartTxBuf[2] = ((gu16v_impedence_data*10) & 0xFF00 )>>8;//;
 	gu8v_UartTxBuf[3] = (gu16_display_weight*10) & 0x00FF;
 	gu8v_UartTxBuf[4] = ((gu16_display_weight*10)& 0xFF00 )>>8;
	gu8v_UartTxBuf[5] = 0x00;//0x00;
	gu8v_UartTxBuf[6] = 0x50;//0x00;
	gu8v_UartTxBuf[7] = 0x00;//0x00;
	gu8v_UartTxBuf[8] = gu8v_weigh_targeunit;
	gu8v_UartTxBuf[9] = 0x00;//锁定数据.
	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}

#if 0
/********************************************************************
Function: 同步時間單位等配置
INPUT	:
OUTPUT	:
NOTE	: MCU喚醒后偵測連接服務器后主動索取當前單位及時間
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
 	gu8v_UartTxBuf[8] = 0x00;//gu8v_weigh_targeunit;
 	gu8v_UartTxBuf[9] = 0x01;//表示过程数据.
 	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}
#endif

/********************************************************************
Function: 同步時間單位等配置
INPUT	:
OUTPUT	:
NOTE	: MCU喚醒后偵測連接服務器后主動索取當前單位及時間
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
Function: uart 管理
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_UserProtocol()
{
#if 1
    #if 0
	// 數據接收 Time Out時間
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
	if((gu8v_UartTxCycle >= C_TIMEING_CYCLE100MS) && (!gbv_IsBusyUartTx))	// 建議加入定時發送，防止數據發送太快
	{
		 gu8v_UartTxCycle = 0;
		if (gbv_TxSDKWeightStatus)
		{
			gbv_TxSDKWeightStatus = 0;
//			fun_TxSDKWeightStatus();
			fun_TxSDKImpedanceStatus();
		}
		else if (gbv_TxSDKImpedanceStatus)
		{
			gbv_TxSDKImpedanceStatus = 0;
			fun_TxSDKImpedanceStatus();
            gu16v_impedence_data = 0;
//			BHSDKState = ENTER_IMPEDANCE;
		}
		else if (gbv_TxFinishStatus)
		{
			gbv_TxFinishStatus = 0;
			//fun_TxFinishStatus();
		}
	}

	// UART RX
	if (gbv_UartRxSuccess)	// UART RX
	{
		gbv_UartRxSuccess = 0;

        //u8 XOR_checksum = get_XOR_Checksum(&gu8v_UartRxBuf[0],POS_CHECKSUM-1);//注意:最后一位为异或校验和,不需计算.
        switch(gu8v_UartRxBuf[0])
        {
			case REQ_TIME:

				break;

			case REQ_HISTORY:

				break;

			case REQ_DIS_BT:

				break;

			case REQ_VERTION:

				break;

            case REQ_UNITSYN:
                switch (gu8v_UartRxBuf[1])
                {
					case 0x00://鍚屾鍗曚綅
						//Byte2:鍗曚綅杞崲瀛楄妭
//						gu8v_weigh_targeunit = gu8v_UartRxBuf[2];
//						fun_Unit_Change(gu16v_weigh);
						break;

					case 0x37://浼犻�佽剛鑲俊鎭粰钃濈墮

						break;

					default:

						break;
				}

				//XOR_checksum = get_XOR_Checksum(&gu8v_UartRxBuf[0],POS_CHECKSUM-1);//娉ㄦ剰:鏈�鍚庝竴浣嶄负寮傛垨鏍￠獙鍜?涓嶉渶璁＄畻.
				//if(XOR_checksum == R_UartData_Buf[POS_CHECKSUM]){
					//Byte1:鍛戒护瀛?
					switch(gu8v_UartRxBuf[POS_CMDTYPE])
					{
						case CMDTYPE_CALIBRATION:

							break;

						case CMDTYPE_SHUTDOWN:
							gu8v_worktasks = TASK_SLEEP;
							set_BHSDKState(ENTER_WEIGHT_NORMAL);
							break;

						case CMDTYPE_LO:
							gu8v_worktasks = TASK_LOWBATTERY;
							break;

						case CMDTYPE_USEROK:

							break;

						case CMDTYPE_MOMBABY:

							break;

						default:
							break;
					}


					//Byte3:鐢ㄦ埛缁勬樉绀?
					//if(gu8v_UartRxBuf[POS_USER] <= USER_P9){
					//	gu8v_user = gu8v_UartRxBuf[POS_USER];
					//}
	//			}


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
        u8 i = 0;
        for(i = 0; i < UART_LENGTH_RX; i++){
            gu8v_UartRxBuf[i] = 0;
        }
	}
#endif
}


