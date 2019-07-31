#ifndef __FUNCTION_H__
#define __FUNCTION_H__

extern u16 gu16v_weigh;
extern u16 gu16_display_weight;
extern u16 gu16v_impedence_data;
extern u8 gu8v_weigh_targeunit;
extern volatile u8  gu8v_05s_count;//led闪烁时亮灭之间的时间间隔的计数值.
extern volatile u8  gu8v_howtimes;//led闪烁多少次，计数一亮一灭算2次.
extern volatile u8  gu8v_led_speed;//led闪烁时亮灭之间的时间间隔,
extern volatile u8  gu8v_led_delay;//延迟x*100MS后开始执行闪烁功能.
extern u8  gu8v_timed_shutdown;
extern u8  gu8v_dismode;
extern u8 gu8v_worktasks;

extern void fun_Unit_Change(u16 weigh);
extern void set_ledflash(u8 mode, u8 ledflash, u8 count, u8 speed, u8 delay, u8 overtime);
//extern u8 get_ledflash_status(void);
extern void set_overtime2poweroff(u8 overtime);

//extern void fun_DisplayMode_NoLoad(void);
//extern void fun_DisplayMode_LoadFix(void);
//extern void fun_DisplayMode_LoadDown(void);
//extern void fun_DisplayMode_OverLoad(void);
//extern void fun_DisplayMode_ImpedanceFinish(void);
//extern void task_bodyfatscales(void);
extern void set_BHSDKState(u8 state);
extern u8 is_BHSDKState_change(void);
extern void Set_DisplayMode(u8 dismode);
extern u8 Get_DisplayMode(void);
extern void fun_DiaplsyMode(void);
extern void is_timedshutdown(void);
extern void task_scales2sleep(void);
//extern void task_scaleswakeup(void);
extern void fun_ble_task(void);
extern void task_bodyfatscales(void);

#endif //__FUNCTION_H__
