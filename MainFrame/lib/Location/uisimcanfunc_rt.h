////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, //
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 		        //
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.		    //
// 																				                                                //
//							                        Copyright (c) UIROBOT. 								            //
// 							                            All rights reserved.								                //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __UISIMCANFUNC_RT_H__
#define __UISIMCANFUNC_RT_H__

#define     UIDEV_RS232CAN	            0x01
#define     UIDEV_WIFI2RS232CAN	        0x02
#define     UIDEV_USBCAN	            0x04
#define     UIDEV_PCICAN	            0x08
#define     UIDEV_ETHCAN	            0x10
#define     THIRD_PARTY_DEV	            0x20
#define     UIDEV_ALL                   (UIDEV_RS232CAN|UIDEV_WIFI2RS232CAN|UIDEV_USBCAN)



#define ERR_SYNTEX				50	//syntext error
#define ERR_NUMERICAL			51	//numerical error
#define ERR_MQEC					53	//current is QEC
#define ERR_MPOS					52	//current is POS
#define ERR_NOMODSUP			54	//no module support
typedef struct _GW_SEARCH_PARA
{
    DWORD  dwComIndex;
    DWORD  dwBtr;
}GW_SEARCH_PARA, *P_GW_SEARCH_PARA;
//Basic Instruction Acknowledgment
typedef struct _BASIC_ACK_OBJ
{
    UINT uiReserv;         //0
    BOOL bENA;             //电机使能状态
    BOOL bDIR;             //电机方向
    BOOL bACR;             //电流减半
    UINT uiMCS;             //电机细分
    UINT uiCUR;             //电流
    UINT uiSPD;             //当前速度
    UINT uiSTP;             //当前步长
}BASIC_ACK_OBJ, *P_BASIC_ACK_OBJ;

//Basic Instruction Feedback
typedef struct _BASIC_FBK_OBJ
{
    UINT uiReserv;         //0
    BOOL bENA;             //电机使能状态
    BOOL bDIR;             //电机方向
    BOOL bACR;             //电流减半
    UINT uiMCS;             //电机细分
    UINT uiCUR;             //电流
    INT iSPD;             //当前速度
    INT iSTP;             //当前步长
}BASIC_FBK_OBJ, *P_BASIC_FBK_OBJ;

//保存驱动器及固件信息
typedef struct _MDL_INFO_OBJ
{
    UINT uiCANNodeID;					//驱动器标识码
    UINT uiCANNodeType;				//驱动器型号
    UINT uiCurrent;						//电流
    BOOL bIntegrationEncode;		//内置编码器
    BOOL bEnCode;						//闭环控制
    BOOL bMotion;						//高级运动控制
    BOOL b2Sensor;						//"-S"
    BOOL b4Sensor;						//"-SP"
    UINT uiFirewareVersion;			//驱动器固件版本
    CHAR szModelName[20];		//驱动器型号
}MDL_INFO_OBJ, *P_MDL_INFO_OBJ;

//UIM设备的主配置寄存器结构
typedef union _UIM_MCFG_INFO_OBJ
{
    struct
    {
        unsigned bS1IE       : 1;
        unsigned bS2IE       : 1;
        unsigned bS3IE       : 1;
        unsigned bP4IE       : 1;
        unsigned bSTPIE      : 1;
        unsigned bORGIE     : 1;
        unsigned bSTLIE      : 1;
        unsigned                : 1;
        unsigned bDM         : 1;
        unsigned bAM         : 1;
        unsigned bCM         : 1;
        unsigned bQEM       : 1;
        unsigned                : 1;
        unsigned bQEI        : 1;
        unsigned bCHS        : 1;
        unsigned bANE        : 1;
        unsigned                : 16;
    };
    UINT uiMcfgVal;
}UIM_MCFG_INFO_OBJ, *P_UIM_MCFG_INFO_OBJ;

//UIM设备的上电状态控制寄存器结构
typedef union _UIM_ICFG_INFO_OBJ
{
    struct
    {
        unsigned bENA        : 1;
        unsigned bCCW        : 1;
        unsigned bPROG       : 1;
        unsigned bELOCK      : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned             : 1;
        unsigned iEV         : 2;
        //unsigned bEV0        : 1;
        //unsigned bEV1        : 1;
        unsigned             : 16;
    };
    UINT uiIcfgVal;
}UIM_ICFG_INFO_OBJ, *P_UIM_ICFG_INFO_OBJ;

//定义CAN信息帧的数据类型
typedef struct _CAN_MSG_OBJ
{
    DWORD   ID;                    //报文 ID  = SID(11位) | EID (18位)
    DWORD   Reserved0;         //保留, 赋值0
    BYTE    Reserved1;         //保留, 赋值0
    BYTE    SendType;		    //0：正常发送，1：自发自收
    BYTE    IDE;	                //0：标准帧，  1：扩展帧
    BYTE    RTR;	                //0：数据帧，  1：远程帧
    BYTE    DataLen;		    //表明Data[8]数组内的的字节数，长度不能超过8；
    BYTE    Data[8];			    //CAN数据包原为8个字节，为了支持RS232，此数据的长度增加为128
    BYTE    Reserved[3];		//系统保留
}CAN_MSG_OBJ, *P_CAN_MSG_OBJ;

//UID设备的主配置寄存器结构
typedef union _UID_MCFG_INFO_OBJ
{
    struct
    {
        unsigned bP1IE			:1;
        unsigned bP2IE			:1;
        unsigned bP3IE			:1;
        unsigned bP4IE			:1;
        unsigned bP5IE			:1;
        unsigned bP6IE			:1;
        unsigned bP7IE			:1;
        unsigned bP8IE			:1;
        unsigned bInitP1DVA	:1;
        unsigned bInitP2DVA	:1;
        unsigned bInitP3DVA	:1;
        unsigned bInitP4DVA	:1;
        unsigned bInitP5DVA	:1;
        unsigned bInitP6DVA	:1;
        unsigned bInitP7DVA	:1;
        unsigned bInitP8DVA	:1;
    };
    UINT uiMcfgVal;
}UID_MCFG_INFO_OBJ, *P_UID_MCFG_INFO_OBJ;

//UIM242XXX系列专用S34CON结构
typedef struct _S34CON
{
    UINT uiReserv    ;//堵状状态步数设置
    BOOL bP4LVL     ; //0:false 发生的事件符合P4EVENT 代码时，P4 出低电平（即0V）。
    //1:true 发生的事件符合P4EVENT 代码时，P4 出高电平（即5V）。
    UINT uiP4EVENT ;
    UINT uiS3RACT   ;//s3上升沿值
    UINT uiS3FACT   ;//s3下降沿值
    UINT uiSTLValue ; //堵状状态步数设置
}S34CON, *P_S34CON;

//S12CON结构
typedef struct _S12CON
{
    UINT uiS2RACT    ;//s2上降沿值
    UINT uiS2FACT    ;//s2下降沿值
    UINT uiS1RACT    ;//s1上升沿值
    UINT uiS1FACT    ;//s1下降沿值
}S12CON, *P_S12CON;

//传感器返回信息结构
typedef struct _SCFG_INFO_OBJ
{
    S12CON s12CON;      //S1,S2传感器动作代码存储到该结构
    S34CON s34CON;      //S3,S4传感器动作代码存储到该结构，仅适用于UIM242
    UINT uiATCONH;       //模拟量的最大值
    UINT uiATCONL;        //模拟量的最小值
}SCFG_INFO_OBJ, *P_SCFG_INFO_OBJ;

//STG所有配置信息
typedef struct _STG_INFO_OBJ
{
    UINT uiSenNum;       //需要设置的传感器
    UINT uiS1TrigerTime;
    UINT uiS2TrigerTime;
    UINT uiS3TrigerTime;
}STG_INFO_OBJ, *P_STG_INFO_OBJ;

//传感器返回信息结构
typedef struct _SFBK_INFO_OBJ
{
    BOOL bSensor1;      //S1传感器的电平逻辑值
    BOOL bSensor2;      //S2传感器的电平逻辑值
    BOOL bSensor3;      //S3传感器的电平逻辑值
    FLOAT fAnaInput;       //The converted value for analog input (12 bit)
}SFBK_INFO_OBJ, *P_SFBK_INFO_OBJ;

//UID820/828 IOC 结构
typedef union _IOC_INFO_OBJ
{
    struct
    {
        unsigned uiP1IOC	:2;
        unsigned uiP2IOC	:2;
        unsigned uiP3IOC	:2;
        unsigned uiP4IOC	:2;
        unsigned uiP5IOC	:2;
        unsigned uiP6IOC	:2;
        unsigned uiP7IOC	:2;
        unsigned uiP8IOC	:2;
    };
    UINT uiIOCVal;
}IOC_INFO_OBJ, *P_IOC_INFO_OBJ;

//UID820/828 DVA结构
typedef union _DVA_INFO_OBJ
{
    struct
    {
        unsigned bP1DVA	:1;
        unsigned bP2DVA	:1;
        unsigned bP3DVA	:1;
        unsigned bP4DVA	:1;
        unsigned bP5DVA	:1;
        unsigned bP6DVA	:1;
        unsigned bP7DVA	:1;
        unsigned bP8DVA	:1;
    };
    UINT uiDVAVal;
}DVA_INFO_OBJ, *P_DVA_INFO_OBJ;

//UID828 AVA端口配置结构
typedef union _AVA_PORT_OBJ
{
    struct
    {
        unsigned bP1AVA	:1;
        unsigned bP2AVA	:1;
        unsigned bP3AVA	:1;
        unsigned bP4AVA	:1;
        unsigned bP5AVA	:1;
        unsigned bP6AVA	:1;
        unsigned bP7AVA	:1;
        unsigned bP8AVA	:1;
    };
    BYTE btAVAPort;
}AVA_PORT_OBJ, *P_AVA_PORT_OBJ;

//UID828 AVA结构
typedef struct _AVA_INFO_OBJ
{
    AVA_PORT_OBJ objAVAPort;
    INT nP1Analog;
    INT nP2Analog;
    INT nP3Analog;
    INT nP4Analog;
    INT nP5Analog;
    INT nP6Analog;
    INT nP7Analog;
    INT nP8Analog;
}AVA_INFO_OBJ, *P_AVA_INFO_OBJ;

//UID820/828 PMB结构
typedef union _PMB_INFO_OBJ
{
    struct
    {
        unsigned uiPwmBase1	:8;
        unsigned uiPwmBase2	:8;
    };
    UINT uiPMBVal;
}PMB_INFO_OBJ, *P_PMB_INFO_OBJ;

//UID820/828 PMD结构
typedef union _PMD_INFO_OBJ
{
    struct
    {
        unsigned uiPwmDuty1	:8;
        unsigned uiPwmDuty2	:8;
    };
    UINT uiPMDVal;
}PMD_INFO_OBJ, *P_PMD_INFO_OBJ;



//UID828 AVA结构，用来获取模拟量值
typedef struct tagAVALogInfo
{
    INT nP8analog;
    INT nP7analog;
    INT nP6analog;
    INT nP5analog;
    INT nP4analog;
    INT nP3analog;
    INT nP2analog;
    INT nP1analog;
}AVALogInfo, *P_AVALogInfo;

//定义USB/PCI/RS232设备信息
typedef struct _DEV_INFO_OBJ
{
    DWORD	dwDevType;		        //0x11代表RS232CAN 0x20代表PCICAN
    DWORD	dwDevIndex;		        //软件启动时为设备分配的ID
    UINT     uiComIndex;               //系统分配的COM编号
    UINT		uiBaudRate;				//COM口对应的波特率
    CHAR	    pszDevName[64];        //CAN数据包原为8个字节，为了支持RS232，此数据的长度增加为128
    UINT		Protocol;                 //是字符串传输还是数据传输
}DEV_INFO_OBJ, *P_DEV_INFO_OBJ;

//DMA struct
typedef struct _DMA_OBJ
{
    UINT uiAddr;         //DMA烧写地址
    UCHAR d0;            //data0
    UCHAR d1;
    UCHAR d2;
    UCHAR d3;
    UCHAR d4;
    UCHAR d5;				//data5
}DMA_OBJ, *P_DMA_OBJ;

//OPR struct
typedef struct _OPR_CMD_OBJ
{
    UINT uiOprCode;         //DMA烧写地址
    UCHAR LL;
    UCHAR HH;
}OPR_CMD_OBJ, *P_OPR_CMD_OBJ;

//OPR DATA struct
typedef struct _OPR_DATA
{
    UINT uiDC;         //Data Count
    UCHAR d0;          //data0
    UCHAR d1;
    UCHAR d2;
    UCHAR d3;
    UCHAR d4;
    UCHAR d5;
    UCHAR d6;
}OPR_DATA, *P_OPR_DATA;

//OPR DATA struct
typedef struct _OPR_DATA_OBJ
{
    UCHAR catalog;     //instruct catalog
    UCHAR code;        //instruct code
    UINT  uiDC;        //Data Count
    UCHAR d0;          //data0 for input or output
    UCHAR d1;
    UCHAR d2;
    UCHAR d3;
    UCHAR d4;
}OPR_DATA_OBJ, *P_OPR_DATA_OBJ;

//NRD DATA struct
typedef struct _NRD_DATA
{
    UINT uiDC;         //Data Count
    UCHAR d[6];          //data for input
}NRD_DATA, *P_NRD_DATA;

typedef DWORD (*SEARCHGATEWAY)(DWORD dwGatewayType, P_GW_SEARCH_PARA pGWSearchPara, P_DEV_INFO_OBJ pDevInfoObj, int iLen);
typedef DWORD (*OPENGATEWAY)(DWORD dwDevIndex, PDWORD pSubNodeIdlist, int iLen, PDWORD pdCanBtr);
typedef DWORD (*CLOSEGATEWAY)(DWORD dwDevType);
/*=================================================================================================
    NAME:  VOID pCallBackFunc(P_RTCN_MSG_OBJ pRtcnMsgObj);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    pRtcnMsgObj[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  No Return Value
===================================================================================================*/
typedef VOID (*PF_SENSOR_NOTIFY_CALLBACK)(DWORD dwDevIndex, P_CAN_MSG_OBJ pRtcnMsgObj, DWORD dwMsgLen);

/*=================================================================================================
    NAME:   DWORD SEARCHDEVICE(DWORD dwDevType);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevType[in]:
    pDevIndexList[out, optional]: This parameter can be NULL.
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is the number of devices.
===================================================================================================*/
typedef DWORD (__cdecl *SEARCHDEVICE)(DWORD dwDevType, P_GW_SEARCH_PARA pGWSearchPara);

/*=================================================================================================
    NAME:   DWORD GetUimDevIdList(DWORD dwDevType, PDWORD pDevIndexList);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevType[in]:
    pDevIndexList[out, optional]: This parameter can be NULL.
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is the number of devices.
===================================================================================================*/
typedef DWORD (__cdecl *GETUIMDEVIDLIST)(DWORD dwDevType, PDWORD pDevIndexList);

/*=================================================================================================
    NAME:   GetUimDevInfo(DWORD dwDevIndex, P_DEV_INFO_OBJ pDevInfoObj);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    pDevInfoObj[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETUIMDEVINFO)(DWORD dwDevIndex, P_DEV_INFO_OBJ pDevInfoObj);

/*=================================================================================================
    NAME:   DWORD OpenUimDev(DWORD dwDevIndex);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]: The index of the device, which needs to be open.
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *OPENUIMDEV)(DWORD dwDevIndex);

/*=================================================================================================
    NAME:   DWORD CloseUimDev(DWORD dwDevIndex);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]: The index of the device, which needs to be closed.
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *CLOSEUIMDEV)(DWORD dwDevIndex);

/*=================================================================================================
    NAME:   DWORD UimGrobReg(DWORD dwDevIndex, PDWORD pCanNodeIdList);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]: The index of the device
    pCanNodeIdList[out, optional]: This parameter can be NULL.
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is the number of sub CAN nodes in whole network.
===================================================================================================*/
typedef DWORD (__cdecl *UIMGROBREG)(DWORD dwDevIndex, PDWORD pCanNodeIdList);

/*=================================================================================================
    NAME:   DWORD GetMDL(DWORD dwDevIndex, DWORD dwCanNodeId, P_MDL_INFO_OBJ pMDLInfoObj);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pMDLInfoObj[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETMDL)(DWORD dwDevIndex, DWORD dwCanNodeId, P_MDL_INFO_OBJ pMDLInfoObj);

/*=================================================================================================
    NAME:   DWORD UimENA(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, P_BASIC_ACK_OBJ pBasicAckobj)
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    bAckEna[in]:
    pBasicAckobj[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *UIMENA)(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, P_BASIC_ACK_OBJ pBasicAckobj);

/*=================================================================================================
    NAME:   DWORD UimOFF(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, P_BASIC_ACK_OBJ pBasicAckobj)
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    bAckEna[in]:
    pBasicAckobj[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *UIMOFF)(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, P_BASIC_ACK_OBJ pBasicAckobj);

/*=================================================================================================
    NAME:   DWORD SetSPD(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETSPD)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD UimACK(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_ACK_OBJ pACKInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pBasicAckobj[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *UIMACK)(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_ACK_OBJ pACKInfo);

/*=================================================================================================
    NAME:   DWORD UimFBK(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_FBK_OBJ pFBKInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pFBKInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *UIMFBK)(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_FBK_OBJ pFBKInfo);

/*=================================================================================================
    NAME:   DWORD UimSFBK(DWORD dwDevIndex, DWORD dwCanNodeId, P_SFBK_INFO_OBJ pSFBKInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pSFBKInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *UIMSFBK)(DWORD dwDevIndex, DWORD dwCanNodeId, P_SFBK_INFO_OBJ pSFBKInfo);

/*=================================================================================================
    NAME:   DWORD SetPOS(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETPOS)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetDOUT(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETDOUT)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetSTP(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETSTP)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetgOFF(DWORD dwDevIndex);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGOFF)(DWORD dwDevIndex);

/*=================================================================================================
    NAME:   DWORD SetgCUR(DWORD dwDevIndex, FLOAT fGrobCur)
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    fGrobCur[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGCUR)(DWORD dwDevIndex, FLOAT fGrobCur);

/*=================================================================================================
    NAME:   DWORD SetgORG(DWORD dwDevIndex, INT iGrobORG);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobORG[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGORG)(DWORD dwDevIndex, INT iGrobORG);

/*=================================================================================================
    NAME:   DWORD SetgDIR(DWORD dwDevIndex, BOOL bDir);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    bDir[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGDIR)(DWORD dwDevIndex, BOOL bDir);

/*=================================================================================================
    NAME:   DWORD SetgSPD(DWORD dwDevIndex, INT iGrobSPD);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobSPD[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGSPD)(DWORD dwDevIndex, INT iGrobSPD);

/*=================================================================================================
    NAME:   DWORD SetgSTP(DWORD dwDevIndex, INT iGrobSTP)
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobSTP[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGSTP)(DWORD dwDevIndex, INT iGrobSTP);

/*=================================================================================================
    NAME:   DWORD SetgPOS(DWORD dwDevIndex, INT iGrobPOS);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobPOS[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGPOS)(DWORD dwDevIndex, INT iGrobPOS);

/*=================================================================================================
    NAME:   DWORD SetgQEC(DWORD dwDevIndex, INT iGrobQEC);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobQEC[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGQEC)(DWORD dwDevIndex, INT iGrobQEC);

/*=================================================================================================
    NAME:   DWORD GetUimMCFG(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_MCFG_INFO_OBJ pMCFGInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobQEC[in]:
    pMCFGInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETUIMMCFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_MCFG_INFO_OBJ pMCFGInfo);

/*=================================================================================================
    NAME:   DWORD SetUimMCFG(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_MCFG_INFO_OBJ pMCFGInfoIn, BOOL bAckEna, P_UIM_MCFG_INFO_OBJ pMCFGInfoOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pMCFGInfoIn[in]:
    bAckEna[in]:
    pMCFGInfoOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETUIMMCFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_MCFG_INFO_OBJ pMCFGInfoIn, BOOL bAckEna, P_UIM_MCFG_INFO_OBJ pMCFGInfoOut);


/*=================================================================================================
    NAME:   DWORD SetmACC(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETMACC)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetmACC(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETMACC)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetmDEC(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETMDEC)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetmDEC(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue)
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETMDEC)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetmMDS(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETMMDS)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetmMDS(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETMMDS)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetmMSS(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETMMSS)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetmMSS(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETMMSS)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetSTG(DWORD dwDevIndex, DWORD dwCanNodeId, P_STG_INFO_OBJ pStgInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pStgInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETSTG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_STG_INFO_OBJ pStgInfo);

/*=================================================================================================
    NAME:   DWORD SetSTG(DWORD dwDevIndex, DWORD dwCanNodeId, P_STG_INFO_OBJ pStgInfoIn, BOOL bAckEna, P_STG_INFO_OBJ pStgInfoOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pStgInfoIn[in]:
    bAckEna[in]:
    pStgInfoOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETSTG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_STG_INFO_OBJ pStgInfoIn, BOOL bAckEna, P_STG_INFO_OBJ pStgInfoOut);

/*=================================================================================================
    NAME:   DWORD SetQEC(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETQEC)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetQER(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETQER)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetORG(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue)
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETORG)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetSQT(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETSQT)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
/*=================================================================================================
    NAME:   DWORD GetSQT(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Output:
===================================================================================================*/

typedef DWORD (__cdecl *GETSQT)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
/*=================================================================================================
    NAME:   DWORD SetBDR(DWORD dwDevIndex, INT iValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iValue[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETBDR)(DWORD dwDevIndex, INT iValue);

/*=================================================================================================
    NAME:   DWORD SetADR(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETADR)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue);

/*=================================================================================================
    NAME:   DWORD SetgMCS(DWORD dwDevIndex, INT iGrobMCS);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobMCS[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGMCS)(DWORD dwDevIndex, INT iGrobMCS);

/*=================================================================================================
    NAME:   DWORD SetgDOUT(DWORD dwDevIndex, INT iGrobDOUT);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobDOUT[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGDOUT)(DWORD dwDevIndex, INT iGrobDOUT);

/*=================================================================================================
    NAME:   DWORD SetgACR(DWORD dwDevIndex, INT iGrobACR);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iGrobACR[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETGACR)(DWORD dwDevIndex, INT iGrobACR);

/*=================================================================================================
    NAME:   DWORD UimSTORE(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *UIMSTORE)(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetACR(DWORD dwDevIndex, DWORD dwCanNodeId, INT iACR, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iACR[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETACR)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iACR, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetACR(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETACR)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetMCS(DWORD dwDevIndex, DWORD dwCanNodeId, INT iMCS, BOOL bAckEna, P_BASIC_ACK_OBJ pACKInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iMCS[in]:
    bAckEna[in]:
    pACKInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETMCS)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iMCS, BOOL bAckEna, P_BASIC_ACK_OBJ pACKInfo);

/*=================================================================================================
    NAME:   DWORD GetMCS(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_ACK_OBJ pACKInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pACKInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETMCS)(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_ACK_OBJ pACKInfo);

/*=================================================================================================
    NAME:   DWORD SetCUR(DWORD dwDevIndex, DWORD dwCanNodeId, INT iCUR, BOOL bAckEna, P_BASIC_ACK_OBJ pACKInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iCUR[in]:
    bAckEna[in]:
    pACKInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETCUR)(DWORD dwDevIndex, DWORD dwCanNodeId, FLOAT fCUR, BOOL bAckEna, P_BASIC_ACK_OBJ pACKInfo);

/*=================================================================================================
    NAME:   DWORD GetCUR(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_ACK_OBJ pACKInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pACKInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETCUR)(DWORD dwDevIndex, DWORD dwCanNodeId, P_BASIC_ACK_OBJ pACKInfo);

/*=================================================================================================
    NAME:   DWORD GetSPD(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETSPD)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);


/*=================================================================================================
    NAME:   DWORD GetDOUT(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETDOUT)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetSTP(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETSTP)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetQEC(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETQEC)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetPOS(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETPOS)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetATCONH(DWORD dwDevIndex, DWORD dwCanNodeId, INT iAtconH, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iAtconH[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETATCONH)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iAtconH, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetATCONH(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETATCONH)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetATCONL(DWORD dwDevIndex, DWORD dwCanNodeId, INT iAtconL, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iAtconH[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETATCONL)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iAtconL, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetATCONL(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETATCONL)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SetBTR(DWORD dwDevIndex, INT iBTR, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    iBTR[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETBTR)(DWORD dwDevIndex, INT iBTR, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetBTR(DWORD dwDevIndex, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETBTR)(DWORD dwDevIndex, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetSCFG(DWORD dwDevIndex, DWORD dwCanNodeId, P_SCFG_INFO_OBJ pSCFGInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pSCFGInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETSCFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_SCFG_INFO_OBJ pSCFGInfo);

/*=================================================================================================
    NAME:   DWORD SetS12CON(DWORD dwDevIndex, DWORD dwCanNodeId, P_S12CON pS12ConIn, BOOL bAckEna, P_S12CON pS12ConOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iAtconH[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETS12CON)(DWORD dwDevIndex, DWORD dwCanNodeId, P_S12CON pS12ConIn, BOOL bAckEna, P_S12CON pS12ConOut);

/*=================================================================================================
    NAME:   DWORD GetS12CON(DWORD dwDevIndex, DWORD dwCanNodeId, P_S12CON pS12Con);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETS12CON)(DWORD dwDevIndex, DWORD dwCanNodeId, P_S12CON pS12Con);

/*=================================================================================================
    NAME:   DWORD SetS34CON(DWORD dwDevIndex, DWORD dwCanNodeId, P_S34CON pS34ConIn, BOOL bAckEna, P_S34CON pS34ConOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iAtconH[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETS34CON)(DWORD dwDevIndex, DWORD dwCanNodeId, P_S34CON pS34ConIn, BOOL bAckEna, P_S34CON pS34ConOut);

/*=================================================================================================
    NAME:   DWORD GetS34CON(DWORD dwDevIndex, DWORD dwCanNodeId, P_S34CON pS34Con);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    Input:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETS34CON)(DWORD dwDevIndex, DWORD dwCanNodeId, P_S34CON pS34Con);


/*=================================================================================================
    NAME:   DWORD SetUIDMCFG(DWORD dwDevIndex, DWORD dwCanNodeId, P_UID_MCFG_INFO_OBJ pUIDMCFGIn, BOOL bAckEna, P_UID_MCFG_INFO_OBJ pUIDMCFGOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pUIDMCFGIn[in]:
    bAckEna[in]:
    pUIDMCFGOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETUIDMCFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UID_MCFG_INFO_OBJ pUIDMCFGIn, BOOL bAckEna, P_UID_MCFG_INFO_OBJ pUIDMCFGOut);

/*=================================================================================================
    NAME:   DWORD GetUIDMCFG(DWORD dwDevIndex, DWORD dwCanNodeId, P_UID_MCFG_INFO_OBJ pUIDMCFG);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pUIDMCFG[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETUIDMCFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UID_MCFG_INFO_OBJ pUIDMCFG);

/*=================================================================================================
    NAME:   DWORD SetIOconfig(DWORD dwDevIndex, DWORD dwCanNodeId, P_IOC_INFO_OBJ pIOCInfoIn, BOOL bAckEna, P_IOC_INFO_OBJ pIOCInfoOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pIOCInfoIn[in]:
    bAckEna[in]:
    pIOCInfoOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETIOCONFIG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_IOC_INFO_OBJ pIOCInfoIn, BOOL bAckEna, P_IOC_INFO_OBJ pIOCInfoOut);

/*=================================================================================================
    NAME:   DWORD GetIOconfig(DWORD dwDevIndex, DWORD dwCanNodeId, P_IOC_INFO_OBJ pIOCInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pIOCInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETIOCONFIG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_IOC_INFO_OBJ pIOCInfo);

/*=================================================================================================
    NAME:   DWORD SetDVAInfo(DWORD dwDevIndex, DWORD dwCanNodeId, P_DVA_INFO_OBJ pDvaInfoIn, BOOL bAckEna, P_DVA_INFO_OBJ pDvaInfoOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pDvaInfoIn[in]:
    bAckEna[in]:
    pDvaInfoOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETDVAINFO)(DWORD dwDevIndex, DWORD dwCanNodeId, P_DVA_INFO_OBJ pDvaInfoIn, BOOL bAckEna, P_DVA_INFO_OBJ pDvaInfoOut);

/*=================================================================================================
    NAME:   DWORD GetAVAInfo(DWORD dwDevIndex, DWORD dwCanNodeId, P_AVA_INFO_OBJ pAvaInfoIn, BOOL bAckEna, P_AVA_INFO_OBJ pAvaInfoOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pAvaInfoIn[in]:
    bAckEna[in]:
    pAvaInfoOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETAVAINFO)(DWORD dwDevIndex, DWORD dwCanNodeId, P_AVA_INFO_OBJ pAvaInfoIn, BOOL bAckEna, P_AVA_INFO_OBJ pAvaInfoOut);

/*=================================================================================================
    NAME:   DWORD GetAVAInfo(DWORD dwDevIndex, DWORD dwCanNodeId, P_DVA_INFO_OBJ pDvaInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pDvaInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETDVAINFO)(DWORD dwDevIndex, DWORD dwCanNodeId, P_DVA_INFO_OBJ pDvaInfo);
/*=================================================================================================
    NAME:   DWORD SetPMB(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMB_INFO_OBJ pPMBInfoIn, BOOL bAckEna, P_PMB_INFO_OBJ pPMBInfoOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pPMBInfoIn[in]:
    bAckEna[in]:
    pPMBInfoOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETPMB)(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMB_INFO_OBJ pPMBInfoIn, BOOL bAckEna, P_PMB_INFO_OBJ pPMBInfoOut);

/*=================================================================================================
    NAME:   DWORD GetPMB(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMB_INFO_OBJ pPMBInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pPMBInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETPMB)(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMB_INFO_OBJ pPMBInfo);

/*=================================================================================================
    NAME:   SetPMD(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMD_INFO_OBJ pPMDInfoIn, BOOL bAckEna, P_PMD_INFO_OBJ pPMDInfoOut);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pPMDInfoIn[in]:
    bAckEna[in]:
    pPMDInfoOut[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETPMD)(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMD_INFO_OBJ pPMDInfoIn, BOOL bAckEna, P_PMD_INFO_OBJ pPMDInfoOut);

/*=================================================================================================
    NAME:   DWORD GetPMD(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMD_INFO_OBJ pPMDInfo);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pPMDInfo[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETPMD)(DWORD dwDevIndex, DWORD dwCanNodeId, P_PMD_INFO_OBJ pPMDInfo);

/*=================================================================================================
    NAME:   DWORD SetPBR(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    iValue[in]:
    bAckEna[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETPBR)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD GetPBR(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETPBR)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

/*=================================================================================================
    NAME:   DWORD SETDMA(DWORD dwDevIndex, DWORD dwCanNodeId, P_DMA_OBJ pDMAin, BOOL bAckEna, P_DMA_OBJ pDMAout);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETDMA)(DWORD dwDevIndex, DWORD dwCanNodeId, P_DMA_OBJ pDMAin, BOOL bAckEna, P_DMA_OBJ pDMAout);

/*=================================================================================================
    NAME:   DWORD GETDMA(DWORD dwDevIndex, DWORD dwCanNodeId, INT iAddr, P_DMA_OBJ pDMAout);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *GETDMA)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iAddr, P_DMA_OBJ pDMAout);

/*=================================================================================================
    NAME:   DWORD SETOPR(DWORD dwDevIndex, DWORD dwCanNodeId, P_OPR_CMD_OBJ pOPRcmdobj, BOOL bAckEna);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETOPR)(DWORD dwDevIndex, DWORD dwCanNodeId, P_OPR_CMD_OBJ pOPRcmdobj, BOOL bAckEna);

/*=================================================================================================
    NAME:   DWORD OPRREAD(DWORD dwDevIndex, DWORD dwCanNodeId, const P_OPR_CMD_OBJ pOPRcmdobj, P_DMA_OBJ pDMAout);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *OPRREAD)(DWORD dwDevIndex, DWORD dwCanNodeId, const P_OPR_CMD_OBJ pOPRcmdobj, P_DMA_OBJ pDMAout);

/*=================================================================================================
    NAME:   DWORD OPRREADPTR(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnLine);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pRtnValue[out]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *OPRREADPTR)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnLine);

/*=================================================================================================
    NAME:   DWORD SetDevIndex(DWORD dwPreDevIndex, DWORD dwNewDevIndex);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwPreDevIndex[in]:
    dwNewDevIndex[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *SETDEVINDEX)(DWORD dwPreDevIndex, DWORD dwNewDevIndex);

typedef DWORD (__cdecl *GETUIMICFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_ICFG_INFO_OBJ pICFGInfoOut);
typedef DWORD (__cdecl *SETUIMICFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_ICFG_INFO_OBJ pICFGInfoIn, BOOL bAckEna, P_UIM_ICFG_INFO_OBJ pICFGInfoOut);
typedef DWORD (__cdecl *UIMADVENA)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
typedef DWORD (__cdecl *UIMGETADVENA)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
typedef DWORD (__cdecl *UIMADVSTORE)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
typedef DWORD (__cdecl *SETBLC)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iValue, BOOL bAckEna, PINT pRtnValue);
typedef DWORD (__cdecl *GETBLC)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);
typedef DWORD (__cdecl *GETQER)(DWORD dwDevIndex, DWORD dwCanNodeId, PINT pRtnValue);

//UIM900 NRD
typedef DWORD (__cdecl *SETNRD)(DWORD dwDevIndex, DWORD dwCanNodeId, P_NRD_DATA pNRDDataIn, BOOL bAckEna, P_NRD_DATA pNRDDataOut);

//================================OPRx RELATED COMMAND====================================//
typedef DWORD (__cdecl *GETUIMICFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_ICFG_INFO_OBJ pICFGInfoOut);
typedef DWORD (__cdecl *SETUIMICFG)(DWORD dwDevIndex, DWORD dwCanNodeId, P_UIM_ICFG_INFO_OBJ pICFGInfoIn, BOOL bAckEna, P_UIM_ICFG_INFO_OBJ pICFGInfoOut);

typedef DWORD (__cdecl *OPRATE)(DWORD dwDevIndex, DWORD dwCanNodeId, P_OPR_DATA_OBJ pOPRDataObjIn, BOOL bAckEna, P_OPR_DATA_OBJ pOPRDataObjOut);

typedef DWORD (__cdecl *OPR_FILLDMA)(DWORD dwDevIndex, DWORD dwCanNodeId, BYTE btFillByte, BOOL bAckEna, P_OPR_DATA_OBJ pOPRDataOut);
typedef DWORD (__cdecl *OPR_CLEARFLASH)(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, P_OPR_DATA_OBJ pOPRDataOut);
typedef DWORD (__cdecl *OPR_BURNFLASH)(DWORD dwDevIndex, DWORD dwCanNodeId, INT iAddress, BOOL bAckEna, P_OPR_DATA_OBJ pOPRDataOut);
//typedef DWORD (__cdecl *OPR_READLINE)(DWORD dwDevIndex, DWORD dwCanNodeId,INT iAddress, BOOL bAckEna, P_OPR_DATA pOPRDataOut);
typedef DWORD (__cdecl *OPR_READCURRPTR)(DWORD dwDevIndex, DWORD dwCanNodeId, BOOL bAckEna, P_OPR_DATA_OBJ pOPRDataOut);

/*=================================================================================================
    NAME:   DWORD UIMRegRtcnCallBack(DWORD dwDevType, DWORD dwDevIndex, PF_SENSOR_NOTIFY_CALLBACK pCallBackFunc);
    ------------------------------------------------------------------------------------------
    Function:
    ------------------------------------------------------------------------------------------
    dwDevIndex[in]:
    dwCanNodeId[in]:
    pCallBackFunc[in]:
    ------------------------------------------------------------------------------------------
    Return Value:  If the function fails, the return value is -1. If the function succeeds, the return value is 1.
===================================================================================================*/
typedef DWORD (__cdecl *UIMREGRTCNCALLBACK)(DWORD dwDevType, DWORD dwDevIndex, PF_SENSOR_NOTIFY_CALLBACK pCallBackFunc);

#endif //__UISIMCANFUNC_RT_H__
