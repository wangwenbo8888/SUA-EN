#ifndef VARIABLE
#define VARIABLE

#define SERVICE_VERSION  0
// draw sence 形状
#define RECTANGLE        0
#define ELLIPSE          1

// 停止的时候超声探头停在-90度
#define END_ANGLE        -90.0

enum PackageFlag {
    // Header
    // Client to server
    INQUIRY = 10,
    PLAN = 20,
    SONICATION = 30,
    // Server to client
    FEEDBACK = 40,
    // Server to client: used for EMC test
    TEST = 50,
    // Terminator

    // Hifu water status quary start
    // wangwenbo 20160824
    TEMP1 = 60,
    TEMP2 = 61,
    LEVEL = 62,
    // Hifu water status quary end
    SERVERFLAG = 241,
    TERMINATOR = 99,
    HEARTRECE = 254,
    HEARTBEAT = 255
};

enum DataFlag {
    // Inquiry from client to server: network
    NETWORK = 100,
    // Results feedbacked from server to client: network
    NETWORK_CONNECTED,
    NETWORK_DISCONNECTED,
    // Inquiry from client to server: power amplifiers
    POWER_AMPILIFIERS = 110,
    // Results feedbacked from server to client: power amplifiers
    POWER_AMPLIFIERS_AVAILABLE,
    POWER_AMPLIFIERS_NONAVAILABLE,
    // Inquiry from client to server: power amplifiers voltage
    POWER_AMPLIFIERS_VOLTAGE,
    // Inquiry from client to server: power amplifiers tempature
    POWER_AMPLIFIERS_TEMPATURE,

    // Inquiry from client to server: phase controller
    PHASE_CONTROLLER = 120,
    // Results feedbacked from server to client: phase controller
    PHASE_CONTROLLER_AVAILABLE,
    PHASE_CONTROLLER_NONAVAILABLE,
    // Inquiry from client to server: hifu driver
    HIFU_DRIVER = 130,
    // Results feedbacked from server to client: hifu driver
    HIFU_DRIVER_AVAILABLE,
    HIFU_DRIVER_NONAVAILABLE,
    // Inquiry from client: image grabber
    IMAGE_GRABBER = 140,
    // Results feedbacked from client to client: image grabber
    IMAGE_GRABBER_AVAILABLE,
    IMAGE_GRABBER_NONAVAILABLE,
    // Inquiry from client: water cycle
    WATER_CYCLE = 150,
    // Results feedbacked from client to client: water cycle
    WATER_CYCLE_AVAILABLE,
    WATER_CYCLE_NONAVAILABLE,
    WATER_CYCLE_LEVEL_FULL,
    WATER_CYCLE_LEVEL_HALF,
    WATER_CYCLE_LEVEL_NO,
    // Inquiry from client: location
    POSITION = 160,
    // Results feedbacked from client to client: location
    POSITION_AVAILABLE,
    POSITION_NONAVAILABLE,

    // Hifu water status quary start
    // wangwenbo 20160824
    WATER_TEMP_AVAILABLE=180,  // 水温在0到100度之间
    WATER_TEMP_INNER_ERROR,    // 内部错误
    WATER_TEMP_OVER_100,       // 水温超过100
    WATER_TEMP_BELOW_0,        // 水温低于0
    WATER_TEMP_NO_THERMOMETER, // 没有水温计

    WATER_LEVEL_EMPTY,         // 无水
    WATER_LEVEL_AVALIABLE,     // 有水
    WATER_LEVEL_FULL,          // 满水
    WATER_LEVEL_INNER_ERROR,   // 内部错误
    WATER_LEVEL_NO_GAUGE,      // 无水位计
    // Hifu water status quary end

    // append begin
    // include indicator light and foot switch
    APPEND = 200,
    APPEND_AVAILABLE,
    APPEND_UNAVAILABLE,
    // append end

    // Sonication feedbacked from server to client: plan
    PLAN_READY = 300,
    PLAN_NOT_READY,
    // Action from client to server: sonication
    SONICATION_GET_READY = 310,
    SONICATION_START,
    SONICATION_STOP,

    SONICATION_MUTI_SET_PARA,    // 设置多点同时辐照参数
    SONICATION_MUTI_START,       // 设置多点同时辐照开始
    SONICATION_MUTI_STOP,        // 设置多点同时辐照停止
    // Sonication feedbacked from server to client: power amplifiers
    POWER_AMPLIFIERS_READY = 320,
    POWER_AMPLIFIERS_NOT_READY,
    // Sonication feedbacked from server to client: sonication status
    SONICATION_STARTED = 330,
    SONICATION_STOPPED,
    // Feedback from server to client
    SPOT_COMPLETED = 400,
    PLAN_COMPLETED,
    // Foot switch status 导致的phase control enable,disable
    PHASE_CONTROLLER_DISABLE = 500,
    PHASE_CONTROLLER_ENABLE,
    // phase control feedback
    PHASE_CONTROLLER_DISABLED = 510,
    PHASE_CONTROLLER_ENABLED,
    // 返回功放板电压故障
    POWER_VOLTAGE_ERROR = 103,
//    POWER_VOLTAGE_SUCCESS = 601,
};

//  Location

//定义错误码
enum LocErrorCode{
    //以下为UIM初始化时错误码
    NoError             ,       //没有错误
    LoadDllError	=2,     //装载DLL库出错
    SearchGatewayError	,       //执行DLL库函数SearchGateway出错
    NoGatewayError	,       //没有发现网关(Gateway)
    MoreGatewayError	,       //发现1个以上Gateway
    OpenGatewayError    ,       //执行DLL库函数OpenGateway出错
    NoSiteError         ,       //没有发现站点(Site)
    SiteNumberError	,       //Site数量不正确
    FitSiteError	,       //和已知的Site不匹配
    AdjRotateError	,	//校正旋转探头错误

    AxisError           ,       //不正确的运动轴
    LockedRotor         ,       //堵转
    BelowRange          ,
    OverRange           ,

    //以下为执行DLL库函数时错误码
    SetSPDError         =80,    //执行DLL库函数SetSPD出错
    SetSTPError         ,       //执行DLL库函数SetSTP出错
    SetQECError         ,       //执行DLL库函数SetQEC出错
    UimENAError         ,       //执行DLL库函数UimENA出错
    GetQECError         ,       //执行DLL库函数GetQEC出错
    SetDOUTError        ,       //执行DLL库函数SetDOUT出错
    SetORGError                 //执行DLL库函数SetORG出错

};

//高强度超声治疗系统的子系统
enum SubSys
{
    SUPER_B = 0,    // B超
    WATER_CIRCLE,   // 水循环
    MECHINE_MOVE,   // 机械移动
    POWER_AMP,      // 超声驱动
    NET_WORK,       //网络
};

//自检的模块子系统
enum selfSubSys
{
    iappned = 0,    // 控制盒
    ilocation,   // 机械移动
    iwarter,   // 水循环
    inetwork,      // 网络
    iimagegraber,   //超声图像
    ipower        //电压
};


#endif // VARIABLE

