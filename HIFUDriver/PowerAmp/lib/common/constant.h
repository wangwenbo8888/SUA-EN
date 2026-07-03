#ifndef CONSTANT
#define CONSTANT

//  Debug
#define DEBUG 1
//#define NO_HARDWARE

//  Power Amplifiers
enum Circle
{
    Circle_center = 0,    // 中点
    Circle_one,         // 内圈
    Circle_two,        // 中圈
    Circle_three,         // 外圈
    Circle_four,
    Circle_five,

    Circle_end
};

#define     VOLTAGE_MAXIMUM     20
#define     AMP_COUNT           144
#define     TEST_CHANNEL_COUNT  2
#define     WAITING_PERIOD      50  // unit: ms
#define     SERIAL_DELAY        3
#define     BYTE_RETURNED_COUNT 5
#define     FAIL_THRESHOLD      20
#define     ERROR_IMAGE_INDEX   -100

//  Phase Controller

#define     DIO_NAME            "USB-4751,BID#0"
#define     PORT_CHANNEL        4
#define     PORT_PHASE          5
#define     PORT_WORK           3
#define     BYTE_LOAD           0x80
#define     BYTE_LOCK           0x00
#define     BYTE_ENABLE         0x40
#define     BYTE_DISABLE        0x00
#define     BYTE_PHASE_DISABLE  0x20
#define     BYTE_PHASE_ENABLE   0x00

// Indicator light begin

#define     APPEND_NAME         "USB-4704,BID#0"
#define     RED_ON              0x08
#define     RED_OFF             0xF7
#define     YELLOW_ON           0x04
#define     YELLOW_OFF          0xFB
#define     GREEN_ON            0x02
#define     GREEN_OFF           0xFD
#define     LIGHTS_OFF          0x00

// Indicator light end

//  Session

#define     MILLISECOND_UNIT    1000
#define     PERCENT_UNIT        100

//  Network
#define SEND_SAFE_COUNTER 5

//  Location

#define     MAX_GATEWAY         2           //最大网关数
#define     MAX_SITE            16          //最大站点数
#define     SITE_NUMBER         5           //当前项目站点数
#define     AXIS_NUMBER         4           //当前项目电机(轴)数
#define     KEY_NUMBER          6           //当前项目按键数
#define     UIM_ID              10          //UIM(轴)起始ID
#define     UID_ID              18          //UID(8路IO)之ID

#define     AXIS_Z              0           //Z轴,上下,开环
#define     AXIS_L              1           //L轴,左右,闭环
#define     AXIS_F              2           //F轴,前后,闭环
#define     AXIS_A              3           //A轴,角度,闭环

#define     RATE_Z              6400        //Z轴比率:脉冲/mm
#define     RATE_L              1000        //L轴比率:脉冲/°
#define     RATE_F              1000        //F轴比率:脉冲/°
#define     RATE_A              500         //A轴比率:脉冲/°

// #define     SPEED_Z             10000       //Z轴速度
#define     SPEED_Z             24000       //Z轴速度
#define     SPEED_L             5000        //L轴速度
#define     SPEED_F             5000        //F轴速度
// #define     SPEED_A             10000       //A轴速度
#define     SPEED_A             10000       //A轴速度
#define     TIM_KEY             10          // 按键消抖
#define     TIM_POS             50          // 延迟后获取位置
#define     TIM_RANGE           2000        // 边界延迟

#define     MAX_Z               380         // Z轴最大值(mm)
#define     MIN_Z               -20         // Z轴最小值(mm)
#define     MAX_L               15          // L轴最大值(°)
#define     MIN_L               -15         // L轴最小值(°)
#define     MAX_F               15          // F轴最大值(°)
#define     MIN_F               -15         // F轴最小值(°)
#define     MAX_A               90          // A轴最大值(°)
#define     MIN_A               -90         // A轴最小值(°)

#define     ADJ_Z               0           // Z轴校正值(mm)
#define     ADJ_L               5           // L轴校正值(°)
#define     ADJ_F               5           // F轴校正值(°)
#define     ADJ_A               30          // A轴校正值(°)

typedef     unsigned int    UINT;
typedef     unsigned int    ERRORTYPE;
typedef     QString         ERRORMESSAGE;
typedef     unsigned int    AID;
typedef     float           APOS;

//  WATER CYCLE
#define BYTE_CHECK_DEVICE 0xF7
#define BYTE_DEVICE_ID 0xA5

#define BYTE_OPEN_PUMP 0xD1
#define BYTE_CLOSE_PUMP 0xD0

#define BYTE_OPEN_RELAY 0xD8DA

#define BYTE_CHECK_CONNECTION 0xFC
#define BYTE_RETURN_CONNECTION 0xF4
#define BYTE_ENABLE_DEVICE 0xF9

#define BYTE_QUERY_STATUS 0xFB
#define BYTE_WATER_LOW 0x70
#define BYTE_WATER_HIGH 0x71
#define BYTE_WATER_FULL 0x72

// 液位查询 王文波添加 20160824 start
#define BYTE_LEVEL_INNER_ERROR  0x7C      // 内部错误
#define BYTE_LEVEL_NO_GAGUAE    0x7F      // 无水位计
// 液位查询 end

// 查询温度计温度返回值定义 start
// 王文波 20160824
#define BYTE_TEMP_MIN            0x00     // 0度
#define BYTE_TEMP_MAX            0x64     // 100度
#define BYTE_TEMP_INNER_ERROR    0x6C     // 温度内部错误
#define BYTE_TEMP_OVER_100       0x6D     // 温度超过100度
#define BYTE_TEMP_LESS_0         0x6E     // 温度低于0度
#define BYTE_TEMP_NO_THETMETER   0x6F     // 无温度计
// 查询温度计温度状态返回值定义 end

#define BYTE_ECHO 0xF5
//  FINISH

//  Configuration

#define SETTINGS_PATH   "../../lib/config/config.ini"
#define DQA_JSON_PATH   "../../lib/config/dqa.json"
#define ICON_PIC_PATH   "../../lib/config/icon/"
#define TREATMENT_JSON  "../../lib/config/plan/plan.json"
#define LOG_PATH        "../../lib/config/treatment.log"

#define PLAN_FOLD       "../../lib/config/plan/"

#define CLIP_WIDE 200       // 重建和三维处理时裁剪的图像宽度
#define CLIP_HIGH 200       // 重建和三维处理时裁剪的图像高度
#define HALF_CLIP_WIDE 100  // 裁剪图像中心
#define HALF_CLIP_HIGH 100  // 裁剪图像体中心



// 输出log所在行和列信息
#define TRACE_CMH(fmt,...)\
    ("%s:",__FILE__)<<("%d:",__LINE__)<<("%s:",__FUNCTION__)

#endif // CONSTANT

