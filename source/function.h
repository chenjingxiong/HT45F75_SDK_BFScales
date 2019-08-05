#ifndef __FUNCTION_H__
#define __FUNCTION_H__

extern u8  gu8_dismode;//LED显示模式.
extern u16 gu16_display_weight;//已经转换单位后的显示体重数据.
extern u8 gu8_weigh_targeunit;//目标体重单位.

/* LED 闪烁，显示，定时等变量*/
extern volatile u8  gu8v_time_dalay; //
extern volatile u8  gu8v_howtimes;  //
extern volatile u8  gu8v_led_speed; //闪烁的快慢
extern volatile u8  gu8v_led_delay; //延时delay时间后才执行闪烁.
extern volatile u8  gu8v_led_delay3S;//开机LED全显3s计时.
extern volatile u8  gu8v_timed_shutdown;//定时关机计时.

extern void fun_Unit_Change(u16 weigh);
extern void set_ledflash(u8 mode, u8 ledflash, u8 count, u8 speed, u8 delay, u8 overtime);
extern u8 get_ledflash_status(void);

extern void Set_DisplayMode(u8 dismode);
//extern u8 Get_DisplayMode(void);
extern void fun_diaplay_mode(void);
extern void set_overtime2poweroff(u8 overtime);
extern void is_timedshutdown(void);
extern void fun_timing(void);

#endif //__FUNCTION_H__
