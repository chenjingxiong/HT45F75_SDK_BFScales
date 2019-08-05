#ifndef __FUNCTION_H__
#define __FUNCTION_H__

extern u8  gu8_dismode;//LED��ʾģʽ.
extern u16 gu16_display_weight;//�Ѿ�ת����λ�����ʾ��������.
extern u8 gu8_weigh_targeunit;//Ŀ�����ص�λ.

/* LED ��˸����ʾ����ʱ�ȱ���*/
extern volatile u8  gu8v_time_dalay; //
extern volatile u8  gu8v_howtimes;  //
extern volatile u8  gu8v_led_speed; //��˸�Ŀ���
extern volatile u8  gu8v_led_delay; //��ʱdelayʱ����ִ����˸.
extern volatile u8  gu8v_led_delay3S;//����LEDȫ��3s��ʱ.
extern volatile u8  gu8v_timed_shutdown;//��ʱ�ػ���ʱ.

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
