////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, //
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 		        //
// WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.		    //
// 																				                                                //
//							                        Copyright (c) UIROBOT. 								            //
// 							                            All rights reserved.								                //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "LoadSimCanFunc.h"

SEARCHGATEWAY SearchGateway;
OPENGATEWAY OpenGateway;
CLOSEGATEWAY CloseGateway;
SETDEVINDEX SetDevIndex;
SEARCHDEVICE SearchDevice;
GETUIMDEVIDLIST GetUimDevIdList;
GETUIMDEVINFO GetUimDevInfo;
OPENUIMDEV OpenUimDev;
CLOSEUIMDEV CloseUimDev;
UIMGROBREG UimGrobReg;
GETMDL GetMDL;
UIMENA UimEna;
UIMOFF UimOff;
SETSPD SetSPD;
UIMACK UimAck;
UIMFBK UimFbk;
UIMSFBK UimSfbk;
SETPOS SetPOS;
SETSTP SetSTP;
SETGOFF SetgOFF;
SETGCUR SetgCUR;
SETGORG SetgORG;
SETGDIR SetgDIR;
SETGSPD SetgSPD;
SETGSTP SetgSTP;
SETGPOS SetgPOS;
SETGQEC SetgQEC;
GETUIMMCFG GetUimMCFG;
SETUIMMCFG SetUimMCFG;
SETMACC SetmACC;
GETMACC GetmACC;
SETMDEC SetmDEC;
GETMDEC GetmDEC;
SETMMDS SetmMDS;
GETMMDS GetmMDS;
SETMMSS SetmMSS;
GETMMSS GetmMSS;
GETSTG GetSTG;
SETSTG SetSTG;
SETQEC SetQEC;
SETQER SetQER;
SETORG SetORG;
SETSQT SetSQT;
GETSQT GetSQT;
SETBDR SetBDR;
SETADR SetADR;
SETGMCS SetgMCS;
SETGDOUT SetgDOUT;
SETGACR SetgACR;
UIMSTORE UimStore;
SETACR SetACR;
GETACR GetACR;
SETMCS SetMCS;

SETCUR SetCUR;

GETSPD GetSPD;
GETSTP GetSTP;
GETQEC GetQEC;
GETPOS GetPOS;
SETATCONH SetATCONH;
GETATCONH GetATCONH;
SETATCONL SetATCONL;
GETATCONL GetATCONL;
SETBTR SetBTR;
GETBTR GetBTR;
GETSCFG GetSCFG;
SETS12CON SetS12CON;
GETS12CON GetS12CON;
SETS34CON SetS34CON;
GETS34CON GetS34CON;
SETDOUT SetDOUT;
GETDOUT GetDOUT;
SETUIDMCFG SetUIDMCFG;
GETUIDMCFG GetUIDMCFG;
SETIOCONFIG SetIOconfig;
GETIOCONFIG GetIOconfig;
SETDVAINFO SetDVAInfo;
GETDVAINFO GetDVAInfo;
GETAVAINFO GetAVAInfo;
SETPMB SetPMB;
GETPMB GetPMB;
SETPMD SetPMD;
GETPMD GetPMD;
SETPBR SetPBR;
GETPBR GetPBR;
SETDMA SetDMA;
GETDMA GetDMA;


GETUIMICFG GetUimICFG;
SETUIMICFG SetUimICFG;
UIMADVENA UimAdvENA;
UIMGETADVENA UimGetAdvENA;
UIMADVSTORE UimAdvStore;
GETBLC GetBLC;
SETBLC SetBLC;
GETQER GetQER;
SETNRD SetNRD;

UIMREGRTCNCALLBACK UIMRegRtcnCallBack;

HINSTANCE m_hInstDevFunc;
BOOL LoadSimCanFunc()
{
    m_hInstDevFunc= NULL;
    SearchGateway= NULL;
    OpenGateway= NULL;
    CloseGateway= NULL;
    SearchDevice = NULL;
    GetUimDevIdList = NULL;
    GetUimDevInfo = NULL;
    OpenUimDev = NULL;
    CloseUimDev = NULL;
    UimGrobReg = NULL;
    GetMDL = NULL;
    UimEna = NULL;
    UimOff = NULL;
    SetSPD = NULL;
    UimAck = NULL;
    UimFbk = NULL;
    UimSfbk = NULL;
    SetPOS = NULL;
    SetSTP = NULL;
    SetgOFF = NULL;
    SetgCUR = NULL;
    SetgORG = NULL;
    SetgDIR = NULL;
    SetgSPD = NULL;
    SetgSTP = NULL;
    SetgPOS = NULL;
    SetgQEC = NULL;
    GetUimMCFG = NULL;
    SetUimMCFG = NULL;
    SetmACC = NULL;
    GetmACC = NULL;
    SetmDEC = NULL;
    GetmDEC = NULL;
    SetmMDS = NULL;
    GetmMDS = NULL;
    SetmMSS = NULL;
    GetmMSS = NULL;
    GetSTG = NULL;
    SetSTG = NULL;
    SetQEC = NULL;
    SetQER = NULL;
    SetORG = NULL;
    SetSQT = NULL;
    GetSQT = NULL;
    SetBDR = NULL;
    SetADR = NULL;
    SetgMCS = NULL;
    SetgDOUT = NULL;
    SetgACR = NULL;
    UimStore = NULL;
    SetACR = NULL;
    GetACR = NULL;
    SetMCS = NULL;

    SetCUR = NULL;
    GetSPD = NULL;
    GetSTP = NULL;
    GetQEC = NULL;
    GetPOS = NULL;
    SetATCONH = NULL;
    GetATCONH = NULL;
    SetATCONL = NULL;
    GetATCONL = NULL;
    SetBTR = NULL;
    GetBTR = NULL;
    GetSCFG = NULL;
    SetS12CON = NULL;
    GetS12CON = NULL;
    SetS34CON = NULL;
    GetS34CON = NULL;
    SetDOUT = NULL;
    GetDOUT = NULL;
    SetUIDMCFG = NULL;
    GetUIDMCFG = NULL;
    SetIOconfig = NULL;
    GetIOconfig = NULL;
    SetDVAInfo = NULL;
    GetDVAInfo = NULL;
    GetAVAInfo = NULL;
    SetPMB = NULL;
    GetPMB = NULL;
    SetPMD = NULL;
    GetPMD = NULL;
    SetPBR = NULL;
    GetPBR = NULL;
    SetDMA = NULL;
    GetDMA = NULL;

    SetDevIndex = NULL;
    SetNRD = NULL;

    GetUimICFG = NULL;
    SetUimICFG = NULL;
    UimAdvENA = NULL;
    UimGetAdvENA = NULL;
    UimAdvStore = NULL;
    GetBLC = NULL;
    SetBLC = NULL;
    GetQER = NULL;
    UIMRegRtcnCallBack = NULL;

    //装载UIROBOT提供的CAN网络驱动动态库
        m_hInstDevFunc = LoadLibrary(TEXT("../lib/Location/Inc/UISimCanFunc.dll"));
    if (m_hInstDevFunc == NULL)
    {
        FreeLibrary(m_hInstDevFunc);
        return FALSE;
    }

    //装载库函数
    SearchGateway= (SEARCHGATEWAY)GetProcAddress(m_hInstDevFunc, "SearchGateway");
    OpenGateway= (OPENGATEWAY)GetProcAddress(m_hInstDevFunc, "OpenGateway");
    CloseGateway= (CLOSEGATEWAY)GetProcAddress(m_hInstDevFunc, "CloseGateway");

    SearchDevice = (SEARCHDEVICE)GetProcAddress(m_hInstDevFunc, "SearchDevice");
    GetUimDevIdList = (GETUIMDEVIDLIST)GetProcAddress(m_hInstDevFunc, "GetUimDevIdList");
    GetUimDevInfo = (GETUIMDEVINFO)GetProcAddress(m_hInstDevFunc, "GetUimDevInfo");
    OpenUimDev = (OPENUIMDEV)GetProcAddress(m_hInstDevFunc, "OpenUimDev");
    CloseUimDev = (CLOSEUIMDEV)GetProcAddress(m_hInstDevFunc, "CloseUimDev");
    UimGrobReg = (UIMGROBREG)GetProcAddress(m_hInstDevFunc, "UimGrobReg");
    GetMDL = (GETMDL)GetProcAddress(m_hInstDevFunc, "GetMDL");
    UimEna = (UIMENA)GetProcAddress(m_hInstDevFunc, "UimENA");
    UimOff = (UIMOFF)GetProcAddress(m_hInstDevFunc, "UimOFF");
    SetSPD = (SETSPD)GetProcAddress(m_hInstDevFunc, "SetSPD");
    UimAck = (UIMACK)GetProcAddress(m_hInstDevFunc, "UimACK");
    UimFbk = (UIMFBK)GetProcAddress(m_hInstDevFunc, "UimFBK");
    UimSfbk = (UIMSFBK)GetProcAddress(m_hInstDevFunc, "UimSFBK");
    SetPOS = (SETPOS)GetProcAddress(m_hInstDevFunc, "SetPOS");
    SetSTP = (SETSTP)GetProcAddress(m_hInstDevFunc, "SetSTP");
    SetgOFF = (SETGOFF)GetProcAddress(m_hInstDevFunc, "SetgOFF");
    SetgCUR = (SETGCUR)GetProcAddress(m_hInstDevFunc, "SetgCUR");
    SetgORG = (SETGORG)GetProcAddress(m_hInstDevFunc, "SetgORG");
    SetgDIR = (SETGDIR)GetProcAddress(m_hInstDevFunc, "SetgDIR");
    SetgSPD = (SETGSPD)GetProcAddress(m_hInstDevFunc, "SetgSPD");
    SetgSTP = (SETGSTP)GetProcAddress(m_hInstDevFunc, "SetgSTP");
    SetgPOS = (SETGPOS)GetProcAddress(m_hInstDevFunc, "SetgPOS");
    SetgQEC = (SETGQEC)GetProcAddress(m_hInstDevFunc, "SetgQEC");
    GetUimMCFG = (GETUIMMCFG)GetProcAddress(m_hInstDevFunc, "GetUimMCFG");
    SetUimMCFG = (SETUIMMCFG)GetProcAddress(m_hInstDevFunc, "SetUimMCFG");
    SetmACC = (SETMACC)GetProcAddress(m_hInstDevFunc, "SetmACC");
    GetmACC = (GETMACC)GetProcAddress(m_hInstDevFunc, "GetmACC");
    SetmDEC = (SETMDEC)GetProcAddress(m_hInstDevFunc, "SetmDEC");
    GetmDEC = (GETMDEC)GetProcAddress(m_hInstDevFunc, "GetmDEC");
    SetmMDS = (SETMMDS)GetProcAddress(m_hInstDevFunc, "SetmMDS");
    GetmMDS = (GETMMDS)GetProcAddress(m_hInstDevFunc, "GetmMDS");
    SetmMSS = (SETMMSS)GetProcAddress(m_hInstDevFunc, "SetmMSS");
    GetmMSS = (GETMMSS)GetProcAddress(m_hInstDevFunc, "GetmMSS");
    GetSTG = (GETSTG)GetProcAddress(m_hInstDevFunc, "GetSTG");
    SetSTG = (SETSTG)GetProcAddress(m_hInstDevFunc, "SetSTG");
    SetQEC = (SETQEC)GetProcAddress(m_hInstDevFunc, "SetQEC");
    SetQER = (SETQER)GetProcAddress(m_hInstDevFunc, "SetQER");
    SetORG = (SETORG)GetProcAddress(m_hInstDevFunc, "SetORG");
    SetSQT = (SETSQT)GetProcAddress(m_hInstDevFunc, "SetSQT");
    GetSQT = (GETSQT)GetProcAddress(m_hInstDevFunc, "GetSQT");
    SetBDR = (SETBDR)GetProcAddress(m_hInstDevFunc, "SetBDR");
    SetADR = (SETADR)GetProcAddress(m_hInstDevFunc, "SetADR");
    SetgMCS = (SETGMCS)GetProcAddress(m_hInstDevFunc, "SetgMCS");
    SetgDOUT = (SETGDOUT)GetProcAddress(m_hInstDevFunc, "SetgDOUT");
    SetgACR = (SETGACR)GetProcAddress(m_hInstDevFunc, "SetgACR");
    UimStore = (UIMSTORE)GetProcAddress(m_hInstDevFunc, "UimSTORE");
    SetACR = (SETACR)GetProcAddress(m_hInstDevFunc, "SetACR");
    GetACR = (GETACR)GetProcAddress(m_hInstDevFunc, "GetACR");
    SetMCS = (SETMCS)GetProcAddress(m_hInstDevFunc, "SetMCS");
    SetCUR = (SETCUR)GetProcAddress(m_hInstDevFunc, "SetCUR");
    GetSPD = (GETSPD)GetProcAddress(m_hInstDevFunc, "GetSPD");
    GetSTP = (GETSTP)GetProcAddress(m_hInstDevFunc, "GetSTP");
    GetQEC = (GETQEC)GetProcAddress(m_hInstDevFunc, "GetQEC");
    GetPOS = (GETPOS)GetProcAddress(m_hInstDevFunc, "GetPOS");
    SetATCONH = (SETATCONH)GetProcAddress(m_hInstDevFunc, "SetATCONH");
    GetATCONH = (GETATCONH)GetProcAddress(m_hInstDevFunc, "GetATCONH");
    SetATCONL = (SETATCONL)GetProcAddress(m_hInstDevFunc, "SetATCONL");
    GetATCONL = (GETATCONL)GetProcAddress(m_hInstDevFunc, "GetATCONL");
    SetBTR = (SETBTR)GetProcAddress(m_hInstDevFunc, "SetBTR");
    GetBTR = (GETBTR)GetProcAddress(m_hInstDevFunc, "GetBTR");
    GetSCFG = (GETSCFG)GetProcAddress(m_hInstDevFunc, "GetSCFG");
    SetS12CON = (SETS12CON)GetProcAddress(m_hInstDevFunc, "SetS12CON");
    GetS12CON = (GETS12CON)GetProcAddress(m_hInstDevFunc, "GetS12CON");
    SetS34CON = (SETS34CON)GetProcAddress(m_hInstDevFunc, "SetS34CON");
    GetS34CON = (GETS34CON)GetProcAddress(m_hInstDevFunc, "GetS34CON");
    SetDOUT =(SETDOUT)GetProcAddress(m_hInstDevFunc, "SetDOUT");
    GetDOUT =(GETDOUT)GetProcAddress(m_hInstDevFunc, "GetDOUT");
    SetUIDMCFG = (SETUIDMCFG)GetProcAddress(m_hInstDevFunc, "SetUIDMCFG");
    GetUIDMCFG = (GETUIDMCFG)GetProcAddress(m_hInstDevFunc, "GetUIDMCFG");
    SetIOconfig = (SETIOCONFIG)GetProcAddress(m_hInstDevFunc, "SetIOconfig");
    GetIOconfig = (GETIOCONFIG)GetProcAddress(m_hInstDevFunc, "GetIOconfig");
    SetDVAInfo = (SETDVAINFO)GetProcAddress(m_hInstDevFunc, "SetDVAInfo");
    GetDVAInfo = (GETDVAINFO)GetProcAddress(m_hInstDevFunc, "GetDVAInfo");
    GetAVAInfo = (GETAVAINFO)GetProcAddress(m_hInstDevFunc, "GetAVAInfo");
    SetPMB = (SETPMB)GetProcAddress(m_hInstDevFunc, "SetPMB");
    GetPMB = (GETPMB)GetProcAddress(m_hInstDevFunc, "GetPMB");
    SetPMD = (SETPMD)GetProcAddress(m_hInstDevFunc, "SetPMD");
    GetPMD = (GETPMD)GetProcAddress(m_hInstDevFunc, "GetPMD");
    SetPBR = (SETPBR)GetProcAddress(m_hInstDevFunc, "SetPBR");
    GetPBR = (GETPBR)GetProcAddress(m_hInstDevFunc, "GetPBR");
    SetDMA = (SETDMA)GetProcAddress(m_hInstDevFunc, "SetDMA");
    GetDMA = (GETDMA)GetProcAddress(m_hInstDevFunc, "GetDMA");
    SetDevIndex = (SETDEVINDEX)GetProcAddress(m_hInstDevFunc, "SetDevIndex");

    GetUimICFG = (GETUIMICFG)GetProcAddress(m_hInstDevFunc, "GetUimICFG");
    SetUimICFG = (SETUIMICFG)GetProcAddress(m_hInstDevFunc, "SetUimICFG");
    UimAdvENA = (UIMADVENA)GetProcAddress(m_hInstDevFunc, "UimAdvENA");
    UimGetAdvENA = (UIMGETADVENA)GetProcAddress(m_hInstDevFunc, "UimGetAdvENA");
    UimAdvStore = (UIMADVSTORE)GetProcAddress(m_hInstDevFunc, "UimAdvSTORE");
    GetBLC = (GETBLC)GetProcAddress(m_hInstDevFunc, "GetBLC");
    SetBLC = (SETBLC)GetProcAddress(m_hInstDevFunc, "SetBLC");
    GetQER = (GETQER)GetProcAddress(m_hInstDevFunc, "GetQER");
    SetNRD = (SETNRD)GetProcAddress(m_hInstDevFunc, "SetNRD");



    UIMRegRtcnCallBack = (UIMREGRTCNCALLBACK)GetProcAddress(m_hInstDevFunc, "UIMRegRtcnCallBack");

        if (SearchDevice == NULL || GetUimDevIdList == NULL|| GetUimDevInfo == NULL
        || OpenUimDev == NULL || CloseUimDev == NULL || UimGrobReg == NULL
        || GetMDL == NULL || UimEna == NULL || UimOff == NULL
        || SetSPD == NULL || UimAck == NULL || UimFbk == NULL
        || UimSfbk == NULL || SetPOS == NULL || SetSTP == NULL
        || SetgOFF == NULL || SetgCUR == NULL || SetgORG == NULL
        || SetgDIR == NULL || SetgSPD == NULL || SetgSTP == NULL
        || SetgPOS == NULL || SetgQEC == NULL || GetUimMCFG == NULL
        || SetUimMCFG == NULL || SetmACC == NULL || GetmACC == NULL
        || SetmDEC == NULL || GetmDEC == NULL || SetmMDS == NULL
        || GetmMDS == NULL || SetmMSS == NULL || GetmMSS == NULL
        || GetSTG == NULL || SetSTG == NULL || SetQEC == NULL
        || SetQER == NULL || SetORG == NULL || SetSQT == NULL || GetSQT == NULL
        || SetBDR == NULL || SetADR == NULL || SetgMCS == NULL
        || SetgDOUT == NULL || SetgACR == NULL || UimStore == NULL
        || SetACR == NULL || GetACR == NULL || SetMCS == NULL
         || SetCUR == NULL ||SearchGateway== NULL || OpenGateway == NULL || CloseGateway== NULL
        || GetSPD == NULL || GetSTP == NULL || GetQEC == NULL
        || GetPOS == NULL || SetATCONH == NULL || GetATCONH == NULL
        || SetATCONL == NULL || GetATCONL == NULL || SetBTR == NULL
        || GetBTR == NULL || GetSCFG == NULL || SetS12CON == NULL
        || GetS12CON == NULL  || SetS34CON == NULL  || GetS34CON == NULL  || SetDOUT == NULL
        || SetUIDMCFG == NULL || GetUIDMCFG == NULL || SetIOconfig == NULL
        || GetIOconfig == NULL || SetDVAInfo == NULL || GetDVAInfo == NULL || GetDVAInfo == NULL
        || SetPMB == NULL || GetPMB == NULL || SetPMD == NULL
        || GetPMD == NULL || SetPBR == NULL || GetPBR == NULL
        || SetDMA == NULL || GetDMA == NULL
        || UIMRegRtcnCallBack == NULL || SetDevIndex == NULL
        || GetUimICFG == NULL || SetUimICFG == NULL
        || UimAdvENA == NULL || UimGetAdvENA == NULL || UimAdvStore == NULL
        || GetBLC == NULL || SetBLC == NULL || GetQER == NULL || SetNRD== NULL )
    {
        return FALSE;
    }
        return TRUE;
}


VOID FreeSimCanFunc()
{
     if (m_hInstDevFunc)
     {
         FreeLibrary(m_hInstDevFunc);
         m_hInstDevFunc = NULL;
     }
}
