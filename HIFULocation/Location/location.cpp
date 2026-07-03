#include <QTimerEvent>
#include <QSettings>
#include <QEventLoop>
#include <QThread>

#include <QTimer>
#include <QDebug>

#include "location.h"
#include "tchar.h"
#include "LoadSimCanFunc.h"

#include <QDateTime>

Q_LOGGING_CATEGORY(LOCATION,"LOCATION")

Location::Location(QObject *parent) : QObject(parent)
{
    _pSLocation=this;
    connect(_pSLocation,SIGNAL(HandleRtcnNotify(uint,uint,uint)),
            this,SLOT(AsyncRevRtcnNotify(uint,uint,uint)));
    m_iModeZ=0;
}

Location::~Location()
{
    if (m_bDevCnectFlg)
    {
        UIMRegRtcnCallBack(0, m_uiDevIndex, NULL);
        if(SetStop(m_uiDevIndex,UIM_ID+AXIS_Z)&&SetStop(m_uiDevIndex,UIM_ID+AXIS_L)&&SetStop(m_uiDevIndex,UIM_ID+AXIS_F)
                &&SetStop(m_uiDevIndex,UIM_ID+AXIS_A))
            qCDebug(LOCATION()) << "优爱宝停止运动";
        else
            return;
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
    falling_edge_flag=false;
    speedmotion_flag=false;
    for(int i=0;i<AXIS_NUMBER;i++)
    {
        bAxisOn[i]=FALSE;
        m_ModeA[i]=IDLE;
    }
    bUIDon = FALSE;
    for(int i=0;i<KEY_NUMBER;i++)
    {
        bKeyEn[i]=FALSE;
        b_TimerR[i]=FALSE;
    }

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
    Port=configIniRead->value("Location/Port").toInt();
    Baund=configIniRead->value("Location/Baund").toInt();
    //QSettings *configIniRead = new QSettings("./config/config.ini", QSettings::IniFormat);
    m_Rate[0]=configIniRead->value("Location/Rate_Z").toInt();
    m_Rate[1]=configIniRead->value("Location/Rate_L").toInt();
    m_Rate[2]=configIniRead->value("Location/Rate_F").toInt();
    m_Rate[3]=configIniRead->value("Location/Rate_A").toInt();
    m_Speed[0]=configIniRead->value("Location/Speed_Z").toInt();
    m_Speed[1]=configIniRead->value("Location/Speed_L").toInt();
    m_Speed[2]=configIniRead->value("Location/Speed_F").toInt();
    m_Speed[3]=configIniRead->value("Location/Speed_A").toInt();
    m_Zero[0]=configIniRead->value("Location/Zero_Z").toInt();
    m_Zero[1]=configIniRead->value("Location/Zero_L").toInt();
    m_Zero[2]=configIniRead->value("Location/Zero_F").toInt();
    m_Zero[3]=configIniRead->value("Location/Zero_A").toInt();
    m_iModeZ=configIniRead->value("Location/Mode_Z",0).toInt();
    m_DeltaZ_Min = configIniRead->value("Location/DeltaZ_Min",2).toInt();
    m_DeltaZ_Max = configIniRead->value("Location/DeltaZ_Max",8).toInt();
    if(m_iModeZ<0||m_iModeZ>1){
        qDebug(LOCATION())<<"Z轴电机模式配置文件错误 m_iModeZ:"<<m_iModeZ;
        m_iModeZ=0;
    }
    delete configIniRead;
//    printf("Z=%d,L=%d,F=%d,A=%d",m_Zero[0],m_Zero[1],m_Zero[2],m_Zero[3]);

    ERRORTYPE RctnErr=initUIM();

    if (RctnErr != NoError)
    {
        emit statusUpdated(POSITION_NONAVAILABLE);
        emitError(RctnErr);
        return false;
    } else
         emit statusUpdated(POSITION_AVAILABLE);

    qCDebug(LOCATION()) << "Initialized successfully.";
    return true;
}

int Location::initUIM()
{
    // 载入动态库
    if (!LoadSimCanFunc())
    {
        qCDebug(LOCATION()) <<"载入动态库失败";
        return LoadDllError;
    }

    P_GW_SEARCH_PARA pGWSearchPara = NULL;
    if(!pGWSearchPara)
    {
        pGWSearchPara = new GW_SEARCH_PARA[5];
        memset(pGWSearchPara,0,5 * sizeof(GW_SEARCH_PARA));
    }
//    pGWSearchPara->dwComIndex = Port;
    pGWSearchPara->dwComIndex = 9;
    pGWSearchPara->dwBtr = Baund;

    //查找网关
    DWORD m_ComNum = SearchGateway(UIDEV_RS232CAN, NULL, m_GatewayInfo, MAX_GATEWAY);
    if (m_ComNum == -1)
    {
        qCDebug(LOCATION()) <<"查找网关失败";
        return SearchGatewayError;
    }
    else if (m_ComNum == 0)
    {
        qCDebug(LOCATION()) <<"未找到网关";
        return NoGatewayError;
    }
    else if (m_ComNum != 1)
    {
        qCDebug(LOCATION()) <<"找到多个网关";
        return MoreGatewayError;
    }
    m_uiDevIndex = m_GatewayInfo[0].dwDevIndex;

   //打开网关
   DWORD dwCanBtr;
   DWORD m_dwSubCanNodeLen = OpenGateway(m_uiDevIndex, m_SiteID, 120, &dwCanBtr);
   if (m_dwSubCanNodeLen == -1)
   {
       qCDebug(LOCATION()) <<"打开网关失败";
       return OpenGatewayError;
   }
   else if (m_dwSubCanNodeLen == 0)
   {
       qCDebug(LOCATION()) <<"没有找到站点";
       return NoSiteError;
   }
   //else if (m_dwSubCanNodeLen != SITE_NUMBER)
   //    return SiteNumberError;
   m_bDevCnectFlg = TRUE;

   for (uint i = 0; i < m_dwSubCanNodeLen; i++)
   {
       //获取站点信息
       GetMDL(m_uiDevIndex, m_SiteID[i], &(p_SiteInfo[i]));

       //判断是否已知的ID
       m_uiSiteID = m_SiteID[i];
       qCDebug(LOCATION()) <<"站点匹配 ID:"<<m_uiSiteID;
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
   if (bAxisOn[0] && bAxisOn[1] && bAxisOn[2] && bAxisOn[3])
   {
       //设置回调函数
       UIMRegRtcnCallBack(0, m_uiDevIndex, Location::StaticRevRtcnNotify);
   }else
   {
       qCDebug(LOCATION()) <<"站点ID匹配失败,匹配结果：Z L F A:"<<bAxisOn[0] << bAxisOn[1] << bAxisOn[2] << bAxisOn[3];
       return FitSiteError;
   }
   return NoError;
}

bool Location::zero (AID AxisID)
{
    int Step;
    Step=0;
    b_Adj[AxisID]=true;

    m_iQEC =(int) (m_AdjPos[AxisID] * m_Rate[AxisID]);
    m_iSPD = m_Speed[AxisID];
    m_uiSiteID = UIM_ID+AxisID;

    switch (AxisID) {
    case AXIS_Z:
    {
        qCDebug(LOCATION()) <<"开始Z轴(上下)校零"<<"设备ID:"<<m_uiDevIndex<<"站点ID:"<<m_uiSiteID;
        qCDebug(LOCATION()) <<"参数配置 Rate:"<<m_Rate[AxisID]<<"Speed:"<<m_Speed[AxisID]<<"Zero:"<<m_Zero[AxisID];
        if(!SetTTLOutput(m_uiDevIndex,m_uiSiteID,true))
            return false;
        if(!SetSpeedMotion(m_uiDevIndex,m_uiSiteID,m_iSPD))
            return false;

        if(!(WaitByFlag(TIM_OUT_Z,1,FS1,AxisID)))
        {
            qCDebug(LOCATION()) <<"到达S1标志错误或等待超时"<<"等待标志:"<<m_FlagAdj[2][AxisID]<<"期望标志:4";
            return false;
        }
        Wait(5000);
        getPos(AXIS_Z);
        Wait(1000);
        m_iZ_Delta = m_iZ -m_iZ_REC;
        qDebug()<<"m_iZ_Delta===="<<m_iZ_Delta<<", m_iZ_REC===="<<m_iZ_REC<<", m_iZ===="<<m_iZ;
        if((!zero_flag) || (m_iZ_Delta<m_DeltaZ_Min) || (m_iZ_Delta >m_DeltaZ_Max)){
            SetStop(m_uiDevIndex,m_uiSiteID);
            qDebug(LOCATION()) << "Z轴校零失败";
            return false;
        }
        if(!SetOrigin(m_uiDevIndex,m_uiSiteID))
            return false;
        b_Adj[AxisID]=false;
        qCDebug(LOCATION()) <<"Z轴校零成功";
        return true;
    }
        break;
    case AXIS_A:
    {
        qCDebug(LOCATION()) <<"开始A轴(旋转)校零"<<"设备ID:"<<m_uiDevIndex<<"站点ID:"<<m_uiSiteID;
        qCDebug(LOCATION()) <<"参数配置 Rate:"<<m_Rate[AxisID]<<"Speed:"<<m_Speed[AxisID]<<"Zero:"<<m_Zero[AxisID];

        if(!SetTTLOutput(m_uiDevIndex,m_uiSiteID,false))
            return false;
        if(!SetSpeedMotion(m_uiDevIndex,m_uiSiteID,m_iSPD))
            return false;
        if(!WaitByFlag(TIM_OUT_Z,1,FS2,AxisID))
        {
            qCDebug(LOCATION()) <<"到达S2标志错误或等待超时"<<"等待标志:"<<m_FlagAdj[2][AxisID]<<"期望标志:1";
            return false;
        }
        Wait(500);
        Step++;
        if(!SetTTLOutput(m_uiDevIndex,m_uiSiteID,true))
            return false;
        if(!SetSpeedMotion(m_uiDevIndex,m_uiSiteID,-m_iSPD))
            return false;
        if(!WaitByFlag(TIM_OUT_A,1,FS3,AxisID))
        {
            qCDebug(LOCATION()) <<"到达S3标志错误或等待超时"<<"等待标志:"<<m_FlagAdj[3][AxisID]<<"期望标志:1";
            return false;
        }
        Step++;
        if(!SetTTLOutput(m_uiDevIndex,m_uiSiteID,false))
            return false;
        if(!SetStep(m_uiDevIndex,m_uiSiteID,-m_Zero[AxisID]))
            return false;
        if(!WaitByFlag(TIM_OUT_A,1,FDIST,AxisID))
        {
            qCDebug(LOCATION()) <<"偏移量到位标志错误或等待超时"<<"等待标志:"<<m_FlagAdj[0][AxisID]<<"期望标志:1";
            return false;
        }
        Step++;
        if(!SetOrigin(m_uiDevIndex,m_uiSiteID))
            return false;
        b_Adj[AxisID]=false;
        qCDebug(LOCATION()) <<"A轴校零成功";
        return true;
    }
        break;
    case AXIS_L:
    {
        if(!setPos(AXIS_L,0)) return false;
        return true;
    }
        break;
    case AXIS_F:
    {
        if(!setPos(AXIS_F,0)) return false;
        return true;
    }
        break;
    default:
        break;
    }
}

bool Location::setPos(AID AxisID, APOS AxisPOS)
{
    qCDebug(LOCATION())<< LOCATION().categoryName()
                       << "setPos,AxisID:"<<AxisID<<"AxisPOS:"<<AxisPOS
                       << TRACE_CMH();

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
    if (AxisID == AXIS_Z&&m_iModeZ==0)
    {
        if (-1 == SetPOS(m_uiDevIndex, m_uiSiteID, m_iQEC, TRUE, &aRtnValue)){
            emitError(SetQECError); return false;
        }
    }
    else
    {
        if (-1 == SetQEC(m_uiDevIndex, m_uiSiteID, m_iQEC, TRUE, &aRtnValue)){
            emitError(SetQECError); emit LocationErr();m_bDevCnectFlg=false;return false;
        }
        else{
            emit SetPosSuccess();
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
        qCDebug(LOCATION()) << "载入 UISimCanFunc.dll 失败!";
        break;
    }
    case SearchGatewayError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数SearchGateway出错";
        break;
    }
    case NoGatewayError:
    {
        qCDebug(LOCATION()) << "没有找到网关";
        break;
    }
    case MoreGatewayError:
    {
        qCDebug(LOCATION()) << "找到多个网关";
        break;
    }
    case OpenGatewayError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数OpenGateway出错";
        break;
    }
    case NoSiteError:
    {
        qCDebug(LOCATION()) << "站点数量=0";
        break;
    }
    case SiteNumberError:
    {
        qCDebug(LOCATION()) << "站点数量错误";
        break;
    }
    case FitSiteError:
    {
        qCDebug(LOCATION()) << "和已知站点不匹配";
        break;
    }
    case AdjRotateError:
    {
        qCDebug(LOCATION()) << "校正旋转探头错误";
        break;
    }
    case AxisError:
    {
        qCDebug(LOCATION()) << "不正确的运动轴";
        break;
    }
    case LockedRotor:
    {
        qCDebug(LOCATION()) << "堵转";
        break;
    }
    case BelowRange:
    {
        qCDebug(LOCATION()) << "触碰下限位开关";
        break;
    }
    case OverRange:
    {
        qCDebug(LOCATION()) << "触碰上限位开关";
        break;
    }

    case SetSPDError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数SetSPD出错";
        break;
    }
    case SetSTPError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数SetSTP出错";
        break;
    }
    case SetQECError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数SetQEC出错";
        break;
    }
    case UimENAError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数UimENA出错";
        break;
    }
    case GetQECError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数GetQEC出错";
        break;
    }
    case SetDOUTError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数SettDOUT出错";
        break;
    }
    case SetORGError:
    {
        qCDebug(LOCATION()) << "执行DLL库函数SetORG出错";
        break;
    }
    default:
        qCDebug(LOCATION()) << "其他错误";
        break;
    }
}

void Location::emitWarning(ERRORTYPE ErrCode)
{
    emit error(ErrCode);
}

void Location::RevRtcnNotify(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen)
{
    UINT dwCanNodeId = ((pRtcnMsgObj->ID >> 3) & 0x060) + ((pRtcnMsgObj->ID >> 19) & 0x01F);
    UINT iRCW = pRtcnMsgObj->ID & 0xFF;
    UINT iData = pRtcnMsgObj->Data[0] &0xFF;
    emit HandleRtcnNotify(dwCanNodeId,iRCW,iData);
}

void Location::AsyncRevRtcnNotify(uint dwCanNodeId, uint iRCW, uint iData)
{
    UINT iAxis=dwCanNodeId-UIM_ID;
    UINT iKey=iData-1;

    UINT i =0;
    QString strTime;
    QString strOneMsgHex;
    QString strShowMsg;
    strShowMsg=QString("%1=%2").arg("SiteID").arg(dwCanNodeId);

    bool outlog=false;

    QString strTemp;
    switch (iRCW)
    {
    case 62:
        strTemp.append("UID900");
        break;
    case 113:
        strTemp.append("UIM242：S1 falling edge");
        zero_flag = false;
        falling_edge_flag=true;
        m_FlagAdj[1][iAxis]=1;
        emitWarning(BelowRange);
        if (iAxis<3){
            bKeyEn[iAxis*2+1]=FALSE;
            b_TimerR[iAxis*2+1]=true;
            if(m_TimerR[iAxis*2+1]!=0) killTimer(m_TimerR[iAxis*2+1]);
            m_TimerR[iAxis*2+1] = startTimer(TIM_RANGE);
        }
        outlog=true;
        break;
    case 113 + (1<<7):
        strTemp.append("UIM242：S1 risning edge");
        falling_edge_flag=false;
        if (iAxis<3){
            bKeyEn[iAxis*2+1]=TRUE;
        }
        outlog=true;
        getPos(iAxis);
        m_iZ_REC = m_iZ;
        break;
    case 114:
        strTemp.append("UIM242：S2 falling edge");
        falling_edge_flag=true;
        m_FlagAdj[2][iAxis]=1;
        emitWarning(OverRange);
        if (iAxis<3){
            bKeyEn[iAxis*2]=FALSE;
            b_TimerR[iAxis*2]=true;
            if(m_TimerR[iAxis*2]!=0) killTimer(m_TimerR[iAxis*2]);
            m_TimerR[iAxis*2] = startTimer(TIM_RANGE);
        }
        outlog=true;
        break;
    case 114 + (1<<7):
        strTemp.append("UIM242：S2 risning edge");
        falling_edge_flag=false;
        if (iAxis<3){
            bKeyEn[iAxis*2]=TRUE;
        }
        outlog=true;
        getPos(iAxis);
        break;
    case 115:
        strTemp.append("UIM242：S3 falling edge");
        m_FlagAdj[3][iAxis]=1;
        break;
    case 115 + (1<<7):
        strTemp.append("UIM242：S3 risning edge");
//        m_FlagAdj[3][iAxis]=1;
        break;
    case 116:
        strTemp.append("UIM242：P4 risning edge TTL output P4 Low");
        break;
    case 116+ (1<<7):
        strTemp.append("UIM242：P4 risning edge TTL output P4 High");
        break;
    case 117:
        strTemp.append("UIM242：displacement control complete(Open Loop)");
        outlog=true;
        if(!speedmotion_flag)
            getPos(iAxis);
        break;
    case 117 + (1<<7):
        strTemp.append("UIM242：displacement control complete(Close Loop)");
        outlog=true;
        zero_flag = true;
        if(!speedmotion_flag)
            getPos(iAxis);
        m_FlagAdj[0][iAxis]=1;
        break;
    case 118:
        strTemp.append("UIM242：zero position"); break;
    case 119:
        strTemp.append("UIM242：stalling");
        //SetStop(m_uiDevIndex,dwCanNodeId);
        emitError(LockedRotor);
        //SetSpeedMotion(m_uiDevIndex,dwCanNodeId,-1500);
        bKeyPrs[iKey] = true;
//        SetSpeedMotionByKey(iKey,true);
        break;
    case 112:
        strTemp.append("UID820/828：CH%1%2").arg(iData).arg(" falling edge");
        bKeyPrs[iKey] = true;
//        SetSpeedMotionByKey(iKey,true);
        outlog=true;
        break;
    case 112 + (1<<7):
        strTemp.append("UID820/828：CH%1%2").arg(iData).arg(" risning edge");
        bKeyPrs[iKey] = false;
//        SetSpeedMotionByKey(iKey,false);
        outlog=true;
        break;
    default: break;
    }

    if(outlog){
        //输出返回信息
        strTime=QDateTime::currentDateTime().toString("hh:mm:ss:zzz");
//        SYSTEMTIME stTime = {0};
//        GetLocalTime(&stTime);
//        strTime.append(_T("%02d:%02d:%02d:%03d"), stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
        strOneMsgHex=QString("%1:%2 ,%3").arg(strTime).arg(strShowMsg).arg(strTemp);
        qCDebug(LOCATION()) <<"优爱宝实时消息:"<<strOneMsgHex;
    }
}

void Location::timerEvent(QTimerEvent *event)
{
    int nIDEvent = event->timerId();
    killTimer(nIDEvent);
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

//速度模式运动
bool Location::SetSpeedMotion(UINT DevIndex,UINT SiteID,int speed)
{
    INT aRtnValue;
    if (-1 == SetSTP(DevIndex, SiteID, 0, TRUE, &aRtnValue))
    {
        qCDebug(LOCATION()) <<"设置相对位移增量为0失败";
        emitError(SetSTPError);
        return false;
    }
    if (-1 == SetSPD(DevIndex, SiteID, speed, TRUE, &aRtnValue))
    {
        qCDebug(LOCATION()) <<"设置速度失败";
        emitError(SetSPDError);
        return false;
    }
    BASIC_ACK_OBJ RtnValue = { 0 };
    if (-1 == UimEna(DevIndex, SiteID, TRUE, &RtnValue))
    {
        qCDebug(LOCATION()) <<"使能电机失败";
        emitError(UimENAError);
        return false;
    }
    return true;
}
//使能和脱机
bool Location::SetUimEnable(UINT DevIndex,UINT SiteID,bool enable)
{
    BASIC_ACK_OBJ RtnValue = {0};
    if(enable){
        if (-1 == UimEna(DevIndex, SiteID, TRUE, &RtnValue))
        {
            qCDebug(LOCATION()) <<"使能电机失败";
            emitError(UimENAError);
            return false;
        }
    }
    else
    {
        if (-1 == UimOff(DevIndex, SiteID, TRUE, &RtnValue))
        {
            qCDebug(LOCATION()) <<"电机脱机失败";
            emitError(UimENAError);
            return false;
        }
    }
    return true;
}
//设置TTL是否输出高电平
bool Location::SetTTLOutput(UINT DevIndex,UINT SiteID,bool bOut)
{
    if(bOut){
        if(-1==SetDOUT(DevIndex, SiteID,1,false,NULL))
        {
            qCDebug(LOCATION()) <<"设置TTL输出5V失败";
            return false;
        }
    }
    else{
        if(-1==SetDOUT(DevIndex, SiteID,0,false,NULL))
        {
            qCDebug(LOCATION()) <<"设置TTL输出0V失败";
            return false;
        }
    }
    return true;
}
//设置原点
bool Location::SetOrigin(UINT DevIndex, UINT SiteID)
{
    qCDebug(LOCATION()) <<"设置原点"<<DevIndex<<","<<SiteID;
    if (-1 == SetORG(DevIndex, SiteID,0,false,NULL)){
        qCDebug(LOCATION()) <<"设置原点失败";
        emitError(SetORGError);
        return false;
    }
    return true;
}
//设置相对位移增量（速度位设置）
bool Location::SetStep(UINT DevIndex, UINT SiteID, int iStep)
{
    if (-1 == SetSTP(DevIndex, SiteID,iStep,false,NULL)){
        qCDebug(LOCATION()) <<"设置校正偏移量失败";
        emitError(SetQECError);
        return false;
    }
    return true;
}

//按键的速度模式
bool Location::SetSpeedMotionByKey(UINT iKey,bool bKeyPress)
{
    qCDebug(LOCATION()) <<"SetSpeedMotionByKey,iKey:"<<iKey<<"bKeyPress:"<<bKeyPress;
    INT aRtnValue;
    INT j=(iKey & 0x6) >> 1;
    UINT siteid=0;
    int iSpeed=0;
    if (bKeyPress)
    {
        if (bAxisOn[j] && bKeyEn[iKey]&&!b_TimerR[iKey])
          {
            siteid = UIM_ID + j;	//计算设备号
            iSpeed = (1 - ((iKey & 0x1) << 1)) * m_Speed[j];	//计算运动方向
//            if(SetSpeedMotion(m_uiDevIndex,siteid,iSpeed));
            m_ModeA[j]=SPEED;
            if(!SetSpeedMotion(m_uiDevIndex,siteid,iSpeed))
                return false;
        }
    }
    else
    {

        if (bAxisOn[j]&&!b_TimerR[iKey])
        {
            siteid = UIM_ID + j;	//计算设备号
            //设置速度=0
            if (-1 == SetSPD(m_uiDevIndex, siteid, 0, TRUE, &aRtnValue)) {emitError(SetSPDError);return false;}
        }
    }
    return true;
}
//停止运动
bool Location::SetStop(UINT DevIndex, UINT SiteID)
{
    INT aRtnValue;
    if (-1 == SetSTP(DevIndex, SiteID, 0, TRUE, &aRtnValue))
    {
        qCDebug(LOCATION()) <<"SetStop Axis"<<SiteID<<"设置相对位移增量为0失败";
        emitError(SetSTPError);
        return false;
    }
    if (-1 == SetSPD(DevIndex, SiteID, 0, TRUE, &aRtnValue))
    {
        qCDebug(LOCATION()) <<"SetStop Axis"<<SiteID<<"设置速度失败";
        emitError(SetSPDError);
        return false;
    }
    return true;
}
//延时
void Location::Wait(int msecond)
{
    QEventLoop waitloop;
    QTimer::singleShot(msecond, &waitloop, SLOT(quit()));
    waitloop.exec();
}
//延时到某标志
bool Location::WaitByFlag(int iTimeOut, int iExpectFlag,FlagType flag,int Axis)
{
    int iFlag=(int)flag;
    m_FlagAdj[iFlag][Axis]=0;
    int waittime=0;
    while (waittime<=iTimeOut && m_FlagAdj[iFlag][Axis]!=iExpectFlag){
        QEventLoop eventloop;
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
        waittime++;
    }
    if(waittime==iTimeOut||m_FlagAdj[iFlag][Axis]!=iExpectFlag)
    {
        return false;
    }
    return true;
}

void Location::ReceiveHeadControl(int AID, int direction)
{
    INT aRtnValue;
    switch(AID)
    {
        case 0:
            if(direction==0/*&&falling_edge_flag==false*/)
            {
                speedmotion_flag=false;
                if (-1 == SetSPD(m_uiDevIndex, UIM_ID+AXIS_Z, 0, TRUE, &aRtnValue))
                    qDebug()<<"set Z stop failed";
                Delay_MSec(350);
                getPos(AXIS_Z);
            }
            else if(direction==-1/*&&falling_edge_flag==false*/&&m_iZ<400)
            {
                speedmotion_flag=true;
                if(!SetSpeedMotion(m_uiDevIndex,UIM_ID+AXIS_Z,-m_Speed[0]))
                    qDebug()<<"set Z move failed";
            }
            else if(direction==1/*&&falling_edge_flag==false*/&&m_iZ>0)
            {
                speedmotion_flag=true;
                if(!SetSpeedMotion(m_uiDevIndex,UIM_ID+AXIS_Z,m_Speed[0]))
                    qDebug()<<"set Z move failed";
            }
            break;

        case 1:
            if(direction==0/*&&falling_edge_flag==false*/)
            {
                speedmotion_flag=false;
                if (-1 == SetSPD(m_uiDevIndex, UIM_ID+AXIS_L, 0, TRUE, &aRtnValue))
                    qDebug()<<"set L stop failed";
                getPos(AXIS_L);
            }
            else if(direction==-1/*&&falling_edge_flag==false*/)
            {
                speedmotion_flag=true;
                if(!SetSpeedMotion(m_uiDevIndex,UIM_ID+AXIS_L,-m_Speed[1]))
                    qDebug()<<"set L move failed";
            }
            else if(direction==1/*&&falling_edge_flag==false*/)
            {
                speedmotion_flag=true;
                if(!SetSpeedMotion(m_uiDevIndex,UIM_ID+AXIS_L,m_Speed[1]))
                    qDebug()<<"set L move failed";
            }
            break;

        case 2:
            if(direction==0/*&&falling_edge_flag==false*/)
            {
                speedmotion_flag=false;
                if (-1 == SetSPD(m_uiDevIndex, UIM_ID+AXIS_F, 0, TRUE, &aRtnValue))
                    qDebug()<<"set F stop failed";
                getPos(AXIS_F);
            }
            else if(direction==-1/*&&falling_edge_flag==false*/)
            {
                speedmotion_flag=true;
                if(!SetSpeedMotion(m_uiDevIndex,UIM_ID+AXIS_F,-m_Speed[2]))
                    qDebug()<<"set F move failed";
            }
            else if(direction==1/*&&falling_edge_flag==false*/)
            {
                speedmotion_flag=true;
                if(!SetSpeedMotion(m_uiDevIndex,UIM_ID+AXIS_F,m_Speed[2]))
                    qDebug()<<"set F move failed";
            }
            break;
    }
}

void Location::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
