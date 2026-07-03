/*==============================================================================
*
*       filename        dllentry.c
*       purpose         API for dynamic loading okapi32.dll (Ok series image cards)
*       language        Microsoft Visual C 5.0
*       author          S. Yang
*       date            mon-date-99
*-----------------------
*       modify by       S. Yang
*       purpose         
*       language        Microsoft Visual C 5.0 
*       date            99
*
*-----------------------
*
*		Copyright (C).  All Rights Reserved.
*
*
*==============================================================================
*       language include files
*/


#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include <dos.h>
#include <memory.h>
#include <time.h>
#include<windows.h>


#include "okapi32.h"


/*-----------------------------------------------------------------------------
*       Functions Pointors
*/


//1. section

HANDLE	(WINAPI *lpfnOpenBoard)(MLONG *);
HANDLE	(WINAPI *lpfnOpenBoardEx)(MLONG *, HANDLE, LPVOID);
BOOL	(WINAPI *lpfnCloseBoard)(HANDLE);
INT32	(WINAPI *lpfnGetLastError)(void);
MLONG	(WINAPI *lpfnGetBufferSize)(HANDLE hBoard, void **lpLinear, MWORD *dwSize);
LPVOID 	(WINAPI *lpfnGetBufferAddr)(HANDLE hBoard, MLONG iNoFrame);
LPVOID  (WINAPI *lpfnGetTargetInfo)(HANDLE hBoard, TARGET tgt, MLONG lNoFrame, SHORT * wid, SHORT * ht, MLONG *stride);
INT32	(WINAPI *lpfnGetTypeCode)(HANDLE hBoard,LPSTR lpBoardName);
INT32	(WINAPI *lpfnGetNetDevNumber)(INT32 iTypeCode, LPSTR lpNameString);
MLONG	(WINAPI *lpfnSetTargetRect)(HANDLE hBoard, TARGET target, LPRECT lpTgtRect);
INT32	(WINAPI *lpfnSetToWndRect)(HANDLE hBoard, HWND hWnd);
MLONG	(WINAPI *lpfnSetVideoParam)(HANDLE hBoard, INT32 wParam, MLONG lParam);
MLONG	(WINAPI *lpfnSetCaptureParam)(HANDLE hBoard, INT32 wParam, MLONG lParam);
MLONG	(WINAPI *lpfnSetDeviceParam)(HANDLE hBoard, INT32 wParam, MLONG lParam);

MLONG	(WINAPI *lpfnSetUserData)(HANDLE hBoard, MLONG mUserData);
BOOL	(WINAPI *lpfnSetSeqProcWnd)(HANDLE hBoard, HWND hwndMain);
BOOL	(WINAPI *lpfnSetSeqCallback)(HANDLE hBoard, 
								BOOL CALLBACK BeginProc(HANDLE hBoard), 
								BOOL CALLBACK SeqProc(HANDLE hBoard, MLONG No), 
								BOOL CALLBACK EndProc(HANDLE hBoard));
BOOL	(WINAPI *lpfnSetEventCallback)(HANDLE hBoard, INT32 iEvent, BOOL CALLBACK EventProc(HANDLE hBoard, MWORD dwReserved) );
BOOL	(WINAPI *lpfnSetCloseCallback)(HANDLE hBoard, BOOL CALLBACK CloseProc(HANDLE hBoard, MWORD dwRes));


MLONG	(WINAPI *lpfnCaptureTo)(HANDLE hBoard, TARGET Dest, MLONG start, MLONG lParam);
BOOL	(WINAPI *lpfnCaptureSingle)(HANDLE hBoard, TARGET Dest, MLONG lStart);
BOOL	(WINAPI *lpfnCaptureActive)(HANDLE hBoard, TARGET Dest, MLONG lStart);
HANDLE	(WINAPI *lpfnCaptureThread)(HANDLE hBoard, TARGET Dest, MLONG lStart, MLONG lNoFrame);
BOOL	(WINAPI *lpfnCaptureSequence)(HANDLE hBoard, MLONG lStart, MLONG lNoFrame);

MLONG	(WINAPI *lpfnCaptureToScreen)(HANDLE hBoard);
MLONG	(WINAPI *lpfnPlaybackFrom)(HANDLE hBoard, TARGET src, MLONG start, MLONG lParam);
BOOL	(WINAPI *lpfnPlaybackSequence)(HANDLE hBoard, MLONG lStart, MLONG lNoFrame);
MLONG	(WINAPI *lpfnSetDriverParam)(INT32 lWhich, MLONG lParam);
MLONG	(WINAPI *lpfnGetCaptureStatus)(HANDLE hBoard, BOOL bWait);
MLONG	(WINAPI *lpfnGetLineReady)(HANDLE hBoard, MLONG *iCurrFrameNo, MWORD dwRes);

MLONG	(WINAPI *lpfnStopCapture)(HANDLE hBoard);

MLONG	(WINAPI *lpfnGetSeqCapture)(HANDLE hApi, MLONG start, MLONG count);

HANDLE	(WINAPI *lpfnCaptureByBuffer)(HANDLE hBoard, TARGET tgt, MLONG start, MLONG num);
HANDLE	(WINAPI *lpfnCaptureByBufferEx)(HANDLE hBoard, MLONG fileset, TARGET dest, MLONG start, MLONG num);
HANDLE	(WINAPI *lpfnPlaybackByBuffer)(HANDLE hBoard, TARGET src, MLONG start, MLONG num);


INT32	(WINAPI *lpfnReadPixel)(HANDLE hBoard, TARGET src, MLONG start, SHORT x, SHORT y);
INT32	(WINAPI *lpfnWritePixel)(HANDLE hBoard, TARGET tgt, MLONG start, SHORT x, SHORT y,  INT32 lValue);

MLONG	(WINAPI *lpfnSetConvertParam)(HANDLE hBoard, INT32 wParam, MLONG lParam);
INT32	(WINAPI *lpfnReadRect)(HANDLE hBoard, TARGET src, MLONG start, LPBYTE lpBuf);
INT32	(WINAPI *lpfnWriteRect)(HANDLE hBoard, TARGET dst, MLONG start, LPBYTE lpBuf);
INT32	(WINAPI *lpfnTransferRect)(HANDLE hBoard, TARGET dest, MLONG iFirst, TARGET src, MLONG iStart, MLONG lNum);
INT32	(WINAPI *lpfnConvertRect)(HANDLE hBoard, TARGET dst, MLONG first, TARGET src, MLONG start, MLONG no);
INT32	(WINAPI *lpfnConvertRectEx)(HANDLE hDstBoard, TARGET dst, MLONG first, HANDLE hSrcBoard,TARGET src, MLONG start, MLONG no);
INT32	(WINAPI *lpfnReadRectEx)(HANDLE hBoard, TARGET src, MLONG start, LPBYTE lpBuf, INT32 lParam);
INT32	(WINAPI *lpfnWriteRectEx)(HANDLE hBoard, TARGET dst, MLONG start, LPBYTE lpBuf, INT32 lParam);

BOOL	(WINAPI *lpfnSetTextTo)(HANDLE hBoard, TARGET target, LPRECT lpRect, LOGFONT *lfLogFont, SETTEXTMODE *textMode, LPSTR lpString, INT32 lLength); 
INT32	(WINAPI *lpfnDrawLineTo)(HANDLE hBoard, TARGET target, MLONG lStart, POINT pt0, POINT pt1, INT32 iForeColor);
INT32	(WINAPI *lpfnDrawEllipsTo)(HANDLE hBoard, TARGET target, MLONG lStart, LPRECT lpRect, INT32 iForeColor);
INT32	(WINAPI *lpfnFillCircleTo)(HANDLE hBoard, TARGET target,MLONG lStart, LPRECT lpRect, INT32 iForeColor, INT32 lMode);
HDC		(WINAPI *lpfnCreateDCBitmap)(HANDLE hBoard, TARGET target, HANDLE *hDCBitmap);
BOOL	(WINAPI *lpfnMapDCBitmapTo)(HANDLE hDCBitmap, MLONG lStart);
BOOL	(WINAPI *lpfnFreeDCBitmap)(HANDLE hDCBitmap);

BOOL	(WINAPI *lpfnWaitVerticalSync)(HANDLE hBoard, BOOL bHeader);



INT32	(WINAPI *lpfnGetSignalParam)(HANDLE hBoard, INT32 wParam);
INT32	(WINAPI *lpfnWaitSignalEvent)(HANDLE hBoard, INT32 wParam, MLONG lMilliSecond);
MLONG	(WINAPI *lpfnPutSignalParam)(HANDLE hBoard, INT32 wParam, MLONG lParam);


INT32	(WINAPI *lpfnEnableMask)(HANDLE hBoard, BOOL bMask);
INT32	(WINAPI *lpfnSetMaskRect)(HANDLE hBoard, LPRECT lpRect, LPSTR lpMask);

INT32	(WINAPI *lpfnFillOutLUT)(HANDLE hBoard,  LPVOID bLUT, INT32 offset, INT32 num);
INT32	(WINAPI *lpfnFillInputLUT)(HANDLE hBoard,  LPVOID bLUT, INT32 offset, INT32 num);

INT32	(WINAPI *lpfnGetImageDevice)(OKDEVTYPE **lpOkDevInfo, MLONG lpParam);
SHORT	(WINAPI *lpfnGetSlotBoard)(BOARDTYPE **lpOkInfo);
SHORT	(WINAPI *lpfnGetBoardIndex)(CHAR *szBoardName, SHORT iNo);
INT32	(WINAPI *lpfnGetBoardName)(SHORT lIndex, LPSTR szBoardName);

MLONG	(WINAPI *lpfnGetAvailBuffer)(void **lpLinear, MWORD *dwSize);
MLONG	(WINAPI *lpfnLockBuffer)(HANDLE hBoard, MWORD dwSizeByte, void **lpBasLinear);
BOOL	(WINAPI *lpfnUnlockAllBuffer)(void);

INT32	(WINAPI *lpfnApplyMemBlock)(DWORD dwBlockSize, DWORD dwBlockNo);
BOOL	(WINAPI *lpfnFreeMemBlock)(void);
INT32	(WINAPI *lpfnGetMemBlock)(HANDLE hBoard, DWORD *dwEachSize,  DWORD *dwBlockNo);
INT32	(WINAPI *lpfnLockMemBlock)(HANDLE hBoard, DWORD dwBlockNo);
BOOL	(WINAPI *lpfnUnlockMemBlock)(void);



MLONG	(WINAPI *lpfnMulCaptureTo)(HANDLE *lphBoard, TARGET Dest, MLONG start, MLONG lParam);
HANDLE	(WINAPI *lpfnMulCaptureByBuffer)(HANDLE *lphBoard, TARGET tgt, MLONG start, MLONG num);

BOOL	(WINAPI *lpfnSaveInitParam)(HANDLE hBoard, SHORT iChannNo);
BOOL	(WINAPI *lpfnLoadInitParam)(HANDLE hBoard, SHORT iChannNo);

MLONG	(WINAPI *lpfnSetAllocBuffer)(MLONG lSize);
BOOL	(WINAPI *lpfnSetStaticVxD)(INT32 lMode);
BOOL	(WINAPI *lpfnSetNTDriver)(BOOL bCmd);
INT32	(WINAPI *lpfnSetLangResource)(INT32 bCmd);


BOOL	(WINAPI *lpfnUnRegister)(DWORD dwCmd);
INT32	(WINAPI *lpfnGetProgramInfo)(INT32 iItem, LPSTR lpString, INT32 iSize);

INT32	(WINAPI *lpfnReadProtCode)(HANDLE hBoard, SHORT iIndex);
INT32	(WINAPI *lpfnWriteProtCode)(HANDLE hBoard, SHORT iIndex, INT32 Code);

MLONG	(WINAPI *lpfnSaveImageFile)(HANDLE hBoard, LPSTR szFileName, MLONG first, TARGET target, MLONG start, MLONG num);
MLONG	(WINAPI *lpfnLoadImageFile)(HANDLE hBoard, LPSTR szFileName, MLONG first, TARGET target, MLONG start, MLONG num);
BOOL	(WINAPI *lpfnGetCurrImageInfo)(HANDLE hBoard, MLONG lpImgFrmInfo, MLONG lSize);


BOOL	(WINAPI *lpfnLoadConfigFile)(HANDLE hBoard, LPSTR szFileName);
BOOL	(WINAPI *lpfnSaveConfigFile)(HANDLE hBoard, LPSTR szFileName);

BOOL	(WINAPI *lpfnLoadFlatModelFile)(HANDLE hBoard, LPSTR szFileName);
BOOL	(WINAPI *lpfnSaveFlatModelFile)(HANDLE hBoard, LPSTR szFileName);
 
BOOL	(WINAPI *lpfnOpenSetParamDlg)( HANDLE hBoard, HWND hParentWnd);
BOOL	(WINAPI *lpfnOpenSeqCaptureDlg)( HANDLE hBoard, HWND hParentWnd);
LPDIBINFO (WINAPI *lpfnOpenReplayDlg)( HANDLE hBoard, HWND hWnd, TARGET src, MLONG total);
HWND     (WINAPI *lpfnOpenReplayDlgEx)( HANDLE hBoard, HWND hWnd, TARGET src, MLONG total,
									LPBITMAPINFOHEADER lpbi, LPBYTE lpdib);
INT32	(WINAPI *lpfnAutoAdjustBright)(HANDLE hBoard, LPRECT lpRect, LPVOID dwRes);
INT32	(WINAPI *lpfnAutoSetCapRect)(HANDLE hBoard, INT32 Width, INT32 Height);
BOOL	(WINAPI *lpfnOpenSetLUTDlg)(HANDLE hBoard, HWND hWnd, LPVOID lpLUT);

BOOL	(WINAPI *lpfnUnifyFields)(HANDLE hBoard, TARGET target, MLONG start);
INT32	(WINAPI *lpfnDiffusionFilter)(HANDLE hBoard, TARGET target, MLONG start, SHORT nLoop);
INT32	(WINAPI *lpfnGuassFilter)(HANDLE hBoard, TARGET target, MLONG start);
INT32	(WINAPI *lpfnSharpFilter)(HANDLE hBoard, TARGET target, MLONG start, SHORT iClass);
float	(WINAPI *lpfnGetFocusMeasure)(HANDLE hBoard, TARGET target, MLONG start, LPRECT lpRect, SHORT iMethod, SHORT iPreDegree);
INT32	(WINAPI *lpfnMultiFrmMean)(HANDLE hBoard, TARGET target, MLONG start, MLONG number);
INT32	(WINAPI *lpfnTakeRectMean)(HANDLE hBoard, TARGET target, LPRECT lpRect, MLONG start, short iMode);
INT32	(WINAPI *lpfnEvaluateHistogram)(HANDLE hBoard, TARGET target, LPRECT lpRect, MLONG start, LPDWORD lpHist);
INT32	(WINAPI *lpfnGetLinExtenMaplut)(LPDWORD lpHist, short iNumBits, short iNumChann, short iIntensity, LPWORD lpwLUT);
INT32	(WINAPI *lpfnGetHistEquaMaplut)(LPDWORD lpHist, short iNumBits, short iNumChann, LPWORD lpwLUT);

HANDLE	(WINAPI *lpfnBeginEncode)(HANDLE hBoard, DWORD dwCodeWay, MLONG lpImageInfo);
INT32	(WINAPI *lpfnEncodeImage)(HANDLE hCoder, TARGET src, MLONG start, LPBYTE lpData, MLONG maxlen);
INT32	(WINAPI *lpfnEndEncode)(HANDLE hCoder);


HANDLE	(WINAPI *lpfnBeginDecode)(HANDLE hBoard, DWORD dwCodeWay, LPBYTE lpData, MLONG lpImageInfo);
INT32	(WINAPI *lpfnDecodeImage)(HANDLE hCoder, LPBYTE lpData, MLONG *length, TARGET target, MLONG start);
INT32	(WINAPI *lpfnEndDecode)(HANDLE hCoder);


BOOL	(WINAPI *lpfnOutputByte)(WORD wport, BYTE data);
BYTE	(WINAPI *lpfnInputByte)(WORD wport);
BOOL	(WINAPI *lpfnOutputShort)(WORD wport, SHORT data);
SHORT	(WINAPI *lpfnInputShort)(WORD wport);
BOOL	(WINAPI *lpfnOutputLong)(WORD wport, MLONG data);
MLONG	(WINAPI *lpfnInputLong)(WORD wport);

BOOL	(WINAPI *lpfnSetPortBase)(WORD wPortBase, SHORT iPortCount);
SHORT	(WINAPI *lpfnGetGPIOPort)(SHORT index, WORD *wPortBase);


DWORD	(WINAPI *lpfnGetAddrForVB)(void *);
DWORD	(WINAPI *lpfnGetTickCount)(void );
void	(WINAPI *lpfnSleep)(DWORD dwMill);
DWORD	(WINAPI *lpfnGetTickCountMicro)(LPDWORD lpHiDword);
void	(WINAPI *lpfnSleepMicro)(DWORD dwMicro);


//------audio section
HANDLE	(WINAPI *lpfnOpenAudio)(HANDLE hBoard, MLONG lParam);
BOOL	(WINAPI *lpfnCloseAudio)(HANDLE hAudio);
MLONG	(WINAPI *lpfnCaptureAudio)(HANDLE hAudio, TARGET target, FARPROC lpfnUserProc, MLONG lParam);

BOOL	(WINAPI *lpfnStopCaptureAudio)(HANDLE hAudio);

INT32	(WINAPI *lpfnSetAudioParam)(HANDLE hAudio, INT32 wParam, INT32 lParam);
INT32	(WINAPI *lpfnReadAudioData)(HANDLE hAudio, LPBYTE lpAudioBuf, INT32 lReadSize);


//------stream section
HANDLE	(WINAPI *lpfnOpenStream)(HANDLE hBoard, DWORD lParam);
BOOL	(WINAPI *lpfnCloseStream)(HANDLE hStream);
HANDLE	(WINAPI *lpfnCaptureStream)(HANDLE hStream, TARGET target, FARPROC lpfnUserProc, MLONG lMiliseconds);
BOOL	(WINAPI *lpfnStopCaptureStream)(HANDLE hStream, DWORD dwPause);

MLONG	(WINAPI *lpfnSetStreamParam)(HANDLE hStream, INT32 wParam, MLONG lParam);
INT32	(WINAPI *lpfnReadStreamData)(HANDLE hStream, LPBYTE lpStreamBuf, INT32 lReadSize);
MLONG	(WINAPI	*lpfnGetCurrStream)(HANDLE hStream, LPVOID lParam);

//-------serial section
INT32	(WINAPI *lpfnSetSerial)(HANDLE hBoard, INT32 wParam, INT32 lParam);
INT32	(WINAPI *lpfnReadSerial)(HANDLE hBoard, LPVOID lpBuffer, INT32 lSize, MLONG lTimeOut);
BOOL	(WINAPI *lpfnWriteSerial)(HANDLE hBoard, LPVOID lpBuffer, INT32 lSize, MLONG lTimeOut);


/*-----------------------------------------------------------------------------
*       Functions Process
*/


//2. section

static int		iLoadNum=0;
static HANDLE	hDLLDrv;

BOOL LoadOkApiDll(BOOL bCount)
{
	if(!hDLLDrv)
		hDLLDrv=LoadLibrary(TEXT("OKAPI32.DLL"));

	if(!hDLLDrv) //not found
		return FALSE;

	if(iLoadNum==0) {
		(FARPROC )lpfnOpenBoard=GetProcAddress(hDLLDrv,"okOpenBoard");
		(FARPROC )lpfnOpenBoardEx=GetProcAddress(hDLLDrv,"okOpenBoardEx");
		(FARPROC )lpfnCloseBoard=GetProcAddress(hDLLDrv,"okCloseBoard");
		(FARPROC )lpfnGetLastError=GetProcAddress(hDLLDrv,"okGetLastError");
		(FARPROC )lpfnGetBufferSize=GetProcAddress(hDLLDrv,"okGetBufferSize");
		(FARPROC )lpfnGetBufferAddr=GetProcAddress(hDLLDrv,"okGetBufferAddr");
		(FARPROC )lpfnGetTargetInfo=GetProcAddress(hDLLDrv,"okGetTargetInfo");

		(FARPROC )lpfnGetTypeCode=GetProcAddress(hDLLDrv,"okGetTypeCode");
		(FARPROC )lpfnGetNetDevNumber=GetProcAddress(hDLLDrv,"okGetNetDevNumber");

		(FARPROC )lpfnSetTargetRect=GetProcAddress(hDLLDrv,"okSetTargetRect");
		(FARPROC )lpfnSetToWndRect=GetProcAddress(hDLLDrv,"okSetToWndRect");
		(FARPROC )lpfnSetVideoParam=GetProcAddress(hDLLDrv,"okSetVideoParam");
		(FARPROC )lpfnSetCaptureParam=GetProcAddress(hDLLDrv,"okSetCaptureParam");
		(FARPROC )lpfnSetDeviceParam=GetProcAddress(hDLLDrv,"okSetDeviceParam");

		(FARPROC )lpfnCaptureTo=GetProcAddress(hDLLDrv,"okCaptureTo");
		(FARPROC )lpfnCaptureSingle=GetProcAddress(hDLLDrv,"okCaptureSingle");
		(FARPROC )lpfnCaptureActive=GetProcAddress(hDLLDrv,"okCaptureActive");
		(FARPROC )lpfnCaptureThread=GetProcAddress(hDLLDrv,"okCaptureThread");
		(FARPROC )lpfnCaptureSequence=GetProcAddress(hDLLDrv,"okCaptureSequence");
		(FARPROC )lpfnCaptureToScreen=GetProcAddress(hDLLDrv,"okCaptureToScreen");
		(FARPROC )lpfnPlaybackFrom=GetProcAddress(hDLLDrv,"okPlaybackFrom");
		(FARPROC )lpfnPlaybackSequence=GetProcAddress(hDLLDrv,"okPlaybackSequence");
		(FARPROC )lpfnSetDriverParam=GetProcAddress(hDLLDrv,"okSetDriverParam");
		(FARPROC )lpfnGetCaptureStatus=GetProcAddress(hDLLDrv,"okGetCaptureStatus");
		(FARPROC )lpfnGetLineReady=GetProcAddress(hDLLDrv,"okGetLineReady");

		(FARPROC )lpfnStopCapture=GetProcAddress(hDLLDrv,"okStopCapture");

		(FARPROC )lpfnGetSeqCapture=GetProcAddress(hDLLDrv,"okGetSeqCapture");

		(FARPROC )lpfnCaptureByBuffer=GetProcAddress(hDLLDrv,"okCaptureByBuffer");
		(FARPROC )lpfnCaptureByBufferEx=GetProcAddress(hDLLDrv,"okCaptureByBufferEx");
		(FARPROC )lpfnPlaybackByBuffer=GetProcAddress(hDLLDrv,"okPlaybackByBuffer");

		(FARPROC )lpfnReadPixel=GetProcAddress(hDLLDrv,"okReadPixel");
		(FARPROC )lpfnWritePixel=GetProcAddress(hDLLDrv,"okWritePixel");
		(FARPROC )lpfnSetConvertParam=GetProcAddress(hDLLDrv,"okSetConvertParam");
		(FARPROC )lpfnReadRect=GetProcAddress(hDLLDrv,"okReadRect");
		(FARPROC )lpfnWriteRect=GetProcAddress(hDLLDrv,"okWriteRect");
		(FARPROC )lpfnTransferRect=GetProcAddress(hDLLDrv,"okTransferRect");
		(FARPROC )lpfnConvertRect=GetProcAddress(hDLLDrv,"okConvertRect");
		(FARPROC )lpfnConvertRectEx=GetProcAddress(hDLLDrv,"okConvertRectEx");
		(FARPROC )lpfnReadRectEx=GetProcAddress(hDLLDrv,"okReadRectEx");
		(FARPROC )lpfnWriteRectEx=GetProcAddress(hDLLDrv,"okWriteRectEx");

		(FARPROC )lpfnSetTextTo=GetProcAddress(hDLLDrv,"okSetTextTo");
		(FARPROC )lpfnDrawLineTo=GetProcAddress(hDLLDrv,"okDrawLineTo");
		(FARPROC )lpfnDrawEllipsTo=GetProcAddress(hDLLDrv,"okDrawEllipsTo");
		(FARPROC )lpfnFillCircleTo=GetProcAddress(hDLLDrv,"okFillCircleTo");

		(FARPROC )lpfnCreateDCBitmap=GetProcAddress(hDLLDrv,"okCreateDCBitmap");
		(FARPROC )lpfnMapDCBitmapTo=GetProcAddress(hDLLDrv,"okMapDCBitmapTo");
		(FARPROC )lpfnFreeDCBitmap=GetProcAddress(hDLLDrv,"okFreeDCBitmap");

		(FARPROC )lpfnWaitVerticalSync=GetProcAddress(hDLLDrv,"okWaitVerticalSync");


		(FARPROC )lpfnGetSignalParam=GetProcAddress(hDLLDrv,"okGetSignalParam");
		(FARPROC )lpfnWaitSignalEvent=GetProcAddress(hDLLDrv,"okWaitSignalEvent");
		(FARPROC )lpfnPutSignalParam=GetProcAddress(hDLLDrv,"okPutSignalParam");

		(FARPROC )lpfnSetUserData=GetProcAddress(hDLLDrv,"okSetUserData");
		(FARPROC )lpfnSetSeqProcWnd=GetProcAddress(hDLLDrv,"okSetSeqProcWnd");

		(FARPROC )lpfnSetSeqCallback=GetProcAddress(hDLLDrv,"okSetSeqCallback");
		(FARPROC )lpfnSetEventCallback=GetProcAddress(hDLLDrv,"okSetEventCallback");
		(FARPROC )lpfnSetCloseCallback=GetProcAddress(hDLLDrv,"okSetCloseCallback");

		(FARPROC )lpfnEnableMask=GetProcAddress(hDLLDrv,"okEnableMask");
		(FARPROC )lpfnSetMaskRect=GetProcAddress(hDLLDrv,"okSetMaskRect");
		(FARPROC )lpfnFillOutLUT=GetProcAddress(hDLLDrv,"okFillOutLUT");
		(FARPROC )lpfnFillInputLUT=GetProcAddress(hDLLDrv,"okFillInputLUT");

		(FARPROC )lpfnGetImageDevice=GetProcAddress(hDLLDrv,"okGetImageDevice");
		(FARPROC )lpfnGetSlotBoard=GetProcAddress(hDLLDrv,"okGetSlotBoard");
		(FARPROC )lpfnGetBoardIndex=GetProcAddress(hDLLDrv,"okGetBoardIndex");
		(FARPROC )lpfnGetBoardName=GetProcAddress(hDLLDrv,"okGetBoardName");

		(FARPROC )lpfnGetAvailBuffer=GetProcAddress(hDLLDrv,"okGetAvailBuffer");
		(FARPROC )lpfnLockBuffer=GetProcAddress(hDLLDrv,"okLockBuffer");
		(FARPROC )lpfnUnlockAllBuffer=GetProcAddress(hDLLDrv,"okUnlockAllBuffer");

		(FARPROC )lpfnApplyMemBlock=GetProcAddress(hDLLDrv,"okApplyMemBlock");
		(FARPROC )lpfnFreeMemBlock=GetProcAddress(hDLLDrv,"okFreeMemBlock");
		(FARPROC )lpfnGetMemBlock=GetProcAddress(hDLLDrv,"okGetMemBlock");
		(FARPROC )lpfnLockMemBlock=GetProcAddress(hDLLDrv,"okLockMemBlock");
		(FARPROC )lpfnUnlockMemBlock=GetProcAddress(hDLLDrv,"okUnlockMemBlock");

		(FARPROC )lpfnMulCaptureTo=GetProcAddress(hDLLDrv,"okMulCaptureTo");
		(FARPROC )lpfnMulCaptureByBuffer=GetProcAddress(hDLLDrv,"okMulCaptureByBuffer");

		(FARPROC )lpfnSaveInitParam=GetProcAddress(hDLLDrv,"okSaveInitParam");
		(FARPROC )lpfnLoadInitParam=GetProcAddress(hDLLDrv,"okLoadInitParam");

		(FARPROC )lpfnSetAllocBuffer=GetProcAddress(hDLLDrv,"okSetAllocBuffer");
		(FARPROC )lpfnSetStaticVxD=GetProcAddress(hDLLDrv,"okSetStaticVxD");
		(FARPROC )lpfnSetNTDriver=GetProcAddress(hDLLDrv,"okSetNTDriver");
		(FARPROC )lpfnSetLangResource=GetProcAddress(hDLLDrv,"okSetLangResource");


		(FARPROC )lpfnUnRegister=GetProcAddress(hDLLDrv,"okUnRegister");
		(FARPROC )lpfnGetProgramInfo=GetProcAddress(hDLLDrv,"okGetProgramInfo");

		(FARPROC )lpfnReadProtCode=GetProcAddress(hDLLDrv,"okReadProtCode");
		(FARPROC )lpfnWriteProtCode=GetProcAddress(hDLLDrv,"okWriteProtCode");

		(FARPROC )lpfnSaveImageFile=GetProcAddress(hDLLDrv,"okSaveImageFile");
		(FARPROC )lpfnLoadImageFile=GetProcAddress(hDLLDrv,"okLoadImageFile");
		(FARPROC )lpfnGetCurrImageInfo=GetProcAddress(hDLLDrv,"okGetCurrImageInfo");


		(FARPROC )lpfnLoadConfigFile=GetProcAddress(hDLLDrv,"okLoadConfigFile");
		(FARPROC )lpfnSaveConfigFile=GetProcAddress(hDLLDrv,"okSaveConfigFile");

		(FARPROC )lpfnLoadFlatModelFile=GetProcAddress(hDLLDrv,"okLoadFlatModelFile");
		(FARPROC )lpfnSaveFlatModelFile=GetProcAddress(hDLLDrv,"okSaveFlatModelFile");

		(FARPROC )lpfnOpenSetParamDlg=GetProcAddress(hDLLDrv,"okOpenSetParamDlg");
		(FARPROC )lpfnOpenSeqCaptureDlg=GetProcAddress(hDLLDrv,"okOpenSeqCaptureDlg");
		(FARPROC )lpfnOpenReplayDlg=GetProcAddress(hDLLDrv,"okOpenReplayDlg");
		(FARPROC )lpfnOpenReplayDlgEx=GetProcAddress(hDLLDrv,"okOpenReplayDlgEx");
		(FARPROC )lpfnAutoAdjustBright=GetProcAddress(hDLLDrv,"okAutoAdjustBright");
		(FARPROC )lpfnAutoSetCapRect=GetProcAddress(hDLLDrv,"okAutoSetCapRect");
		(FARPROC )lpfnOpenSetLUTDlg=GetProcAddress(hDLLDrv,"okOpenSetLUTDlg");

		(FARPROC )lpfnUnifyFields=GetProcAddress(hDLLDrv,"okUnifyFields");
		(FARPROC )lpfnDiffusionFilter=GetProcAddress(hDLLDrv,"okDiffusionFilter");
		(FARPROC )lpfnGuassFilter=GetProcAddress(hDLLDrv,"okGuassFilter");
		(FARPROC )lpfnSharpFilter=GetProcAddress(hDLLDrv,"okSharpFilter");
		(FARPROC )lpfnGetFocusMeasure=GetProcAddress(hDLLDrv,"okGetFocusMeasure");
		(FARPROC )lpfnMultiFrmMean=GetProcAddress(hDLLDrv,"okMultiFrmMean");
		(FARPROC )lpfnTakeRectMean=GetProcAddress(hDLLDrv,"okTakeRectMean");
		(FARPROC )lpfnEvaluateHistogram=GetProcAddress(hDLLDrv,"okEvaluateHistogram");
		(FARPROC )lpfnGetLinExtenMaplut=GetProcAddress(hDLLDrv,"okGetLinExtenMaplut");
		(FARPROC )lpfnGetHistEquaMaplut=GetProcAddress(hDLLDrv,"okGetHistEquaMaplut");

		(FARPROC )lpfnBeginEncode=GetProcAddress(hDLLDrv,"okBeginEncode");
		(FARPROC )lpfnEncodeImage=GetProcAddress(hDLLDrv,"okEncodeImage");
		(FARPROC )lpfnEndEncode=GetProcAddress(hDLLDrv,"okEndEncode");
		(FARPROC )lpfnBeginDecode=GetProcAddress(hDLLDrv,"okBeginDecode");
		(FARPROC )lpfnDecodeImage=GetProcAddress(hDLLDrv,"okDecodeImage");
		(FARPROC )lpfnEndDecode=GetProcAddress(hDLLDrv,"okEndDecode");


		(FARPROC )lpfnOutputByte=GetProcAddress(hDLLDrv,"okOutputByte");
		(FARPROC )lpfnInputByte=GetProcAddress(hDLLDrv,"okInputByte");
		(FARPROC )lpfnOutputShort=GetProcAddress(hDLLDrv,"okOutputShort");
		(FARPROC )lpfnInputShort=GetProcAddress(hDLLDrv,"okInputShort");
		(FARPROC )lpfnOutputLong=GetProcAddress(hDLLDrv,"okOutputLong");
		(FARPROC )lpfnInputLong=GetProcAddress(hDLLDrv,"okInputLong");

		(FARPROC )lpfnSetPortBase=GetProcAddress(hDLLDrv,"okSetPortBase");
		(FARPROC )lpfnGetGPIOPort=GetProcAddress(hDLLDrv,"okGetGPIOPort");

		(FARPROC )lpfnGetAddrForVB=GetProcAddress(hDLLDrv,"okGetAddrForVB");
		(FARPROC )lpfnGetTickCount=GetProcAddress(hDLLDrv,"okGetTickCount");
		(FARPROC )lpfnSleep=GetProcAddress(hDLLDrv,"okSleep");
		(FARPROC )lpfnGetTickCountMicro=GetProcAddress(hDLLDrv,"okGetTickCountMicro");
		(FARPROC )lpfnSleepMicro=GetProcAddress(hDLLDrv,"okSleepMicro");


		//---audio
		(FARPROC )lpfnOpenAudio=GetProcAddress(hDLLDrv,"okOpenAudio");
		(FARPROC )lpfnCloseAudio=GetProcAddress(hDLLDrv,"okCloseAudio");
		(FARPROC )lpfnCaptureAudio=GetProcAddress(hDLLDrv,"okCaptureAudio");
		(FARPROC )lpfnStopCaptureAudio=GetProcAddress(hDLLDrv,"okStopCaptureAudio");
		(FARPROC )lpfnSetAudioParam=GetProcAddress(hDLLDrv,"okSetAudioParam");
		(FARPROC )lpfnReadAudioData=GetProcAddress(hDLLDrv,"okReadAudioData");

		//---stream
		(FARPROC )lpfnOpenStream=GetProcAddress(hDLLDrv,"okOpenStream");
		(FARPROC )lpfnCloseStream=GetProcAddress(hDLLDrv,"okCloseStream");
		(FARPROC )lpfnCaptureStream=GetProcAddress(hDLLDrv,"okCaptureStream");
		(FARPROC )lpfnStopCaptureStream=GetProcAddress(hDLLDrv,"okStopCaptureStream");
		(FARPROC )lpfnSetStreamParam=GetProcAddress(hDLLDrv,"okSetStreamParam");
		(FARPROC )lpfnReadStreamData=GetProcAddress(hDLLDrv,"okReadStreamData");
		(FARPROC )lpfnGetCurrStream=GetProcAddress(hDLLDrv,"okGetCurrStream");

		//---serial
		(FARPROC )lpfnSetSerial=GetProcAddress(hDLLDrv,"okSetSerial");
		(FARPROC )lpfnReadSerial=GetProcAddress(hDLLDrv,"okReadSerial");
		(FARPROC )lpfnWriteSerial=GetProcAddress(hDLLDrv,"okWriteSerial");
		
	}

	if(bCount || (iLoadNum==0) )
	//if( (iLoadNum==0) )
		iLoadNum++;

	return TRUE;
}

FreeOkApiDll()
{
	iLoadNum--;
	if(iLoadNum<=0) {
		FreeLibrary(hDLLDrv);
		hDLLDrv=0;
	}
	return 1;
}

FARPROC GetProcPointer(LPSTR lpszName)
{
	if(!hDLLDrv)
		hDLLDrv=LoadLibrary("OKAPI32.DLL");

	return GetProcAddress(hDLLDrv,lpszName);
}


HANDLE	WINAPI okOpenBoard(MLONG *iIndex) 
{
	LoadOkApiDll(1);

	return lpfnOpenBoard(iIndex);
}

HANDLE	WINAPI okOpenBoardEx(MLONG *iIndex, HANDLE hParBoard, LPVOID lParam) 
{
	LoadOkApiDll(1);

	return lpfnOpenBoardEx(iIndex, hParBoard, lParam);
}


BOOL	WINAPI okCloseBoard(HANDLE hBoard)
{
	BOOL	ret=0;

	if(hDLLDrv)
		ret=lpfnCloseBoard(hBoard);

	if(ret)
		FreeOkApiDll();

	return ret;
}


//3. sections

INT32	WINAPI okGetLastError(void )
{	
	(FARPROC )lpfnGetLastError=GetProcPointer("okGetLastError");
	return lpfnGetLastError();	}

MLONG	WINAPI okGetBufferSize(HANDLE hBoard, void **lpLinear, MWORD *dwSize)
{	
	(FARPROC )lpfnGetBufferSize=GetProcPointer("okGetBufferSize");
	return lpfnGetBufferSize(hBoard, lpLinear, dwSize);	}


LPVOID	WINAPI okGetBufferAddr(HANDLE hBoard, MLONG iNoFrame)
{	return lpfnGetBufferAddr(hBoard, iNoFrame);	}

LPVOID	WINAPI okGetTargetInfo(HANDLE hBoard, TARGET tgt, MLONG lNoFrame, SHORT * wid, SHORT * ht, MLONG *stride)
{	return lpfnGetTargetInfo(hBoard, tgt, lNoFrame, wid, ht, stride);	}

INT32	WINAPI okGetTypeCode(HANDLE hBoard,LPSTR lpBoardName)
{	return lpfnGetTypeCode(hBoard,lpBoardName);	}

INT32	WINAPI okGetNetDevNumber(INT32 iTypeCode, LPSTR lpNameString)
{	return lpfnGetNetDevNumber(iTypeCode,lpNameString);}

MLONG	WINAPI okSetTargetRect(HANDLE hBoard, TARGET target, LPRECT lpTgtRect)
{	return lpfnSetTargetRect(hBoard, target, lpTgtRect);	}

INT32	WINAPI okSetToWndRect(HANDLE hBoard, HWND hWnd)
{	return lpfnSetToWndRect(hBoard, hWnd);	}

MLONG	WINAPI okCaptureTo(HANDLE hBoard, TARGET Dest, MLONG start, MLONG lParam)
{	return lpfnCaptureTo(hBoard, Dest, start, lParam);	}

BOOL	WINAPI okCaptureSingle(HANDLE hBoard, TARGET Dest, MLONG lStart)
{	return lpfnCaptureSingle(hBoard, Dest, lStart);	}

BOOL	WINAPI okCaptureActive(HANDLE hBoard, TARGET Dest, MLONG lStart)
{	return lpfnCaptureActive(hBoard, Dest, lStart);	}

HANDLE	WINAPI okCaptureThread(HANDLE hBoard, TARGET Dest, MLONG lStart, MLONG lNoFrame)
{	return lpfnCaptureThread(hBoard, Dest, lStart, lNoFrame);	}

BOOL	WINAPI okCaptureSequence(HANDLE hBoard, MLONG lStart, MLONG lNoFrame)
{	return lpfnCaptureSequence(hBoard, lStart, lNoFrame);	}

MLONG	WINAPI okCaptureToScreen(HANDLE hBoard)
{	return lpfnCaptureToScreen(hBoard);	}

MLONG	WINAPI okPlaybackFrom(HANDLE hBoard, TARGET src, MLONG start, MLONG lParam)
{	return lpfnPlaybackFrom(hBoard, src, start, lParam);	}

BOOL	WINAPI okPlaybackSequence(HANDLE hBoard, MLONG lstart, MLONG lNoFrame)
{	return lpfnPlaybackSequence(hBoard, lstart, lNoFrame);	}

MLONG	WINAPI okSetDriverParam(INT32 lWhich, MLONG lParam)
{	return lpfnSetDriverParam(lWhich, lParam);}

MLONG	WINAPI okGetCaptureStatus(HANDLE hBoard, BOOL bWait)
{	return lpfnGetCaptureStatus(hBoard, bWait);	}

MLONG	WINAPI okGetLineReady(HANDLE hBoard, MLONG *iCurrFrameNo, MWORD dwRes)
{	return lpfnGetLineReady(hBoard, iCurrFrameNo, dwRes);	}



MLONG	WINAPI okStopCapture(HANDLE hBoard)
{	return lpfnStopCapture(hBoard);	}

HANDLE	WINAPI okCaptureByBuffer(HANDLE hBoard, TARGET dest, MLONG start, MLONG num)
{	return lpfnCaptureByBuffer(hBoard, dest, start, num);	}

MLONG	WINAPI okGetSeqCapture(HANDLE hBoard, MLONG start, MLONG count)
{	return lpfnGetSeqCapture(hBoard, start, count);	}


HANDLE	WINAPI okCaptureByBufferEx(HANDLE hBoard, MLONG fileset, TARGET dest, MLONG start, MLONG num)
{	return lpfnCaptureByBufferEx(hBoard, fileset, dest, start, num);	}

HANDLE	WINAPI okPlaybackByBuffer(HANDLE hBoard, TARGET src, MLONG start, MLONG num)
{	return lpfnPlaybackByBuffer(hBoard, src, start, num);	}

MLONG	WINAPI okSetVideoParam(HANDLE hBoard, INT32 wParam, MLONG lParam)
{	return lpfnSetVideoParam(hBoard, wParam, lParam);	}

MLONG	WINAPI okSetCaptureParam(HANDLE hBoard, INT32 wParam, MLONG lParam)
{	return lpfnSetCaptureParam(hBoard, wParam, lParam);	}

MLONG	WINAPI okSetDeviceParam(HANDLE hBoard, INT32 wParam, MLONG lParam)
{	return lpfnSetDeviceParam(hBoard, wParam, lParam);	}


INT32	WINAPI okReadPixel(HANDLE  hBoard, TARGET src, MLONG start, SHORT x, SHORT y)
{	return lpfnReadPixel(hBoard, src, start, x, y);	}

INT32	WINAPI okWritePixel(HANDLE  hBoard, TARGET src, MLONG start, SHORT x, SHORT y, INT32 lvalue)
{	return lpfnWritePixel(hBoard, src, start, x, y, lvalue);	}

MLONG	WINAPI okSetConvertParam(HANDLE hBoard, INT32 wParam, MLONG lParam)
{	return lpfnSetConvertParam(hBoard, wParam, lParam);	}

INT32	WINAPI okReadRect(HANDLE  hBoard, TARGET src, MLONG start, LPBYTE lpBuf)
{	return lpfnReadRect(hBoard, src, start, lpBuf);	}

INT32	WINAPI okWriteRect(HANDLE  hBoard, TARGET dst, MLONG start, LPBYTE lpBuf)
{	return lpfnWriteRect(hBoard, dst, start, lpBuf);	}

INT32	WINAPI okTransferRect(HANDLE hBoard, TARGET dest, MLONG iFirst, TARGET src, MLONG iStart, MLONG lNum)
{	return lpfnTransferRect(hBoard, dest, iFirst, src, iStart, lNum);	}

INT32	WINAPI okConvertRect(HANDLE hBoard, TARGET dst, MLONG first, TARGET src, MLONG start, MLONG no)
{	return lpfnConvertRect(hBoard, dst, first, src, start, no);	}

INT32	WINAPI okConvertRectEx(HANDLE hDstBoard, TARGET dst, MLONG first, HANDLE hSrcBoard, TARGET src, MLONG start, MLONG no)
{	return lpfnConvertRectEx(hDstBoard, dst, first, hSrcBoard, src, start, no);	}

INT32	WINAPI okReadRectEx(HANDLE hBoard, TARGET src, MLONG start, LPBYTE lpBuf, INT32 lParam)
{	return lpfnReadRectEx(hBoard, src, start, lpBuf, lParam);	}

INT32	WINAPI okWriteRectEx(HANDLE hBoard, TARGET dst, MLONG start, LPBYTE lpBuf, INT32 lParam)
{	return lpfnWriteRectEx(hBoard, dst, start, lpBuf, lParam);	}


BOOL	WINAPI okSetTextTo(HANDLE hBoard, TARGET target, LPRECT lpRect, LOGFONT *lfLogFont, SETTEXTMODE *textMode, LPSTR lpString, INT32 lLength)
{	return lpfnSetTextTo( hBoard, target, lpRect, lfLogFont, textMode, lpString, lLength);	}

INT32	WINAPI okDrawLineTo(HANDLE hBoard, TARGET target, MLONG lStart, POINT pt0, POINT pt1, INT32 iForeColor)
{	return lpfnDrawLineTo(hBoard, target, lStart, pt0, pt1, iForeColor);	}

INT32	WINAPI okDrawEllipsTo(HANDLE hBoard, TARGET target, MLONG lStart, LPRECT lpRect, INT32 iForeColor)
{	return lpfnDrawEllipsTo(hBoard, target, lStart, lpRect, iForeColor);	}

INT32	WINAPI okFillCircleTo(HANDLE hBoard, TARGET target, MLONG lStart, LPRECT lpRect, INT32 iForeColor, INT32 lMode)
{	return lpfnFillCircleTo(hBoard, target, lStart, lpRect, iForeColor, lMode);	}


HDC		WINAPI okCreateDCBitmap(HANDLE hBoard, TARGET target, HANDLE *hDCBitmap)
{	return lpfnCreateDCBitmap(hBoard, target, hDCBitmap);	}

BOOL	WINAPI okMapDCBitmapTo(HANDLE hDCBitmap, MLONG lStart)
{	return lpfnMapDCBitmapTo(hDCBitmap, lStart);	}

BOOL	WINAPI okFreeDCBitmap(HANDLE hDCBitmap)
{	return lpfnFreeDCBitmap(hDCBitmap);	}

BOOL	WINAPI okWaitVerticalSync(HANDLE hBoard, BOOL bHeader)
{	return lpfnWaitVerticalSync(hBoard, bHeader);	}



INT32	WINAPI okGetSignalParam(HANDLE hBoard, INT32 wParam)
{	return lpfnGetSignalParam(hBoard, wParam);	}

INT32	WINAPI okWaitSignalEvent(HANDLE hBoard, INT32 wParam, MLONG lMilliSecond)
{	return lpfnWaitSignalEvent(hBoard, wParam, lMilliSecond);	}

MLONG	WINAPI okPutSignalParam(HANDLE hBoard, INT32 wParam, MLONG lParam)
{	return lpfnPutSignalParam(hBoard, wParam, lParam);	}


MLONG	WINAPI okSetUserData(HANDLE hBoard, MLONG lUserData)
{	return lpfnSetUserData(hBoard, lUserData);	}

BOOL	WINAPI okSetSeqProcWnd(HANDLE hBoard, HWND hwndMain)
{	return lpfnSetSeqProcWnd(hBoard, hwndMain);	}


BOOL	WINAPI okSetSeqCallback(HANDLE hBoard, 
								BOOL CALLBACK BeginProc(HANDLE hBoard), 
								BOOL CALLBACK SeqProc(HANDLE hBoard, MLONG No), 
								BOOL CALLBACK EndProc(HANDLE hBoard))
{	return lpfnSetSeqCallback(hBoard, BeginProc, SeqProc, EndProc);	}

BOOL	WINAPI okSetEventCallback(HANDLE hBoard, INT32 iEvent, BOOL CALLBACK EventProc(HANDLE hBoard, MWORD dwReserved) )
{
	return lpfnSetEventCallback(hBoard,iEvent,EventProc);
}

BOOL	WINAPI okSetCloseCallback(HANDLE hBoard,
								  BOOL CALLBACK CloseProc(HANDLE hBoard, MWORD dwReserved) 	)
{
	return lpfnSetCloseCallback(hBoard, CloseProc);	
}


INT32	WINAPI okEnableMask(HANDLE hBoard, BOOL bMask)
{	return lpfnEnableMask(hBoard, bMask);	}

INT32	WINAPI okSetMaskRect(HANDLE hBoard, LPRECT lpRect, LPBYTE lpMask)
{	return lpfnSetMaskRect(hBoard, lpRect, lpMask);	}


INT32	WINAPI okFillOutLUT(HANDLE hBoard,  LPVOID bLUT, INT32 start, INT32 num)
{	return lpfnFillOutLUT(hBoard,  bLUT, start, num);	}

INT32	WINAPI okFillInputLUT(HANDLE hBoard,  LPVOID bLUT, INT32 start, INT32 num)
{	return lpfnFillInputLUT(hBoard,  bLUT, start, num);	}



INT32	WINAPI okGetImageDevice(OKDEVTYPE **lpOkDevInfo, MLONG lpParam)
{
	(FARPROC )lpfnGetImageDevice=GetProcPointer("okGetImageDevice");
	return lpfnGetImageDevice(lpOkDevInfo, lpParam);	}

SHORT	WINAPI okGetSlotBoard(BOARDTYPE **lpOkInfo)
{
	(FARPROC )lpfnGetSlotBoard=GetProcPointer("okGetSlotBoard");
	return lpfnGetSlotBoard(lpOkInfo);	}

SHORT	WINAPI okGetBoardIndex(CHAR *szBoardName, SHORT iNo)
{
	(FARPROC )lpfnGetBoardIndex=GetProcPointer("okGetBoardIndex");
	return lpfnGetBoardIndex(szBoardName, iNo);	}


INT32	WINAPI okGetBoardName(SHORT lIndex, LPSTR szBoardName)
{
	(FARPROC )lpfnGetBoardName=GetProcPointer("okGetBoardName");
	return lpfnGetBoardName(lIndex, szBoardName);	}


MLONG	WINAPI okGetAvailBuffer(void **lpLinear, MWORD *dwSize)
{
	(FARPROC )lpfnGetAvailBuffer=GetProcPointer("okGetAvailBuffer");
	return lpfnGetAvailBuffer(lpLinear, dwSize);	}

MLONG	WINAPI okLockBuffer(HANDLE hBoard, MWORD dwSizeByte, void **lpBasLinear)
{	return lpfnLockBuffer(hBoard, dwSizeByte, lpBasLinear);	}

BOOL	WINAPI okUnlockAllBuffer(void)
{	return lpfnUnlockAllBuffer();	}


INT32	WINAPI okApplyMemBlock(DWORD dwBlockSize, DWORD dwBlockNo)
{	(FARPROC )lpfnApplyMemBlock=GetProcPointer("okApplyMemBlock");
	return lpfnApplyMemBlock(dwBlockSize, dwBlockNo);	}

BOOL	WINAPI okFreeMemBlock(void)
{	return lpfnFreeMemBlock();	}

INT32	WINAPI okGetMemBlock(HANDLE hBoard, DWORD *dwEachSize,  DWORD *dwBlockNo)
{	return lpfnGetMemBlock(hBoard, dwEachSize,  dwBlockNo);	}

INT32	WINAPI okLockMemBlock(HANDLE hBoard, DWORD dwBlockNo)
{	return lpfnLockMemBlock(hBoard, dwBlockNo);	}

BOOL	WINAPI okUnlockMemBlock(void)
{	return lpfnUnlockMemBlock();	}



MLONG	WINAPI okMulCaptureTo(HANDLE *lphBoard, TARGET Dest, MLONG start, MLONG lParam)
{	return lpfnMulCaptureTo(lphBoard, Dest, start, lParam);	}

HANDLE	WINAPI okMulCaptureByBuffer(HANDLE *lphBoard, TARGET tgt, MLONG start, MLONG num)
{	return lpfnMulCaptureByBuffer(lphBoard, tgt, start, num);	}


MLONG	WINAPI okSetAllocBuffer(MLONG lSize)
{
	(FARPROC )lpfnSetAllocBuffer=GetProcPointer("okSetAllocBuffer");
	return lpfnSetAllocBuffer(lSize);	
}

BOOL	WINAPI okSetStaticVxD(INT32 lMode)
{
	(FARPROC )lpfnSetStaticVxD=GetProcPointer("okSetStaticVxD");
	return lpfnSetStaticVxD(lMode);	
}

BOOL	WINAPI okSetNTDriver(BOOL bCmd)
{
	(FARPROC )lpfnSetNTDriver=GetProcPointer("okSetNTDriver");
	return lpfnSetNTDriver(bCmd);	
}

INT32	WINAPI okSetLangResource(INT32 langcode)
{
	(FARPROC )lpfnSetLangResource=GetProcPointer("okSetLangResource");
	return lpfnSetLangResource(langcode);	
}


BOOL	WINAPI okUnRegister(DWORD dwCmd)
{	
	(FARPROC )lpfnUnRegister=GetProcPointer("okUnRegister");
	return lpfnUnRegister(dwCmd);	
}

INT32	WINAPI okGetProgramInfo(INT32 iItem, LPSTR lpString, INT32 iSize)
{	
	(FARPROC )lpfnGetProgramInfo=GetProcPointer("okGetProgramInfo");
	return lpfnGetProgramInfo(iItem, lpString, iSize);
}


BOOL	WINAPI okSaveInitParam(HANDLE hBoard, SHORT iChannNo)
{	return lpfnSaveInitParam(hBoard, iChannNo);	}

BOOL	WINAPI okLoadInitParam(HANDLE hBoard, SHORT iChannNo)
{	return lpfnLoadInitParam(hBoard, iChannNo);	}

INT32	WINAPI okReadProtCode(HANDLE hBoard, SHORT iIndex)
{	return lpfnReadProtCode(hBoard, iIndex);	}

INT32	WINAPI okWriteProtCode(HANDLE hBoard, SHORT iIndex, INT32 Code)
{	return lpfnWriteProtCode(hBoard, iIndex, Code);	}



MLONG WINAPI okSaveImageFile(HANDLE hBoard, LPSTR szFileName, MLONG first, TARGET target, MLONG start, MLONG num)
{	return lpfnSaveImageFile(hBoard, szFileName, first, target, start, num);	}

MLONG WINAPI okLoadImageFile(HANDLE hBoard, LPSTR szFileName, MLONG first, TARGET target, MLONG start, MLONG num)
{	return lpfnLoadImageFile(hBoard, szFileName, first, target, start, num);	}

BOOL WINAPI okGetCurrImageInfo(HANDLE hBoard, MLONG lpImgFrmInfo, MLONG lSize)
{	return lpfnGetCurrImageInfo(hBoard, lpImgFrmInfo, lSize);	}


BOOL WINAPI okLoadConfigFile(HANDLE hBoard, LPSTR szFileName)
{	return lpfnLoadConfigFile(hBoard, szFileName);	}

BOOL WINAPI okSaveConfigFile(HANDLE hBoard, LPSTR szFileName)
{	return lpfnSaveConfigFile(hBoard, szFileName);	}

BOOL WINAPI okLoadFlatModelFile(HANDLE hBoard, LPSTR szFileName)
{	return lpfnLoadFlatModelFile(hBoard, szFileName);	}

BOOL WINAPI okSaveFlatModelFile(HANDLE hBoard, LPSTR szFileName)
{	return lpfnSaveFlatModelFile(hBoard, szFileName);	}

 
BOOL WINAPI okOpenSetParamDlg( HANDLE hBoard, HWND hParentWnd)
{	return lpfnOpenSetParamDlg(hBoard, hParentWnd);	}

BOOL WINAPI okOpenSeqCaptureDlg( HANDLE hBoard, HWND hParentWnd)
{	return lpfnOpenSeqCaptureDlg(hBoard, hParentWnd);	}

LPDIBINFO WINAPI okOpenReplayDlg( HANDLE hBoard, HWND hWnd, TARGET src, MLONG total)
{	return lpfnOpenReplayDlg(hBoard, hWnd, src, total);	}

HWND WINAPI okOpenReplayDlgEx( HANDLE hBoard, HWND hWnd, TARGET src, MLONG total, LPBITMAPINFOHEADER lpbi, LPBYTE lpdib)
{	return lpfnOpenReplayDlgEx(hBoard, hWnd, src, total, lpbi, lpdib);	}

INT32	WINAPI okAutoAdjustBright(HANDLE hBoard, LPRECT lpRect, LPVOID dwRes)
{	return lpfnAutoAdjustBright(hBoard, lpRect, dwRes); }

INT32	WINAPI okAutoSetCapRect(HANDLE hBoard, INT32 Width, INT32 Height)
{	return lpfnAutoSetCapRect(hBoard, Width, Height); }

BOOL	WINAPI okOpenSetLUTDlg(HANDLE hBoard, HWND hWnd, LPVOID lpLUT)
{	return lpfnOpenSetLUTDlg(hBoard, hWnd, lpLUT); }

BOOL	WINAPI okUnifyFields(HANDLE hBoard, TARGET target, MLONG start)
{	return lpfnUnifyFields(hBoard, target, start); }

INT32	WINAPI okDiffusionFilter(HANDLE hBoard, TARGET target, MLONG start, SHORT nLoop)
{	return lpfnDiffusionFilter(hBoard, target, start, nLoop);  }

INT32	WINAPI okGuassFilter(HANDLE hBoard, TARGET target, MLONG start)
{	return lpfnGuassFilter(hBoard, target, start);  }

INT32	WINAPI okSharpFilter(HANDLE hBoard, TARGET target, MLONG start, SHORT iClass)
{	return lpfnSharpFilter(hBoard, target, start, iClass);   }

float	WINAPI okGetFocusMeasure(HANDLE hBoard, TARGET target, MLONG start, LPRECT lpRect, SHORT iMethod, SHORT iPreDegree)
{	return lpfnGetFocusMeasure(hBoard, target, start, lpRect, iMethod, iPreDegree);   }

INT32	WINAPI okMultiFrmMean(HANDLE hBoard, TARGET target, MLONG start, MLONG number)
{	return lpfnMultiFrmMean(hBoard, target, start, number);   }

INT32	WINAPI okTakeRectMean(HANDLE hBoard, TARGET target, LPRECT lpRect, MLONG start, short iMode)
{	return lpfnTakeRectMean(hBoard, target, lpRect, start, iMode);   }

INT32	WINAPI okEvaluateHistogram(HANDLE hBoard, TARGET target, LPRECT lpRect, MLONG start, LPDWORD lpHist)
{	return lpfnEvaluateHistogram(hBoard, target, lpRect, start, lpHist);   }

INT32	WINAPI okGetLinExtenMaplut(LPDWORD lpHist, short iNumBits, short iNumChann, short iIntensity, LPWORD lpwLUT)
{	return lpfnGetLinExtenMaplut(lpHist, iNumBits, iNumChann, iIntensity, lpwLUT);   }

INT32	WINAPI okGetHistEquaMaplut(LPDWORD lpHist, short iNumBits, short iNumChann, LPWORD lpwLUT)
{	return lpfnGetHistEquaMaplut(lpHist, iNumBits, iNumChann, lpwLUT);   }

HANDLE WINAPI okBeginEncode(HANDLE hBoard, DWORD dwCodeWay, MLONG lpImageInfo)
{	return lpfnBeginEncode(hBoard, dwCodeWay, lpImageInfo);	}

INT32 WINAPI okEncodeImage(HANDLE hCoder, TARGET src, MLONG start, LPBYTE lpData, MLONG maxlen)
{	return lpfnEncodeImage(hCoder, src, start, lpData, maxlen);	}

INT32 WINAPI okEndEncode(HANDLE hCoder)
{	return lpfnEndEncode(hCoder);	}


HANDLE WINAPI okBeginDecode(HANDLE hBoard, DWORD dwCodeWay, LPBYTE lpData, MLONG lpImageInfo)
{	return lpfnBeginDecode(hBoard, dwCodeWay, lpData, lpImageInfo);	}

INT32 WINAPI okDecodeImage(HANDLE hCoder, LPBYTE lpData, MLONG *length, TARGET target, MLONG start)
{	return lpfnDecodeImage(hCoder, lpData, length, target, start);	}

INT32 WINAPI okEndDecode(HANDLE hCoder)
{	return lpfnEndDecode(hCoder);	}



BOOL	WINAPI okOutputByte(WORD wport, BYTE data)
{	LoadOkApiDll(0);
	return lpfnOutputByte(wport, data);	}

BYTE	WINAPI okInputByte(WORD wport)
{	LoadOkApiDll(0);
	return lpfnInputByte(wport);	}

BOOL	WINAPI okOutputShort(WORD wport, SHORT data)
{	LoadOkApiDll(0);
	return lpfnOutputShort(wport, data);	}

SHORT	WINAPI okInputShort(WORD wport)
{	LoadOkApiDll(0);
	return lpfnInputShort(wport);	}

BOOL	WINAPI okOutputLong(WORD wport, MLONG data)
{	LoadOkApiDll(0);
	return lpfnOutputLong(wport, data);	}

MLONG	WINAPI okInputLong(WORD wport)
{	LoadOkApiDll(0);	
	return lpfnInputLong(wport);	}


BOOL	WINAPI okSetPortBase(WORD wPortBase, SHORT iPortCount)
{	LoadOkApiDll(0);
	return lpfnSetPortBase(wPortBase, iPortCount);	}

SHORT	WINAPI okGetGPIOPort(SHORT index, WORD *wPortBase)
{	LoadOkApiDll(0);
	return lpfnGetGPIOPort(index, wPortBase);	}
		


DWORD	WINAPI okGetAddrForVB(void *lpArry)
{	LoadOkApiDll(0);
	return lpfnGetAddrForVB(lpArry);	}

DWORD	WINAPI okGetTickCount(void)
{	LoadOkApiDll(0);
	return lpfnGetTickCount();	}

void	WINAPI okSleep(DWORD dwMill)
{	LoadOkApiDll(0);
	lpfnSleep(dwMill);	}

DWORD	WINAPI okGetTickCountMicro(LPDWORD lpHiDword)
{	LoadOkApiDll(0);
	return lpfnGetTickCountMicro(lpHiDword);	}

void	WINAPI okSleepMicro(DWORD dwMicro)
{	LoadOkApiDll(0);
	lpfnSleepMicro(dwMicro);	}


//--------audio-----------

HANDLE	WINAPI okOpenAudio(HANDLE hBoard, MLONG lParam)
{	
	LoadOkApiDll(1);
	return lpfnOpenAudio(hBoard, lParam);	
}

BOOL	WINAPI okCloseAudio(HANDLE hAudio)
{	
	BOOL	ret;
	
	ret=lpfnCloseAudio(hAudio);	
	if(ret)
		FreeOkApiDll();

	return ret;
}


MLONG	WINAPI okCaptureAudio(HANDLE hAudio, TARGET target, FARPROC lpfnUserProc, MLONG lParam)
{	return lpfnCaptureAudio(hAudio, target, lpfnUserProc, lParam);	}

BOOL	WINAPI okStopCaptureAudio(HANDLE hAudio)
{	return lpfnStopCaptureAudio(hAudio);	}

INT32	WINAPI okSetAudioParam(HANDLE hAudio, INT32 wParam, INT32 lParam)
{	return lpfnSetAudioParam(hAudio, wParam, lParam);	}

INT32	WINAPI okReadAudioData(HANDLE hAudio, LPBYTE lpAudioBuf, INT32 lReadSize)
{	return lpfnReadAudioData(hAudio, lpAudioBuf, lReadSize);	}




//--------stream-----------

HANDLE	WINAPI okOpenStream(HANDLE hBoard, DWORD lParam)
{	
	LoadOkApiDll(1);
	return lpfnOpenStream(hBoard, lParam);	
}

BOOL	WINAPI okCloseStream(HANDLE hStream)
{	
	BOOL	ret;
	
	ret=lpfnCloseStream(hStream);	
	if(ret)
		FreeOkApiDll();

	return ret;
}


HANDLE	WINAPI okCaptureStream(HANDLE hStream, TARGET target, FARPROC lpfnUserProc, MLONG lMiliSeconds)
{	return lpfnCaptureStream(hStream, target, lpfnUserProc, lMiliSeconds);	}

BOOL	WINAPI okStopCaptureStream(HANDLE hStream, DWORD dwPause)
{	return lpfnStopCaptureStream(hStream, dwPause);	}

MLONG	WINAPI okSetStreamParam(HANDLE hStream, INT32 wParam, MLONG lParam)
{	return lpfnSetStreamParam(hStream, wParam, lParam);	}

INT32	WINAPI okReadStreamData(HANDLE hStream, LPBYTE lpStreamBuf, INT32 lReadSize)
{	return lpfnReadStreamData(hStream, lpStreamBuf, lReadSize);	}

MLONG	WINAPI	okGetCurrStream(HANDLE hStream, LPVOID lParam)
{	return lpfnGetCurrStream(hStream,lParam); }

INT32	WINAPI okSetSerial(HANDLE hBoard, INT32 wParam, INT32 lParam)
{	return lpfnSetSerial( hBoard, wParam, lParam);	}

INT32	WINAPI okReadSerial(HANDLE hBoard, LPVOID lpBuffer, INT32 lSize, MLONG lTimeOut)
{	return lpfnReadSerial( hBoard, lpBuffer, lSize, lTimeOut);	}

BOOL	WINAPI okWriteSerial(HANDLE hBoard, LPVOID lpBuffer, INT32 lSize, MLONG lTimeOut)
{	return lpfnWriteSerial( hBoard, lpBuffer, lSize, lTimeOut);	}




//-----------end-----------------------------------------------------------------------