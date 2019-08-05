
#include "common.h"

//===============================================================
//function: 时钟初始化,在编译器中配置选项设定
//input   : none.
//output  : none
//description:
//===============================================================
void SysFrequencyInit(void)
{
	_hlclk = 1;
}

//===============================================================
//function: 清除RAM:Sector0,Sector3:0x80~0xFF.
//input   : none.
//output  : none
//description:
//===============================================================
void fun_RamInit(void)
{
	_mp1h = 0;
	_mp1l = 0x80;
	while(_mp1h <2)
	{
		for(_tblp = 0x00;_tblp < 128;_tblp++)
		{
			 _iar1 = 0;
			  _mp1l++;
		}
		_mp1l = 0x80;
		_mp1h++;
	}

	_mp1h = 3;
	_mp1l = 0x80;
	for(_tblp = 0x00;_tblp < 128;_tblp++)
	{
		_iar1 = 0;
		_mp1l++;
	}

}
/********************************************************************
Function: GPIO初始化
INPUT	: none
OUTPUT	: none
NOTE	: 配置所有IO口初始状态.
********************************************************************/
void fun_GPIOInit(void)
{
	//IO
	_pa  = PA_Default;		//PA PORT
	_pac = PAC_Default;
	_papu= PAPU_Default;
	_pawu= PAWU_Default;

	_pb  = PB_Default;		//PB PORT
	_pbc = PBC_Default;
	_pbpu= PBPU_Default;

	_pc  = PC_Default;		//PC PORT
	_pcc = PCC_Default;
	_pcpu= PCPU_Default;

	_pd  = PD_Default;		//PD PORT
	_pdc = PDC_Default;
	_pdpu= PDPU_Default;

	 P_LED_BLE = LOW;//����
	 P_LED_BLE_C = OUTPUT;
	 P_LED_UNIT_PCT	= LOW;//�ٷ�??%
	 P_LED_UNIT_PCT_C = OUTPUT;

	/*Bluetooth init */
	 P_BT_Status_C = INPUT;//INPUT;//OUTPUT;//�ͻ��������Ŷ���:����  BLE_EN�������룬BLE_ST�ߵ�ƽ���
	 P_BLE_EN_C = OUTPUT;
	 P_BLE_EN = LOW;
//	 P_BT_Status = LOW;//LOW;
	_ctrl0 = 0x00;
	SETLEDCURRENT_LEVEL3();
}


//===============================================================
//function: LED 所有IO口初始化为关闭LED状态.
//input   : none.
//output  : none
//description:
//===============================================================
void LED_Init(void)
{
	LEDCOM1 = LOW ;
	LEDCOM2 = LOW ;
	LEDCOM3 = LOW ;
	LEDCOM4 = LOW ;
	LEDSEG = LOW;
	LEDCOMC1 = OUTPUT ;
	LEDCOMC2 = OUTPUT ;
	LEDCOMC3 = OUTPUT ;
	LEDCOMC4 = OUTPUT ;
	LEDSEGC  = OUTPUT ;
	P_LED_BLE = LOW;
	P_LED_BLE_C = OUTPUT;
	P_LED_UNIT_PCT = LOW;
	P_LED_UNIT_PCT_C = OUTPUT;
}

/********************************************************************
Function: Timer初始化
INPUT	: none
OUTPUT	: none
NOTE	:
********************************************************************/
void fun_TimerInit(void)
{
	// TM0
	_tm0c0 = 0x20;		// fsys/16 4us
	_tm0c1 = 0xc1;		// TimeCnt Mode
	_tm0al = 500%256; 	// 500*4us = 2ms;
	_tm0ah = 500/256;
	_t0on  = 1;
	SETCTMA_ISR_ENABLE();

	// TM1
//    _ptm1c0= 0x20;		// fsys/4 4us
//    _ptm1c1= 0xc1;		// TimeCnt Mode
//    _ptm1al= 500%256;	// 500*4us = 2ms
//    _ptm1ah= 500/256;
//    _t1on = 1;
	_t1on = 0;
//	SETPTM1A_ISR_ENABLE();

	// TM2
   // _ptm2c0= 0x00;		// fsys/4 1us
   // _ptm2c1= 0xc1;		// TimeCnt Mode
   // _ptm2al= 1000%256;	// 1000*1us = 1ms
   // _ptm2ah= 1000/256;
   // _ptm2on=1;
	_t2on = 0;

	// TimeBase 0
//	 SETTIMEBASE0_8MS();
//	 SETTIMEBASE0_ISR_Enable();
//	 SETTIMEBASE_ON();
	SETTIMEBASE0_ISR_Disable();
	SETTIMEBASE_OFF();


	// TimeBase 1
//	 SETTIMEBASE1_1000MS();
//	 SETTIMEBASE1_ISR_Enable();
//	 SETTIMEBASE_ON();
	SETTIMEBASE1_ISR_Disable();
	SETTIMEBASE_OFF();

}

/********************************************************************
Function: MCU上電初始化
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/
void fun_PowerOnSysInit(void)
{
	//IAP not need	to init
	//EEPROM not need to init
	//Wait Sys Clock Ok

	SysFrequencyInit();
//	while(!_hto)
//	{
//		GCC_CLRWDT();
//	}
	//RAM
	fun_RamInit();
	//WDT
	SETWDTTIME_125MS();
	//LVR
	SETLVR_2_1V();//����ִ��??LVR �������߻����ʱ�Զ����ܹر�??
	//LVD
//	SETLVD_LVDIN();
//	SETLVD_ENABLE();
	SETLVD_DISABLE();

	//GPIO
	fun_GPIOInit();
	// TM & TimeBase
	fun_TimerInit();
	// SIM User Define
	// UART User Define
	// ADC User Define
}

/********************************************************************
Function: 關閉各個模塊進入HLAT模式
INPUT	:
OUTPUT	:
NOTE	:
********************************************************************/

void fun_PrepareToHalt()
{

	//�P�] timer
	_t0on = 0;
	_t1on = 0;
	_t2on = 0;
	// �P�]�Д�
	_emi = 0;
	SETTIMEBASE_OFF();
//	_tbon = 1;
	SETWDTTIME_1000MS();// 1S����
}

/********************************************************************
Function: 用户设定初始化.
INPUT	:
OUTPUT	:
NOTE	:初始化一些变量,打开中断等.
********************************************************************/
void user_init(void)
{
	Set_AllLEDBuffer(0);
	Set_DisplayMode(DISPLAY_ALLOFF);
	gu8_weigh_targeunit = UNIT_KG;
	set_overtime2poweroff(C_TIME_10S);

	flag_led_Byte = 0x00;
	flag0_time_Byte = 0x00;
	flag1_Byte = 0x00;
	flag2_Byte = 0x00;
	flag3_remenber_Byte = 0x00;


    gbv_IsBusyUartTx = 0;
    gbv_UartRxSuccess = 0;

	//上电开机LED全显3S
    gu8v_led_delay3S = 0;
    gu8_worktasks = TASK_STARTUP;


	//中断:定时和显示扫描.
	_t0on  = 1;
	SETCTMA_ISR_ENABLE();
	_emi = 1;
}

//@------------???��?0???????-------------@
#if 0
DEFINE_ISR(INT0_ISR, INT0_VECTOR)
{

}
//@------------???��?0???????-------------@
DEFINE_ISR(INT1_ISR, INT1_VECTOR)
{
	//gbv_Data_Recive_flag = 1;
}
#endif
//@-------MuFunction0 ?��????????-----------@
//TM0
DEFINE_ISR(MuFunction0_ISR, MuFunction0_VECTOR)
{
	_t0pf=0;
	_t0af=0;
	_mf0f=0;

	fg_time_2ms = 1;

	/* 定时时间 */
	if(C_TIME_100MS <= ++gu8v_time_100ms){
		gu8v_time_100ms = 0;
		fg_time_100ms = 1;

		//timing
		{
			if(!fg_time_3s){
				gu8v_led_delay3S++;
				if(C_TIME_3S <= gu8v_led_delay3S){
					gu8v_led_delay3S = 0;
					fg_time_3s = 1;
				}
			}

			/* 串口发送周期计数 */
			if(C_TIMEING_CYCLE100MS >= gu8v_UartTxCycle) gu8v_UartTxCycle++;

			/* 串口接收超时检测 */
			if(!gbv_UartRxSuccess && fg_uart_rec_start){
				if(gu8v_TBRxTimeOutCnt){
					gu8v_TBRxTimeOutCnt--;
				}else{
					//gbv_UartRxSuccess = 1;
					fg_uart_rec_start = 0;
				}
			}

			/* LED显示闪烁定时 */
			if(fg_led_timing){
				//先延后执行LED闪烁功能.
				if(!fg_led_delay){
					if(gu8v_led_delay)
						gu8v_led_delay--;
					else
						fg_led_delay = 1;
				}
				//延时时间到后执行LED显示闪烁切换标志
				if(fg_led_delay){
					gu8v_time_dalay++;
					if(gu8v_led_speed <= gu8v_time_dalay){
						gu8v_time_dalay = 0;
						if(gu8v_howtimes){
							fg_led_flash = !fg_led_flash;//控制LED一亮一灭闪.
							//fg_led_change:可以用来控制闪烁时体脂与体重的轮流闪烁
							//注意:最好在fg_led_flash=1即LED处于熄灭状态下切换.
							if(fg_led_flash){
								fg_led_change = !fg_led_change;
							}
							gu8v_howtimes--;
						}else{
							fg_led_delay = 0;
							fg_led_timing = 0;
							fg_led_flash = 0;
							fg_led_change = 0;
						}
					}
				}
			}else{
				/*执行闪烁完成后开始计时定时关机*/
				if(gu8v_timed_shutdown){
					gu8v_timed_shutdown--;
					fg_time_10s = 0;
					fg_led_flash = 0;
				}else{
					fg_time_10s = 1;
				}
			}
		}
	}

	//*********** LED *************//
	//fun_LEDBufScan();
	LEDCOM1 = LOW ;
	LEDCOM2 = LOW ;
	LEDCOM3 = LOW ;
	LEDCOM4 = LOW ;
	LEDCOMC1 = OUTPUT ;
	LEDCOMC2 = OUTPUT ;
	LEDCOMC3 = OUTPUT ;
	LEDCOMC4 = OUTPUT ;
	LEDSEGC  = OUTPUT ;
	if(!fg_led_flash){
		switch (LEDScan_Cnt)
		{
			case 0:
				LEDSEG = lu8v_LED_HEX[gu8v_LED_Buffer[NUM_GE]];
				if(fg_led_unit_kg)
				{
					LEDSEG_UNIT_PIONT=HIGH;
				}
				LEDCOM1 = HIGH;
				LEDScan_Cnt = 1;
				break;

			case 1:
				LEDSEG = lu8v_2COM_HEX[gu8v_LED_Buffer[NUM_SHI]];
				if(fg_led_unit_lb)
				{
					LEDSEG_UNIT_PIONT=HIGH;
				}
				LEDCOM2 = HIGH;
				LEDScan_Cnt = 2;
				break;

			case 2:
				LEDSEG = lu8v_LED_HEX[gu8v_LED_Buffer[NUM_BAI]];
				if(fg_led_piont1)
				{
					LEDSEG_UNIT_PIONT = HIGH;
				}
				LEDCOM3 = HIGH;
				LEDScan_Cnt = 3;
				break;

			case 3:
				LEDSEG = lu8v_LED_HEX[gu8v_LED_Buffer[NUM_QIAN]];
				if(fg_led_piont2)
				{
					LEDSEG_UNIT_PIONT=HIGH;
				}
				LEDCOM4 = HIGH;
				LEDScan_Cnt = 0;
				break;

		default:
			LEDScan_Cnt = 0;
			break;
		}

		if(fg_led_ble)//图标:"蓝牙"
		{
			P_LED_BLE = HIGH;
		}else{
			P_LED_BLE = LOW;
		}

		if(fg_led_unit_pct)//图标:"%"
		{
			P_LED_UNIT_PCT = HIGH;
		}else{
			P_LED_UNIT_PCT = LOW;
		}
	}else{
		//LED全部关闭
		P_LED_UNIT_PCT = LOW;
		P_LED_BLE = LOW;
		LEDSEG_UNIT_PIONT = LOW;
		LEDSEG = LOW;
	}
}
//@------------ADC ?��????????--------------@
//DEFINE_ISR(ADC_ISR, ADC_VECTOR)
//{
//
//}
//@----------Timebase0 ?��????????----------@
#if 0
DEFINE_ISR(Timebase0_ISR, Timebase0_VECTOR)
{
//	Timebase_2S_LVD_Check++;
//	gu16v_Updata_Display_Time++;
//	gu16v_Test_OutTime++;
//	gbv_8ms_Using_Key_Scan = 1;
}
//@----------Timebase1 ?��????????----------@
DEFINE_ISR(Timebase1_ISR, Timebase1_VECTOR)
{
/*	gu8v_HaltTime++;*/
	_tb1f = 0;

}
#endif
//@-------MuFunction1 ?��????????----------@
// LVD&EEPROM&UART&SIM
#if 0
DEFINE_ISR(MuFunction1_ISR, MuFunction1_VECTOR)
{

	_mf1f=0;
	if (_nf)
	{
		_acc = _usr;
		_acc = _txrrxr;
		lu8v_RxBufoffset = 0;
	}
	// 帧错�?
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
	// 发送数�?
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
		R_UartData = _txrrxr;
		if(gbv_UartRxSuccess) {
			fg_uart_rec_start = 0;
			return;
		}

		gu8v_TBRxTimeOutCnt = C_TIMEING_TIMEOUT;

		if(!fg_uart_rec_start){
			switch(R_UartData)
			{
				case REQ_UNITSYN:
					fg_uart_rec_start = 1;
					fg_uart_rec_end = 0;
					lu8v_RxBufLength = DATA_BUF_LEN;
					break;
				default:
					lu8v_RxBufLength = DATA_BUF_LEN;
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

			gu8v_UartRxBuf[lu8v_RxBufoffset++] = R_UartData;

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

    _acc = _usr;
    _acc = _txrrxr;
}
#endif
#if 0
//@-------MuFunction2 ?��????????----------@
// I2C &TM1
DEFINE_ISR(MuFunction2_ISP, MuFunction2_VECTOR)
{
	_t1pf=0;
	_t1af=0;
	_mf2f=0;
}
//@-------MuFunction3 ?��????????----------@
// TM2
DEFINE_ISR(MuFunction3_ISP, MuFunction3_VECTOR)
{
	_t2pf=0;
	_t2af=0;
	_mf3f=0;
}
#endif




