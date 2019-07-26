//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ====================================================================================@
//                              此文件不能修改!!!                                       @
// ====================================================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#ifndef _SDK_TYPEDEF_H_
#define _SDK_TYPEDEF_H_
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
typedef enum
{
    // 關閉SDK
    ENTER_SDK_NONE, // 0 SDK 空循環,ADC及阻抗電路保持原來狀態
    STATE_SDK_NONE, // 1 SDK 處於空循環,ADC及阻抗電路保持原來狀態
    // 普通稱重
    ENTER_WEIGHT_NORMAL,   // 2 進入正常稱重模式
    STATE_WEIGHT_PREPARE,  // 3 稱重準備中
    STATE_WEIGHT_NOLOAD,   // 4 空載/當前重量小於最小稱重重量
    STATE_WEIGHT_LOADUP,   // 5 有上稱動作/鎖定狀態下加載解鎖重量,重量由0點變成>最小稱重重量
    STATE_WEIGHT_LOADOK,   // 6完成一次稱重測量
    STATE_WEIGHT_LOADFIX,  // 7 完成一次稱重測量后重量穩定沒有解鎖
    STATE_WEIGHT_LOADDOWN, // 8 下秤動作
    STATE_WEIGHT_OVERLOAD, // 9 超重,當前重量大於最大稱重重量
    // 自動上稱
    ENTER_WEIGHT_AUTOON,   // 10 進入自動上稱模式
    STATE_AUTOON_FASTMODE, // 11 快速ADC自動上稱判斷
    STATE_AUTOON_SLOWMODE, // 12 慢速ADC自動上稱判斷
    STATE_AUTOON_PASS,     // 13 自動上稱OK, 此時可以點亮顯示,并切換到普通稱重模式進行MCU的正常工作
    STATE_AUTOON_FAIL,     // 14 自動上秤失敗,此時請繼續讓 MCU 進入HALT狀態
    // 重量校準標定
    ENTER_WEIGHT_CAL,      // 15 進入重量校準模式
    STATE_WEIGHT_CAL0,     // 16 正在標定零點
    STATE_WEIGHT_CAL1,     // 17 正在標定第1點
    STATE_WEIGHT_CAL2,     // 18 正在標定第2點
    STATE_WEIGHT_CAL3,     // 19 正在標定第3點
    STATE_WEIGHT_CALCHECK, // 20 標定判斷
           STATE_WEIGHT_CALPASS,  // 21 標定成功
    STATE_WEIGHT_CALFAIL,  // 22 標定失敗
    // 阻抗量測
    ENTER_IMPEDANCE,            // 23 進入阻抗測量模式
    STATE_IMPEDANCE_REFERENCE1, // 24 正在量測參考電阻1
    STATE_IMPEDANCE_REFERENCE2, // 25 正在量測參考電阻2
    STATE_IMPEDANCE_CHECKBODY,  // 26 判斷是否有人體
    STATE_IMPEDANCE_RX,         // 27 正在量測待測阻抗
    STATE_IMPEDANCE_FINISH,     // 28 阻抗量測結束,此時可以讀取gu16v_CurrentImpedance_ohm
    // 溫度量測,若lib名字包含 Temp有效
    ENTER_TEMP,     // 29 温度測量
    STATE_TEMP_CAL, // 30 温度校準
    // 心率測量,若lib名字包含 Heat有效
    ENTER_HEATRATE,         // 31 進入心率測量
    STATE_HEATRATE_PREPARE, // 32 心率測量準備中
    STATE_HEATRATE_UPDATE   // 33 心率測量OK,此時可以讀取數據
} BHSDKState_t;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ======================================================@
//              SDK接口  ADC採樣接口                      @
// ======================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
typedef struct
{
	unsigned char ByteHigh;		// R ADC 原始數據高位
	unsigned char ByteMid;		// R ADC 原始數據中位
	unsigned char ByteLow;		// R ADC 原始數據低位
	unsigned char SamplingCnt;	// R ADC 採集次數計數
	union {
		struct
		{
			unsigned char IsReady : 1;  // R 有一筆原始ADC數據
		} b;
		unsigned char byte;
	} flag;
} ADCSource_t;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ======================================================@
//              SDK接口  ADC濾波接口                      @
// ======================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
typedef struct
{
	unsigned long Current;			// R 當前濾波ADC數據
	unsigned long BufMax;			// R 濾波 buf 中最大的ADC值,如果為0x00000000,說明跳動值超過設置濾波範圍
	unsigned long BufMin;			// R 濾波 buf 中最小的ADC值,如果為0xFFFFFFFF,說明跳動值超過設置濾波範圍
	unsigned int StableThreshold;   // RW 目標穩度閥值,用戶在SDK_UserSetting.h設定值
	unsigned char StableCntTag;		// RW 目標穩度次數,用戶在SDK_UserSetting.h設定值
	unsigned char StableCntCurrent; // R 當前穩度次數
	union {
		struct
		{
			unsigned char IsReady : 1;  // R 有一筆濾波ADC數據
			unsigned char IsStable : 1; // R 濾波ADC數據處於穩定狀態
		} b;
		unsigned char byte;
	} flag;
} ADCFilter_t;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ======================================================@
//              SDK接口  Weight接口                       @
// ======================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
typedef struct
{
    struct
    {
        unsigned long Cal0;	    // R    ADC值 - 零點重量
        unsigned long Cal1;	    // R    ADC值 - 第1個標定重量
        unsigned long Cal2;	    // R    ADC值 - 第2個標定重量
        unsigned long Cal3;	    // R    ADC值 - 第3個標定重量
    }CalADCData;// 校準數據
    struct
    {
        unsigned long Cal0;	    // R    ADC值 - 零點重量
        unsigned long Cal1;	    // R    ADC值 - 第1個標定重量
        unsigned long Cal2;	    // R    ADC值 - 第2個標定重量
        unsigned long Cal3;	    // R    ADC值 - 第3個標定重量
    }CalADCDataTemp; // 校準中數據,for debug
    unsigned char Span;	        // R    ADC值 - 1個分辨率
	union {
		struct
		{
			unsigned char IsNeedTare : 1;  // RW 去皮,當前重量為穩定重量時當前重量為0
		} b;
		unsigned char byte;
	} flag;
    unsigned int  DataCurrent;  // R    当前重量值,單位為jin,放大10倍, 若500,表示重量為50.0斤
    unsigned int  DataStable;   // R    穩定重量值,單位為jin,放大10倍, 若500,表示重量為50.0斤
}Weight_t;

typedef struct
{
    unsigned long WeightMax;          // RW 最大測量重量
    unsigned int WeightMin;           // RW 最小測量重量
    unsigned int WeightAutoOn;        // RW 自动唤醒重量
    unsigned int WeightUnLockTemp;    // RW 自動解鎖重量-臨時
    unsigned char WeightUnLockStable; // RW 自動解鎖重量-穩定
    union {
        struct
        {
            unsigned char IsAutoCalOn : 1;    // 自動校準开启
            unsigned char IsAutoUnlockOn : 1; // 自動解鎖开启
        } b;
        unsigned char byte;
    } flag;
} BodyScalesSetting_t;
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ======================================================@
//              SDK接口  體阻抗接口                       @
// ======================================================@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

typedef struct
{
    struct
    {
        unsigned long Cal0;	    // R    ADC值 - 參考電阻1
        unsigned long Cal1;	    // R    ADC值 - 參考電阻2
        unsigned long CalRx0;	// R    ADC值 - 參考電阻1並聯Rx
        unsigned long CalRx;	// R    ADC值 - 待測電阻Rx
    }CalADC;
    union {
        unsigned char byte;
    } flag;
    unsigned int Data;           // R    阻抗 - 阻抗值,單位ohm. 若為(0xffff-無待測阻抗錯誤)/(0xFFF1-接觸異常錯誤)/(0xFFF2-阻抗超出範圍)
} Impedance_t;

#endif
