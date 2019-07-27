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
	gu8v_UartTxBuf[1] = gu16v_impedence_data & 0x00FF;
	gu8v_UartTxBuf[2] = (gu16v_impedence_data & 0xFF00 )>>8;;
	gu8v_UartTxBuf[3] = gu16_display_weight & 0x00FF;
	gu8v_UartTxBuf[4] = (gu16_display_weight & 0xFF00 )>>8;
	gu8v_UartTxBuf[5] = 0x00;
	gu8v_UartTxBuf[6] = 0x00;
	gu8v_UartTxBuf[7] = 0x00;
	gu8v_UartTxBuf[8] = gu8v_weigh_targeunit;
	gu8v_UartTxBuf[9] = 0x00;//锁定数据.
	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}

/********************************************************************
Function: 同步時間單位等配置
INPUT	:
OUTPUT	:
NOTE	: MCU喚醒后偵測連接服務器后主動索取當前單位及時間
********************************************************************/
void fun_TxSDKWeightStatus()
{
	gu8v_UartTxBuf[0] = 0xCF;
	gu8v_UartTxBuf[1] = 0x00;
	gu8v_UartTxBuf[2] = 0x00;
 	gu8v_UartTxBuf[3] = gu16_display_weight & 0x00FF;
 	gu8v_UartTxBuf[4] = (gu16_display_weight& 0xFF00 )>>8;
 	gu8v_UartTxBuf[5] = 0x00;
 	gu8v_UartTxBuf[6] = 0x00;
 	gu8v_UartTxBuf[7] = 0x00;
 	gu8v_UartTxBuf[8] = gu8v_weigh_targeunit;
 	gu8v_UartTxBuf[9] = 0x01;//表示过程数据.
 	gu8v_UartTxBuf[10] = get_XOR_Checksum(gu8v_UartTxBuf,10);
	fun_UartStartTx(11);
}
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
	// 數據接收 Time Out時間
	if (gu8v_TBRxTimeOutCnt >= C_TIMEING_TIMEOUT)
	{
		gu8v_TBRxTimeOutCnt = 0;
		lu8v_RxBufoffset = 0;
		lu8v_RxBufLength = 0xff;
		gbv_UartRxSuccess = 0;
	}
	//	UART TX
	//	if(!gbv_IsBusyUartTx)
	if((gu8v_UartTxCycle >= C_TIMEING_CYCLE2MS) && (!gbv_IsBusyUartTx))	// 建議加入定時發送，防止數據發送太快
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
		// 校驗判斷,是協議確認是否需要
		// volatile unsigned char i,CheckSum=0;
		// for(i=0; i<gu8v_UartRxBuf[2]-2;i++)
		// {
		// 	CheckSum ^= gu8v_UartRxBuf[i];
		// }
		// 數據解析
		/*
		if(gu8v_UartRxBuf[0] == RXFRAME_HEADER && gu8v_UartRxBuf[gu8v_UartRxBuf[2]-1]==RXFRAME_END)
		// if(gu8v_UartRxBuf[0] == RXFRAME_HEADER)
		{
			if(CheckSum ==  gu8v_UartRxBuf[gu8v_UartRxBuf[2]-2] )
			{
				// UART RX 數據解析
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
						gbv_TxSDKImpedanceStatus = 1;	// 配網失敗重新進入配網,知道這次60S週期到
					}
				}
				// AIRKISS ACK
				else if(gu8v_UartRxBuf[1] == CM_WIFI_AIRKISS  )
				{
					gbv_TxSDKImpedanceStatus = 0; // 收到AIRKISS命令,清除不再發送
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
	}
}


