#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QLoggingCategory>

#include "windows.h"
#include "uiSimCanFunc_RT.h"
#include "location_global.h"
#include "constant.h"
#include "variable.h"
#include "macro.h"

Q_DECLARE_LOGGING_CATEGORY(LOCATION)

class LOCATIONSHARED_EXPORT Location : public QObject
{
    Q_OBJECT
public:
    explicit Location(QObject* parent = 0);
    ~Location();

    bool initialize();
    bool setPos(AID, APOS);
    void getPos(AID);
    bool setAng(APOS);
    void getAng();
    void reset();

    APOS m_iZ, m_iL, m_iF;                      //Z/L/F axis coordinate
    APOS m_iA;                                  //A axis coordinate
    DWORD m_SiteID[MAX_SITE];                   //Site ID
    DEV_INFO_OBJ m_GatewayInfo[MAX_GATEWAY];    //Gateway Info
    P_MDL_INFO_OBJ p_SiteInfo = new MDL_INFO_OBJ[SITE_NUMBER];    //Site Info

    static DWORD dwDevIndex;
    static P_CAN_MSG_OBJ pRtcnMsgObj;
    static DWORD dwMsgLen;
    static CAN_MSG_OBJ RtcnMsgObj;

signals:
    void RtcnDat(uint dwCanNodeId, uint iRCW, uint iData);
    void posDat(AID, APOS);
    void error(ERRORTYPE);

public slots:
    void RevRtcnNotify(uint dwCanNodeId, uint iRCW, uint iData);

private:
    int initUIM();
    void TmpRevRtcnNotify(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen);
    static void __cdecl StaticRevRtcnNotify(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen);
    void timerEvent(QTimerEvent *event);
    void emitError(ERRORTYPE ErrCode);
    void emitWarning(ERRORTYPE ErrCode);

    BOOL m_bDevCnectFlg;
    int m_iSPD;
    int m_iSTP;
    int m_iPOS;
    int m_iQEC;
    BOOL bKeyPrs[KEY_NUMBER];
    int m_TimerK[KEY_NUMBER];
    BOOL bKeyEn[KEY_NUMBER];
    BOOL bAxisOn[AXIS_NUMBER];
    BOOL bUIDon;
    int m_TimerA[AXIS_NUMBER];
    int m_Speed[AXIS_NUMBER];
    int m_Rate[AXIS_NUMBER];
    int m_Min[AXIS_NUMBER];
    int m_Max[AXIS_NUMBER];
    UINT m_uiDevIndex;
    UINT m_uiSiteID;

    //定义电机(轴)工作模式
    enum AxisMode{
        IDLE,                 //空闲模式
        SPEED,                //速度模式
        STEP                  //位移模式
    };
    AxisMode m_ModeA[AXIS_NUMBER];

    ErrorCode m_errCode;
};

#endif // LOCATION_H
