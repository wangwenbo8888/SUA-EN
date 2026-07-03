#ifndef VARIABLE
#define VARIABLE

#include "macro.h"

enum cmdType
{
    START = 1,
    STOP,
    PAUSE,
    RESUME
};

typedef struct Plane2DCoordinate
{
    Coordinate x;
    Coordinate y;
}_2DCor;

typedef struct Spot3DCoordinate
{
    Coordinate x;
    Coordinate y;
    Coordinate z;
}_3DCor;

typedef struct SpotSonicationParameter
{
    VOLT volt;
    int totalTime;
    int period;
    int dutyCycle;
    int coolingTime;
}_SoniParam;

typedef struct SessionRecorder
{
    int spotIndex;
    int periodIndex;
}_SesRec;

typedef struct SessionParameter
{
    int spotCount;
    int periodCount;
    int dutyOn;
    int dutyOff;
    int coolingTime;
}_SesParam;

//  HIFULocation

//定义错误码
enum ErrorCode{
    //以下为UIM初始化时错误码
    NoError             ,       //没有错误
    LoadDllError	=2,     //装载DLL库出错
    NoGatewayError	,       //没有发现网关(Gateway)
    MoreGatewayError	,       //发现1个以上Gateway
    OpenGatewayError    ,       //打开Gateway出错
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

//  HIFULocation

#endif // VARIABLE

