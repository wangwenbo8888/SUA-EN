#include <QTimerEvent>
#include <QSettings>
#include <QEventLoop>
#include <QThread>

#include <QTimer>
#include <QDebug>

#include "location.h"
#include "tchar.h"
#include "LoadSimCanFunc.cpp"

static Location* ptrLocation;
Q_LOGGING_CATEGORY(LOCATION,"LOCATION")

Location::Location(QObject *parent) : QObject(parent)
{
    ptrLocation = this;
    connect(ptrLocation,SIGNAL(RtcnDat(uint, uint, uint)),
            ptrLocation,SLOT(RevRtcnNotify(uint, uint, uint)));
}

Location::~Location()
{
    if (m_bDevCnectFlg)
    {
        while(true)
        {
            if (CloseGateway(m_uiDevIndex) == 1)
            {
                break;
            }
        }
        m_bDevCnectFlg = false;
    }
}

bool Location::initialize()
{
    qCDebug(LOCATION()) << "Initialize...";

    m_bDevCnectFlg=false;
    for(int i=0;i<AXIS_NUMBER;i++)
    {
        m_TimerA[i]=0;
        bAxisOn[i]=FALSE;
        m_ModeA[i]=IDLE;
    }
    bUIDon = FALSE;
    for(int i=0;i<KEY_NUMBER;i++)
    {
        m_TimerK[i]=0;
        bKeyEn[i]=FALSE;
        b_TimerR[i]=FALSE;
    }

    m_Speed[0]=SPEED_Z;
    // m_Speed[0]=10000;
    m_Speed[1]=SPEED_L;
    m_Speed[2]=SPEED_F;
    m_Speed[3]=SPEED_A;
    m_Rate[0]=RATE_Z;
    m_Rate[1]=RATE_L;
    m_Rate[2]=RATE_F;
    m_Rate[3]=RATE_A;
    m_Range[0][0]=MIN_Z;
    m_Range[0][1]=MAX_Z;
    m_Range[1][0]=MIN_L;
    m_Range[1][1]=MAX_L;
    m_Range[2][0]=MIN_F;
    m_Range[2][1]=MAX_F;
    m_Range[3][0]=MIN_A;
    m_Range[3][1]=MAX_A;

    m_AdjPos[0]=ADJ_Z;
    m_AdjPos[1]=ADJ_L;
    m_AdjPos[2]=ADJ_F;
    m_AdjPos[3]=ADJ_A;
    QSettings *configIniRead = new QSettings(SETTINGS_PATH, QSettings::IniFormat);
    m_Zero[0]=configIniRead->value("Location/Zero_Z").toInt();
    m_Zero[1]=configIniRead->value("Location/Zero_L").toInt();
    m_Zero[2]=configIniRead->value("Location/Zero_F").toInt();
    m_Zero[3]=configIniRead->value("Location/Zero_A").toInt();
    delete configIniRead;
//    printf("Z=%d,L=%d,F=%d,A=%d",m_Zero[0],m_Zero[1],m_Zero[2],m_Zero[3]);
//    m_Zero[1]=-1727;
//    m_Zero[2]=-853;
//    m_Zero[3]=-2535;

    ERRORTYPE RctnErr=initUIM();

    if (RctnErr != NoError)
    {
        emit statusUpdated(POSITION_NONAVAILABLE);
        emitError(RctnErr);
        return false;
    } else
         emit statusUpdated(POSITION_AVAILABLE);

    qCDebug(LOCATION()) << "Initialized successfully.";
//        qDebug("UIM Gateway=");
//        qDebug("UIM SiteNumber=5");
    return true;
}

int Location::initUIM()
{
    // 载入动态库
    if (!LoadSimCanFunc()) return LoadDllError;

    //查找网关
    DWORD m_ComNum = SearchGateway(UIDEV_ALL, NULL, m_GatewayInfo, MAX_GATEWAY);
    if (m_ComNum == 0)
        return NoGatewayError;
    else if (m_ComNum != 1)
        return MoreGatewayError;
    m_uiDevIndex = m_GatewayInfo[0].dwDevIndex;

   //打开网关
   DWORD dwCanBtr;
   DWORD m_dwSubCanNodeLen = OpenGateway(m_uiDevIndex, m_SiteID, 120, &dwCanBtr);
   if (m_dwSubCanNodeLen == -1)
   {
       return OpenGatewayError;
   }
   else if (m_dwSubCanNodeLen == 0)
       return NoSiteError;
   //else if (m_dwSubCanNodeLen != SITE_NUMBER)
   //    return SiteNumberError;
   m_bDevCnectFlg = TRUE;

   for (uint i = 0; i < m_dwSubCanNodeLen; i++)
   {
       //获取站点信息
       GetMDL(m_uiDevIndex, m_SiteID[i], &(p_SiteInfo[i]));

       //判断是否已知的ID
       m_uiSiteID = m_SiteID[i];
       switch(m_uiSiteID)
       {
       case UIM_ID+AXIS_Z:
       {
           bAxisOn[AXIS_Z] = TRUE;
           getPos(AXIS_Z);
           SetDOUT(m_uiDevIndex, m_uiSiteID,1,false,NULL);      //设置Z轴P4输出5V
           break;
       }
       case UIM_ID+AXIS_L:
       {
           bAxisOn[AXIS_L] = TRUE;
           getPos(AXIS_L);
           break;
       }
       case UIM_ID+AXIS_F:
       {
           bAxisOn[AXIS_F] = TRUE;
           getPos(AXIS_F);
           break;
       }
       case UIM_ID+AXIS_A:
       {
           bAxisOn[AXIS_A] = TRUE;
           getPos(AXIS_A);
           break;
       }
       case UID_ID:
       {
           bUIDon = TRUE;
           //设置UID为输入
           P_UID_MCFG_INFO_OBJ RtnValue = new UID_MCFG_INFO_OBJ, pUIDMCFGInfoObj = new UID_MCFG_INFO_OBJ;
           pUIDMCFGInfoObj->bP1IE = 1;
           pUIDMCFGInfoObj->bP2IE = 1;
           pUIDMCFGInfoObj->bP3IE = 1;
           pUIDMCFGInfoObj->bP4IE = 1;
           pUIDMCFGInfoObj->bP5IE = 1;
           pUIDMCFGInfoObj->bP6IE = 1;
           bKeyEn[0]=TRUE;
           bKeyEn[1]=TRUE;
           bKeyEn[2]=TRUE;
           bKeyEn[3]=TRUE;
           bKeyEn[4]=TRUE;
           bKeyEn[5]=TRUE;
           SetUIDMCFG(m_uiDevIndex, m_uiSiteID, pUIDMCFGInfoObj, TRUE, RtnValue);
           break;
       }
       default:
           break;
       }
   }
   if (bAxisOn[0] && bAxisOn[1] && bAxisOn[2] && bAxisOn[3] && bUIDon)
   {
       //设置回调函数
       UIMRegRtcnCallBack(0, m_uiDevIndex, Location::StaticRevRtcnNotify);
   }else
       return FitSiteError;
   // if (!zero(AXIS_A)) {return AdjRotateError;}
   BASIC_ACK_OBJ RtnValue = { 0 };
   for (int i = 0; i < 3;++i)
   {
       m_uiSiteID = UIM_ID+i;
       if (-1 == UimOff(m_uiDevIndex, m_uiSiteID, TRUE, &RtnValue)) emitError(UimENAError);
   }
   return NoError;
}

bool Location::zero(AID AxisID)
{
    if (AxisID==AXIS_Z) return false;
    int TimOut,Step;
    Step=0;
    b_Adj[AxisID]=true;

    m_iQEC =(int) (m_AdjPos[AxisID] * m_Rate[AxisID]);
    m_iSPD = m_Speed[AxisID];
    m_uiSiteID = UIM_ID+AxisID;
    INT aRtnValue;

    //P4输出0
    if (-1 == SetDOUT(m_uiDevIndex, m_uiSiteID,0,false,NULL)){
        emitError(SetDOUTError); return false;
    }
    //设置速度
    if (SetSPD(m_uiDevIndex, m_uiSiteID, m_iSPD, TRUE, &aRtnValue) == -1) {
        emitError(SetSPDError); return false;
    }
    //使能电机
    BASIC_ACK_OBJ RtnValue = { 0 };
    if (UimEna(m_uiDevIndex, m_uiSiteID, TRUE, &RtnValue) == -1) {
        emitError(UimENAError); return false;
    }

    //等待移动到校正开始位置
    m_Adj[AxisID]=0;TimOut=0;
    if (-1 == SetQEC(m_uiDevIndex, m_uiSiteID, m_iQEC, TRUE, &aRtnValue)){
        emitError(SetQECError); return false;
    }
    while (TimOut<TIM_OUT && m_Adj[AxisID]==0){
        QEventLoop eventloop;QTimer::singleShot(100, &eventloop, SLOT(quit()));eventloop.exec();
        TimOut++;
    }
//    printf("%d,%d,%d\n",Step,TimOut,m_Adj[AxisID]);
    if (m_Adj[AxisID]==0) return false;
    Step++;


    //P4输出5V
    if (-1 == SetDOUT(m_uiDevIndex, m_uiSiteID,1,false,NULL)){
        emitError(SetDOUTError); return false;
    }

    //等待移动到S3上升沿
    m_Adj[AxisID]=0;TimOut=0;
    if (-1 == SetQEC(m_uiDevIndex, m_uiSiteID, -m_iQEC, TRUE, &aRtnValue)){
        emitError(SetQECError); return false;
    }
    while (TimOut<TIM_OUT && m_Adj[AxisID]==0){
        QEventLoop eventloop;QTimer::singleShot(100, &eventloop, SLOT(quit()));eventloop.exec();
      TimOut++;
    }
//    printf("%d,%d,%d\n",Step,TimOut,m_Adj[AxisID]);
    if (m_Adj[AxisID]!=2) return false;
    Step++;

    //P4输出0V
    if (-1 == SetDOUT(m_uiDevIndex, m_uiSiteID,0,false,NULL)){
        emitError(SetDOUTError); return false;
    }

    //等待移动校正偏移量
    m_Adj[AxisID]=0;TimOut=0;
    if (-1 == SetSTP(m_uiDevIndex, m_uiSiteID,-m_Zero[AxisID],false,NULL)){
        emitError(SetQECError); return false;
    }
    while (TimOut<TIM_OUT && m_Adj[AxisID]==0){
        QEventLoop eventloop;QTimer::singleShot(100, &eventloop, SLOT(quit()));eventloop.exec();
      TimOut++;
    }
//    printf("%d,%d,%d\n",Step,TimOut,m_Adj[AxisID]);
    if (m_Adj[AxisID]!=1) return false;
    Step++;

    //设置原点
    if (-1 == SetORG(m_uiDevIndex, m_uiSiteID,0,false,NULL)){
        emitError(SetORGError); return false;
    }
    b_Adj[AxisID]=false;
    return true;
}

    //    m_Chk=0;
//    if (-1 == GetS34CON(m_uiDevIndex, m_uiSiteID, m_S34Con)){
//        emitError(Err_GetS34CON); return false;
//    }
//    uiS3RACT=0b0100;uiS3FACT=0b0100;
//    if (-1 == SetS34CON(m_uiDevIndex, m_uiSiteID,m_S34Con,0 ,m2_S34Con)){
//            emitError(Err_GetS34CON); return false;
//        }

//            ;
//    SetS34CON (
//    DWORD dwDevIndex,
//    DWORD dwCanNodeId,
//    P_S34CON pS34ConIn,
//    BOOL bAckEna,
//    P_S34CON pS34ConOut);do move1;
//    switch(AxisID)
//    {
//    case AXIS_Z:
//        m_iZ = (float)RtnValue / RATE_Z;
//        emit posDat(AXIS_Z,m_iZ);
//        break;
//    case AXIS_L:
//        m_iL = (float)RtnValue / RATE_L;
//        emit posDat(AXIS_L,m_iL);
//        break;
//    case AXIS_F:
//        m_iF = (float)RtnValue / RATE_F;
//        emit posDat(AXIS_F,m_iF);
//        break;
//    case AXIS_A:
//        m_iA = (float)RtnValue / RATE_A;
//        emit posDat(AXIS_A,m_iA);
//        break;
//    default:
//        break;
//    }


bool Location::setPos(AID AxisID, APOS AxisPOS)
{
    APOS iPOS=AxisPOS;
    //规范输入数据范围
    if (AxisID >= AXIS_NUMBER) { emitError(AxisError); return false; }
//    if (iPOS > m_Range[AxisID][1]) { emitWarning(OverRange); iPOS = m_Range[AxisID][1]; }
//    if (iPOS < m_Range[AxisID][0]) { emitWarning(BelowRange); iPOS = m_Range[AxisID][0]; }
    m_iQEC =(int) (iPOS * m_Rate[AxisID]);
    m_iSPD = m_Speed[AxisID];
    m_uiSiteID = UIM_ID+AxisID;
    INT aRtnValue;
    //  refactor codes
    if (AxisID == AXIS_Z)
    {
        if (-1 == SetPOS(m_uiDevIndex, m_uiSiteID, m_iQEC, TRUE, &aRtnValue)){
            emitError(SetQECError); return false;
        }
    }
    else
    {
        if (-1 == SetQEC(m_uiDevIndex, m_uiSiteID, m_iQEC, TRUE, &aRtnValue)){
            emitError(SetQECError); return false;
        }
    }
    //设置闭环位置,设置速度
    if (SetSPD(m_uiDevIndex, m_uiSiteID, m_iSPD, TRUE, &aRtnValue) == -1) {
        emitError(SetSPDError); return false;
    }
    //使能电机
    BASIC_ACK_OBJ RtnValue = { 0 };
    if (UimEna(m_uiDevIndex, m_uiSiteID, TRUE, &RtnValue) == -1) {
        emitError(UimENAError); return false;
    }
    m_ModeA[AxisID] = STEP;
    return true;
}

void Location::getPos(AID AxisID)
{
    //获取位置信息
    int RtnValue;
    if (AxisID == AXIS_Z)
    {
        if (-1 == GetPOS(m_uiDevIndex, UIM_ID+AxisID, &RtnValue)){
            emitError(GetQECError); return;
        }
    }
    else
    {
        if (-1 == GetQEC(m_uiDevIndex, UIM_ID+AxisID, &RtnValue)){
            emitError(GetQECError); return;
        }
    }
    switch (AxisID)
    {
    case AXIS_Z:
        m_iZ = (float)RtnValue / RATE_Z;
        emit posDat(AXIS_Z,m_iZ);
        break;
    case AXIS_L:
        m_iL = (float)RtnValue / RATE_L;
        emit posDat(AXIS_L,m_iL);
        break;
    case AXIS_F:
        m_iF = (float)RtnValue / RATE_F;
        emit posDat(AXIS_F,m_iF);
        break;
    case AXIS_A:
        m_iA = (float)RtnValue / RATE_A;
        emit posDat(AXIS_A,m_iA);
        break;
    default:
        break;
    }
}

bool Location::setAng(APOS axisPos)
{
    return setPos(AXIS_A,axisPos);
}

void Location::getAng()
{
    getPos(AXIS_A);
}

void Location::reset()
{
    setPos(AXIS_Z,0);
    setPos(AXIS_L,0);
    setPos(AXIS_F,0);
    setPos(AXIS_A,0);
}

void Location::emitError(ERRORTYPE ErrCode)
{
    emitWarning(ErrCode);

    switch (ErrCode){
    case LoadDllError:
    {
        qCritical(LOCATION()) << "载入 UISimCanFunc.dll 失败!";
        break;
    }
    case NoGatewayError:
    {
        qCritical(LOCATION()) << "没有找到网关";
        break;
    }
    case MoreGatewayError:
    {
        qCritical(LOCATION()) << "找到多个网关";
        break;
    }
    case OpenGatewayError:
    {
        qCritical(LOCATION()) << "打开网关失败";
        break;
    }
    case NoSiteError:
    {
        qCritical(LOCATION()) << "站点数量=0";
        break;
    }
    case SiteNumberError:
    {
        qCritical(LOCATION()) << "站点数量错误";
        break;
    }
    case FitSiteError:
    {
        qCritical(LOCATION()) << "和已知站点不匹配";
        break;
    }
    default:
        break;
    }
}

void Location::emitWarning(ERRORTYPE ErrCode)
{
    emit error(ErrCode);
}

void Location::StaticRevRtcnNotify(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen)
{
    ptrLocation->TmpRevRtcnNotify(dwDevIndex, pRtcnMsgObj, dwMsgLen);
}

void Location::TmpRevRtcnNotify(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen)
{
    UINT dwCanNodeId = ((pRtcnMsgObj->ID >> 3) & 0x060) + ((pRtcnMsgObj->ID >> 19) & 0x01F);
    UINT iRCW = pRtcnMsgObj->ID & 0xFF;
    UINT iData = pRtcnMsgObj->Data[0] & 0xFF;
    emit RtcnDat(dwCanNodeId,iRCW,iData);
}

void Location::RevRtcnNotify(uint dwCanNodeId, uint iRCW, uint iData)
{
    UINT iAxis=dwCanNodeId-UIM_ID;
    UINT iKey=iData-1;
    QString strShowMsg = _T("");
    strShowMsg.sprintf("站点ID=%d", dwCanNodeId);

    QString strTemp;
    switch (iRCW)
    {
    case 62:
//        strTemp.sprintf("UID900 %02x %02x %02x %02x", pRtcnMsgObj->Data[0], pRtcnMsgObj->Data[1], pRtcnMsgObj->Data[2], pRtcnMsgObj->Data[3]);
        //UIC900收到的数据
        //ToDo
        break;
    case 113:
        strTemp= _T("UIM242: S1下降沿变化通知");
        //S1下降沿
        //ToDo
        m_Adj[iAxis]=3;
        emitWarning(BelowRange);
        if (iAxis<3){
            bKeyEn[iAxis*2+1]=FALSE;
            b_TimerR[iAxis*2+1]=true;
            if(m_TimerR[iAxis*2+1]!=0) killTimer(m_TimerR[iAxis*2+1]);
            m_TimerR[iAxis*2+1] = startTimer(TIM_RANGE);
        }
        break;
    case 113 + (1 << 7) :
        strTemp= _T("UIM242: S1上升沿变化通知");
        //S1上升沿
        //ToDo
        // b_TimerR[iAxis*2+1]=false;
        if (iAxis<3){
            bKeyEn[iAxis*2+1]=TRUE;

        }
        break;
    case 114:
        strTemp= _T("UIM242: S2下降沿变化通知");
        //S2下降沿
        //ToDo
        m_Adj[iAxis]=4;
        emitWarning(OverRange);
        if (iAxis<3){
            bKeyEn[iAxis*2]=FALSE;
            b_TimerR[iAxis*2]=true;
            if(m_TimerR[iAxis*2]!=0) killTimer(m_TimerR[iAxis*2]);
            m_TimerR[iAxis*2] = startTimer(TIM_RANGE);
        }
        break;
    case 114 + (1 << 7) :
        strTemp= _T("UIM242: S2上升沿变化通知");
        //S2上升沿
        //ToDo
        // b_TimerR[iAxis*2]=false;
        if (iAxis<3){
            bKeyEn[iAxis*2]=TRUE;
        }
        break;
    case 115:
        strTemp= _T("UIM242: S3下降沿变化通知");
        //S3下降沿
        //ToDo
//        m_Adj[iAxis]=2;
        break;
    case 115 + (1 << 7) :
        strTemp= _T("UIM242: S3上升沿变化通知");
        //S3上升沿
        //ToDo
        m_Adj[iAxis]=2;
        break;
    case 116:
        strTemp= _T("UIM242: P4输出低电平通知");
        //P4输出低
        //ToDo
        break;
    case 116 + (1 << 7) :
        strTemp= _T("UIM242: P4输出高电平通知");
        //P4输出高
        //ToDo
        break;
    case 117:
        strTemp= _T("UIM242: 位移控制到位通知（开环）");
        //开环到位通知
        //ToDo
        if (m_ModeA[iAxis] == STEP)
        {
            if (m_TimerA[iAxis]!=0) killTimer(m_TimerA[iAxis]);
            m_TimerA[iAxis] = startTimer(TIM_POS);
        }
        break;
    case 117 + (1 << 7) :
        strTemp= _T("UIM242: 位移控制到位通知（闭环）");
        //闭环到位通知
        //ToDo

        m_Adj[iAxis]=1;
        if (m_ModeA[iAxis] == STEP)
        {
            if (m_TimerA[iAxis]!=0) killTimer(m_TimerA[iAxis]);
            m_TimerA[iAxis] = startTimer(TIM_POS);
        }

        break;
    case 118:
        strTemp.sprintf("UIM242: 到达原点通知");
        //原点到位通知
        //ToDo
        break;
    case 119:
        strTemp.sprintf("UIM242: 堵转通知");
        //堵转通知
        //ToDo
        emitError(LockedRotor);
        break;
    case 112:
        strTemp.sprintf(_T("%s%02d%s"), _T("UID820/828: CH"), iData, _T("下降沿变化通知"));
        //UID828某端口下降沿
        //ToDo
        bKeyPrs[iKey] = true;
        if(m_TimerK[iKey] != 0) killTimer(m_TimerK[iKey]);
        m_TimerK[iKey] = startTimer(TIM_KEY);

        break;
    case 112 + (1 << 7) :
        strTemp=QString("UID820/828: CH%1上升沿变化通知").arg(iData);
        //UID828某端口上升沿
        //ToDo
        bKeyPrs[iKey] = false;
        if(m_TimerK[iKey] != 0) killTimer(m_TimerK[iKey]);
        m_TimerK[iKey] = startTimer(TIM_KEY);
        break;
    default:
        break;
    }
}

void Location::timerEvent(QTimerEvent *event)
{
    int nIDEvent = event->timerId();
    killTimer(nIDEvent);
//到位延迟
    for(int i=0;i<AXIS_NUMBER;i++)
    {
        if (nIDEvent == m_TimerA[i])
        {
            m_TimerA[i]=0;
            getPos(i);
            m_uiSiteID = UIM_ID + i;
            BASIC_ACK_OBJ RtnValue = { 0 };
            if (-1 == UimOff(m_uiDevIndex, m_uiSiteID, TRUE, &RtnValue)) emitError(UimENAError);
            if (m_ModeA[i]==STEP) m_ModeA[i]=IDLE;
            break;
        }
    }
//按键延迟
    for(int i=0;i<KEY_NUMBER;i++)
    {
        if (nIDEvent == m_TimerK[i])
        {
            m_TimerK[i]=0;
            INT aRtnValue,j=(i & 0x6) >> 1;
            if (bKeyPrs[i])
            {
                if (bAxisOn[j] && bKeyEn[i] && !b_TimerR[i])
                  {
                    m_uiSiteID = UIM_ID + j;	//计算设备号
                    m_iSPD = (1 - ((i & 0x1) << 1)) * m_Speed[j];	//计算运动方向
                    //设置位移=0, 进入速度模式
                    if (-1 == SetSTP(m_uiDevIndex, m_uiSiteID, 0, TRUE, &aRtnValue)) emitError(SetSTPError);
                    //设置速度
                    if (-1 == SetSPD(m_uiDevIndex, m_uiSiteID, m_iSPD, TRUE, &aRtnValue)) emitError(SetSPDError);
                    //使能电机
                    BASIC_ACK_OBJ RtnValue = { 0 };
                    if (-1 == UimEna(m_uiDevIndex, m_uiSiteID, TRUE, &RtnValue)) emitError(UimENAError);
                    m_ModeA[j]=SPEED;
                }
            }
            else
            {

                if (bAxisOn[j] && !b_TimerR[i])
                {
                    m_uiSiteID = UIM_ID + j;	//计算设备号
                    //设置速度=0
                    BASIC_ACK_OBJ RtnValue = { 0 };
                    if (-1 == SetSPD(m_uiDevIndex, m_uiSiteID, 0, TRUE, &aRtnValue)) emitError(SetSPDError);
//                    if (-1 == UimOff(m_uiDevIndex, m_uiSiteID, TRUE, &RtnValue)) emitError(UimENAError);
                    if (m_TimerA[j]!=0) killTimer(m_TimerA[j]);
                    m_TimerA[j]=startTimer(TIM_POS);
                }
            }
            break;
        }
    }
//两端保护延迟,保护起作用时，按键失效
    for(int i=0;i<KEY_NUMBER;i++)
    {
        if (nIDEvent == m_TimerR[i])
        {
            m_TimerR[i]=0;
            b_TimerR[i]=false;
            break;
        }
    }
}
