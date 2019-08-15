#include "common.h"



volatile unsigned char 	gu8v_UartTxBuf[UART_LENGTH_TX];
volatile unsigned char 	gu8v_UartRxBuf[UART_LENGTH_RX];
volatile unsigned char  lu8v_TxBufoffset;   // TX ƫ����
volatile unsigned char  lu8v_TxBufLength;   // TX ���L��
volatile unsigned char  gu8v_UartTxCycle;   // TX �l���L��
volatile bit gbv_IsBusyUartTx;              // TX is busy

volatile unsigned char  lu8v_RxBufoffset;   // RX ƫ����
volatile unsigned char  lu8v_RxBufLength;   // RX ���L��
volatile unsigned char  gu8v_TBRxTimeOutCnt;// RX ��timeout
volatile bit gbv_UartRxSuccess;             // Rx is ok,user can process the rx buf data
/********************************************************************
Function: Uart��ʼ��
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_UARTPowerOnInit()
{
    // 數據傳輸格式設定
	SET_UART_Format_D8_P0_S1();
    // 波特率設�?
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
//Function: Uart Receive & Send
//INPUT	:
//OUTPUT:
//NOTE	:
//********************************************************************/
DEFINE_ISR(UART_ISR, MuFunction1_VECTOR)
{

	_mf1f=0;
	// �������Ŵ���
	if (_nf)
	{
		_acc = _usr;
		_acc = _txrrxr;
		lu8v_RxBufoffset = 0;
	}
	// ֡����
	if (_ferr)
	{
		_acc = _usr;
		_acc = _txrrxr;
		lu8v_RxBufoffset = 0;
	}
	// �������
	if (_oerr)
	{
		_acc = _usr;
		_acc = _txrrxr;
		lu8v_RxBufoffset = 0;
	}
	// ��������
	if (_txif && gbv_IsBusyUartTx)
	{
		if (lu8v_TxBufoffset < lu8v_TxBufLength)
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


	_uif = 0;
	// ��������
	if (_rxif)
	{
		_rxif = 0;

		_acc = _usr;
		gu8_UartData = _txrrxr;
		if(gbv_UartRxSuccess) {
			fg_uart_rec_start = 0;
			return;
		}

		gu8v_TBRxTimeOutCnt = C_TIMEING_TIMEOUT;

		if(!fg_uart_rec_start){
			switch(gu8_UartData)
			{
				case REC_HEAD_CODE:
					fg_uart_rec_start = 1;
					fg_uart_rec_end = 0;
					lu8v_RxBufLength = REC_BUF_DATA_LEN;
					break;
				default:
					lu8v_RxBufLength = REC_BUF_DATA_LEN;
					break;
			}
			lu8v_RxBufoffset = 0;
		}

		//start recive
		if(fg_uart_rec_start){

			if(fg_uart_rec_start && (lu8v_RxBufLength <= lu8v_RxBufoffset)){
				fg_uart_rec_start = 0;
				fg_uart_rec_end = 0;
				lu8v_RxBufoffset = 0;
			}

			gu8v_UartRxBuf[lu8v_RxBufoffset++] = gu8_UartData;

			if(lu8v_RxBufLength <= lu8v_RxBufoffset)
				fg_uart_rec_end = 1;

			if(fg_uart_rec_end){
				lu8v_RxBufoffset = 0;
				gbv_UartRxSuccess = 1;
				fg_uart_rec_start = 0;
				fg_uart_rec_end = 0;
			}
		}
	}
}

/********************************************************************
Function: Uart��ʼ����.
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
