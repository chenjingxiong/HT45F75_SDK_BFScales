#include "common.h"



volatile unsigned char 	gu8v_UartTxBuf[UART_LENGTH_TX];
volatile unsigned char 	gu8v_UartRxBuf[UART_LENGTH_RX];
volatile unsigned char  lu8v_TxBufoffset;   // TX 偏移量
volatile unsigned char  lu8v_TxBufLength;   // TX 幀長度
volatile unsigned char  gu8v_UartTxCycle;   // TX 發送週期
volatile bit gbv_IsBusyUartTx;              // TX is busy

volatile unsigned char  lu8v_RxBufoffset;   // RX 偏移量
volatile unsigned char  lu8v_RxBufLength;   // RX 幀長度
volatile unsigned char  gu8v_TBRxTimeOutCnt;// RX 幀timeout
volatile bit gbv_UartRxSuccess;             // Rx is ok,user can process the rx buf data
/********************************************************************
Function: Uart初始化程序
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_UARTPowerOnInit()
{
    // 數據傳輸格式設定
	SET_UART_Format_D8_P0_S1();
    // 波特率設定
	SET_UART_BAUTRATE_9600();
	SET_UART_ADDRESS_DISABLE();
	SET_UART_RECEIVER_IE_ENABLE();
	SET_UART_TRANSMITTER_IE_ENABLE();
	SET_UART_TRANSMITTER_EMPTY_ENABLE();
	_mf1e = 1;
	SET_UART_ENABLE();
	// UART0 IO

}

///********************************************************************
//Function: Uart數據發送和接收中斷子程序
//INPUT	:
//OUTPUT:
//NOTE	:
//********************************************************************/
DEFINE_ISR(UART_ISR, 0x028)
{
	// 奇偶校验出错
//	if (_perr0)
//	{
//		_acc = _usr;
//		_acc = _txr_rxr;
//	}
//	 噪声干扰错误
	if (_nf)
	{
		_acc = _usr;
		_acc = _txrrxr;
		lu8v_RxBufoffset = 0;
	}
	// 帧错误
	if (_ferr)
	{
		_acc = _usr;
		_acc = _txrrxr;
		lu8v_RxBufoffset = 0;
	}
	// 溢出错误
	if (_oerr)
	{
		_acc = _usr;
		_acc = _txrrxr;
		lu8v_RxBufoffset = 0;
	}
	// 发送数据
	if (_txif && gbv_IsBusyUartTx)
	{
		if (lu8v_TxBufoffset <= lu8v_TxBufLength)
		{
			_txrrxr = gu8v_UartTxBuf[lu8v_TxBufoffset];
		}
		else
		{
            if (_tidle)
            {
                gbv_IsBusyUartTx = 0;
            }
		}
		lu8v_TxBufoffset++;
	}
	// 接收数据
	if (_rxif)
	{
        gu8v_UartRxBuf[lu8v_RxBufoffset] = _txrrxr;
		gu8v_TBRxTimeOutCnt = 0;
        lu8v_RxBufoffset++;
        // 用戶需要在此寫Rx數據長度
        // 1 固定長度,則在初始化的時候初始化話lu8v_RxBufLength
        // 2 協議自帶長度信息
        if (lu8v_RxBufoffset == 3)
        {
        	lu8v_RxBufLength = gu8v_UartRxBuf[2];
        }
        if (lu8v_RxBufoffset >= lu8v_RxBufLength)
        {
            lu8v_RxBufoffset = 0;
            lu8v_RxBufLength = 0xff;
            gbv_UartRxSuccess  = 1;
        }
	}
}

/********************************************************************
Function: uart發送開始
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void  fun_UartStartTx( unsigned char TxLength)
{
	lu8v_TxBufLength = TxLength;
	// Start TX
	gbv_IsBusyUartTx = 1; // uart 忙碌
	lu8v_TxBufoffset = 1; // 因為0已經直接發送了
	_acc = _usr;
	_txrrxr = gu8v_UartTxBuf[0];
}
