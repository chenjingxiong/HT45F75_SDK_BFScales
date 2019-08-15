//___________________________________________________________________
//___________________________________________________________________
//  Copyright : 2015 BY HOLTEK SEMICONDUCTOR INC
//  File Name : UART.c
// Description: UART收發
//   Customer : Holtek Demo Code
//Targer Board: None
//     Note   :
//___________________________________________________________________
//___________________________________________________________________
#ifndef _UART_H_
#define _UART_H_

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 共用變量 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
extern volatile bit gbv_TxSDKWeightStatus;
extern volatile bit gbv_TxSDKImpedanceStatus;
extern volatile bit gbv_TxFinishStatus;


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 共用函數 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
extern u8 get_XOR_Checksum(volatile u8 * psurce, u8 length);
extern void fun_TxSDKImpedanceWeightStatus();
extern void fun_UserProtocol();




#endif
