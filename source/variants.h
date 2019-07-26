#ifndef __VARIANTS_H__
#define __VARIANTS_H__

extern volatile __byte_type  	flag0_time;//时间标志
#define flag0_time_Byte			flag0_time.u8
//#define fg_time_2ms				flag0_time.bits.b0
//#define fg_time_100ms			flag0_time.bits.b1
#define fg_time_100ms			flag0_time.bits.b2
#define fg_time_1s				flag0_time.bits.b3
//#define fg_time_3s				flag0_time.bits.b4
#define fg_time_10s				flag0_time.bits.b5
//#define fg_time_test			flag0_time.bits.b6//
//#define fg_time_test2			flag0_time.bits.b7//


extern volatile __byte_type  	flag1;// 标志
#define flag1_Byte				flag1.u8
#define fg_led_timing			flag1.bits.b0
#define fg_led_delay			flag1.bits.b1
#define fg_led_flash			flag1.bits.b2
#define fg_led_change			flag1.bits.b3
#define fg_display_updata		flag1.bits.b4
//#define fg_time_10s			flag1.bits.b5
//#define fg_time_test			flag1.bits.b6//
//#define fg_time_test2			flag1.bits.b7//

extern volatile __byte_type  	flag2;// 标志
#define flag2_Byte				flag2.u8
#define fg_loadok				flag2.bits.b0
//#define fg_led_delay			flag2.bits.b1
//#define fg_led_flash			flag2.bits.b2
//#define fg_led_change			flag2.bits.b3
//#define fg_time_3s			flag2.bits.b4
//#define fg_time_10s			flag2.bits.b5
//#define fg_time_test			flag2.bits.b6//
//#define fg_time_test2			flag2.bits.b7//


extern u8 gu8v_time_100ms;
extern u8 gu8v_time_1s;

#endif //__VARIANTS_H__
