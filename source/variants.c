#include "common.h"

volatile __byte_type  	flag0_time;//时间标志
volatile __byte_type  	flag1;// 标志
volatile __byte_type  	flag2;// 标志
volatile __byte_type  	flag3_remember;// 标志


u8 gu8v_time_100ms;
u8 gu8v_time_30s;//记忆200g功能,上一次和现在相差小于200g则显示上一次结果,休眠30s后清除记忆.
u8 gu8_ble_count;
u8 R_UartData;


