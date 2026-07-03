#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QLoggingCategory>

#include "windows.h"
#include "uiSimCanFunc_RT.h"
#include "location_global.h"
#include "constant.h"
#include "variable.h"
#include "QTime"
#include <QCoreApplication>
#ifndef DEBUG
#include "macro.h"
#endif
#define TIM_OUT 1000
#define TIM_OUT_Z 800   //Z轴校零等待时间
#define TIM_OUT_A 300  //A轴等待触发S1时间
Q_DECLARE_LOGGING_CATEGORY(LOCATION)

class Location;
static Location *_pSLocation;

class LOCATIONSHARED_EXPORT Location : public QObject
{
    Q_OBJECT
public:
    explicit Location(QObject* parent = 0);
    ~Location();

    static void __cdecl StaticRevRtcnNotify(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen)
    {
        _pSLocation->RevRtcnNotify(dwDevIndex, pRtcnMsgObj, dwMsgLen);
    }

    bool initialize();
    bool zero(AID);
    bool setPos(AID, APOS);
    void getPos(AID);
    bool setAng(APOS);
    void getAng();
    void reset();

    APOS m_iZ, m_iL, m_iF;                      //Z/L/F axis coordinate
    APOS m_iZ_REC, m_iZ_Delta;
    APOS m_iA;                                  //A axis coordinate
    DWORD m_SiteID[MAX_SITE];                   //Site ID
    DEV_INFO_OBJ m_GatewayInfo[MAX_GATEWAY];    //Gateway Info
    P_MDL_INFO_OBJ p_SiteInfo = new MDL_INFO_OBJ[SITE_NUMBER];    //Site Info
    P_S34CON p_S34CON = new S34CON[AXIS_NUMBER];

    static DWORD dwDevIndex;
    static P_CAN_MSG_OBJ pRtcnMsgObj;
    static DWORD dwMsgLen;
    static CAN_MSG_OBJ RtcnMsgObj;

signals:
    void posDat(AID, APOS);
    void error(ERRORTYPE);
    // TODO: consistent with other modules
     void statusUpdated(DataFlag);
     void HandleRtcnNotify(uint dwCanNodeId, uint iRCW, uint iData);
     void LocationErr();
     void SetPosSuccess();

public slots:
    void RevRtcnNotify(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen);
    void AsyncRevRtcnNotify(uint dwCanNodeId, uint iRCW, uint iData);

    void ReceiveHeadControl(int AID,int direction);

private:
    int initUIM();

    void timerEvent(QTimerEvent *event);
    void emitError(ERRORTYPE ErrCode);
    void emitWarning(ERRORTYPE ErrCode);

    void Delay_MSec(unsigned int msec);

    BOOL m_bDevCnectFlg;
    int m_iSPD;
    int m_iSTP;
    int m_iPOS;
    int m_iQEC;
    BOOL bKeyPrs[KEY_NUMBER];
    BOOL bKeyEn[KEY_NUMBER];
    BOOL bAxisOn[AXIS_NUMBER];
    BOOL bUIDon;
    int m_Speed[AXIS_NUMBER];
    int m_Rate[AXIS_NUMBER];
    //int m_Range[AXIS_NUMBER][2];
    BOOL b_TimerR[KEY_NUMBER];
    int m_TimerR[KEY_NUMBER];
    int m_Range[AXIS_NUMBER][2];
    BOOL b_Adj[AXIS_NUMBER];
    int m_Zero[AXIS_NUMBER];
    int m_AdjPos[AXIS_NUMBER];
    UINT m_uiDevIndex;
    UINT m_uiSiteID;
    int Port,Baund;
    bool falling_edge_flag;
    bool speedmotion_flag;
    bool zero_flag = false;

    int m_iModeZ, m_DeltaZ_Min, m_DeltaZ_Max;

    int m_FlagAdj[4][AXIS_NUMBER]; //axis to displacement/S1/S2/S3

    enum FlagType{
        FDIST=0,    //displacement
        FS1,        //S1
        FS2,        //S2
        FS3         //S3
    };

    //定义电机(轴)工作模式
    enum AxisMode{
        IDLE,                 //空闲模式
        SPEED,                //速度模式
        STEP                  //位移模式
    };
    AxisMode m_ModeA[AXIS_NUMBER];

    LocErrorCode m_errCode;

    void Wait(int msecond);
    bool WaitByFlag(int iTimeOut, int iExpectFlag,FlagType flag,int Axis);
    bool WaitByFlagS2(int iTimeOut, int iAdjIndex, int iExpectFlag);
    //自定义运动
    bool SetSpeedMotion(UINT DevIndex,UINT SiteID,int speed);
    bool SetUimEnable(UINT DevIndex,UINT SiteID,bool enable);
    bool SetTTLOutput(UINT DevIndex,UINT SiteID,bool bOut);
    bool SetOrigin(UINT DevIndex,UINT SiteID);
    bool SetStep(UINT DevIndex,UINT SiteID,int iStep);
    bool SetSpeedMotionByKey(UINT iKey,bool bKeyPress);
    bool SetStop(UINT DevIndex,UINT SiteID);
};

#endif // LOCATION_H
