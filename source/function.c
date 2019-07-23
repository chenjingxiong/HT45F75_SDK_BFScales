#include "common.h"
u16 display_weight;
u8 gu8v_weigh_targeunit;
volatile u8  gu8v_05s_count;//led闪烁时亮灭之间的时间间隔的计数值.
volatile u8  gu8v_howtimes;//led闪烁多少次，计数一亮一灭算2次.
volatile u8  gu8v_led_speed;//led闪烁时亮灭之间的时间间隔,
volatile u8  gu8v_led_delay;//延迟x*100MS后开始执行闪烁功能.
u8  gu8v_timed_shutdown;

void fun_Unit_Change(u16 weigh)
{
//	gu16v_weigh = weigh;

	if(UNIT_KG == gu8v_weigh_targeunit)
	{
		#if 1//(_UART_DEBUG == ENABLE)
			display_weight = (u32)weigh/10; //KG換算 符号强制转换，防止溢出
		#else
			gu16v_Display_Weigh = weigh;
		#endif
	}
	else if(UNIT_LB == gu8v_weigh_targeunit)
	{
		display_weight = (u32)weigh*10000/45359; //LB換算
		/*说明:
			10(g) = 0.0220462(lb),0.1(lb) = 45.359237(g)

			 eg:
			 	若:weigh = 55
			 	则:weigh=55=550(g) -->因为此合泰库精度为10g，即1代表1*10=10(g);x*10=10x(g)
			 	计算:weigh*10000/45359=55*10000/45359
			 		取整数位等于12，代表1.2(lb)
		*/
	}
	else if(UNIT_JIN == gu8v_weigh_targeunit)
	{
		display_weight = (u32)weigh/5; 	//JIN  0.1斤  = 5(weigh) ,gu16v_Display_Weigh的个位代表小数点后一位，保留 一位小數點
		/*说明:
			eg:
			   若:weigh = 44
			   则:weigh=44=440(g) -->因为此合泰库精度为10g，即1代表1*10=10(g);x*10=10x(g)
			   计算:weigh/5 = 44/5 = 8.8
					取整数位等于8，代表0.8(斤)
		*/
	}
}

//===============================================================
//function: 设置LED闪烁.
//input   :
//			mode: LED显示的模式
//			ledflash: =0 关闭闪烁; =1 开启闪烁
//			count: 闪烁次数: =0 不闪烁, =x 闪烁x次.
//			speed: 闪烁的快慢,取值范围0~255,时间等于x100MS.
//			delay: 延迟 delay*100MS 时间后才开始执行闪烁功能
//			overtime:设置定时休眠时间,时间= 100ms*(overtime+delay)
//output  : NULL
//description: 进入休眠前的设置，和唤醒后的初始化.
//===============================================================
void set_ledflash(u8 mode, u8 ledflash, u8 count, u8 speed, u8 delay, u8 overtime)
{
	static u8 mode_backup = 0xaa;
	if(mode_backup != mode){
		mode_backup = mode;

		if(C_LED_FLASH_ON == ledflash){
			fg_led_delay = 0;
			fg_led_timing = 1;
			gu8v_05s_count = 0;
			gu8v_howtimes = count;
			gu8v_led_speed = speed;
			gu8v_led_delay = delay;
		}else{
			fg_led_timing = 0;
			if(overtime)fg_led_flash = 0;
		}
		fg_led_change = 0;
//		overtime = overtime + delay;
		set_overtime2poweroff(overtime);
	}
}

#if 0
u8 get_ledflash_status(void)
{
	return fg_led_timing;
}
#endif

//===============================================================
//function: 设置超时关机时间.
//input   : overtime:单位为100ms;	eg:overtime=10,则代表1s,即10*100ms=1S.
//output  : NULL
//description: 时间: 100MS*overtime.
//===============================================================
void set_overtime2poweroff(u8 overtime)
{
	gu8v_timed_shutdown = overtime;
}


void fun_DisplayMode_NoLoad(void)
{
	fun_Unit_Change(SDKWeight.DataCurrent);
	fun_HEX2BCD(display_weight);
	set_ledflash(BHSDKState,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
}

void fun_DisplayMode_LoadFix(void)
{
	fun_Unit_Change(SDKWeight.DataStable);
	fun_HEX2BCD(display_weight);
	set_ledflash(BHSDKState,C_LED_FLASH_ON,C_LED_FLASH_CNT,C_TIME_05S,C_LED_FLASH_DELAY,C_TIME_10S);
}


void fun_DisplayMode_LoadDown(void)
{
	fun_Unit_Change(SDKWeight.DataStable);
	fun_HEX2BCD(display_weight);

	set_ledflash(BHSDKState,C_LED_FLASH_OFF,0,0,0,C_TIME_10S);
}

