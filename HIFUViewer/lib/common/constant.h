#ifndef CONSTANT
#define CONSTANT

//  SONICATION PARAMETERS
#define VOLTAGE 14
#define MS_UNIT 1000
#define PERCENT_UNIT 100
#define TEST_SPOT_COUNT 20
//  FINISH

//  DOCONTROLLER PARAMETERS
#define DEVICE_ID "USB-4751,BID#0"
#define SONICATIONTIME_LL 6
#define SONICATIONTIME_UL 20
#define SONICATIONPERIOD_LL 1
#define SONICATIONPERIOD_UL 1000
#define DUTYCYCLE_LL 1
#define DUTYCYCLE_UL 100
#define COOLINGTIME_LL 1
#define COOLINGTIME_UL 500
#define SONICATIONTIME_DEFAULT 15
#define SONICATIONPERIOD_DEFAULT 500
#define DUTYCYCLE_DEFAULT 100
#define COOLINGTIME_DEFAULT 300
#define PORT_CHANNEL 4
#define PORT_PHASE 5
#define PORT_LOAD 3
#define PORT_ENABLE 3
#define PORT_DISABLE 3
#define BYTE_LOAD 0x80
#define BYTE_LOCK 0x00
#define BYTE_ENABLE 0x40
#define BYTE_DISABLE 0x00
#define TRANSDUCER_COUNT 144
//  FINISH

//  PA PARAMETERS
#define DEV_TEST_COUNT 2
#define SAFE_COUNTER 5
#define DEV_COUNT_MAX 144
#define VOLT_MAX 18
#define TEST_CHANNEL 15
#define ECHO_PERIOD 50
//  FINISH

//  HIFULocation

#define     MAX_GATEWAY     2           //最大网关数
#define     MAX_SITE        16          //最大站点数
#define     SITE_NUMBER     5           //当前项目站点数
#define     AXIS_NUMBER     4           //当前项目电机(轴)数
#define     KEY_NUMBER      6           //当前项目按键数
#define     UIM_ID          10          //UIM(轴)起始ID
#define     AXIS_Z          0           //Z轴,上下,开环
#define     AXIS_L          1           //L轴,左右,闭环
#define     AXIS_F          2           //F轴,前后,闭环
#define     AXIS_A          3           //A轴,角度,闭环

#define     UID_ID          18          //UID(8路IO)之ID
#define     RATE_Z          6400        //Z轴比率:脉冲/mm
#define     RATE_L          1000        //L轴比率:脉冲/°
#define     RATE_F          1000        //F轴比率:脉冲/°
#define     RATE_A          500         //A轴比率:脉冲/°

#define     SPEED_Z         24000       //Z轴速度
#define     SPEED_L         5000        //L轴速度
#define     SPEED_F         5000        //F轴速度
#define     SPEED_A         10000       //A轴速度
#define     TIM_KEY         50          // 按键消抖
#define     TIM_POS         600         // 延迟后获取位置

#define     MAX_Z           380		// Z轴最大值(mm)
#define     MIN_Z           -20		// Z轴最小值(mm)
#define     MAX_L           15		// L轴最大值(°)
#define     MIN_L           -15		// L轴最小值(°)
#define     MAX_F           15		// F轴最大值(°)
#define     MIN_F           -15		// F轴最小值(°)
#define     MAX_A           90		// A轴最大值(°)
#define     MIN_A           -90		// A轴最小值(°)

//  HIFULocation

#endif // CONSTANT

