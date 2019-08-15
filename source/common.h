#ifndef _COMMON_H_
#define _COMMON_H_

#include "typedef.h"
#include "HT45F75.h"
#include "..\HT45F75_77_BIA4Scales_SDK\SDK_Interface.h"
#include "BodyFat_R.h"
#include "HT45F75_UART.h"
#include "User_Protocol.h"
#include "Sys_Init.h"
#include "LEDScalesSDK.h"
#include "GPIO.h"
#include "variants.h"
#include "mydefine.h"
#include "bodyfatscales_task.h"
#include "function.h"

#define IsAutoCalOn SDKWeightSetting.flag.b.IsAutoCalOn
#endif
