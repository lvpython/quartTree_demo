#ifndef	_PORT_DEFINE_H_

#define	_PORT_DEFINE_H_



//移植区	开始：

//这一部分内容需要在其它平台下重新进行类型映射

//某一些类型如GUID需要结构定义

#include "stdlib.h"



#ifdef	_WIN32_WCE

#	ifndef	WIN32

#		define	WIN32

#	endif

#endif



#include "string.h"



#define	SMG_ASSERT_VALID(x)

#define	SMG_INLINE			inline



#if	defined(WIN32) && !defined(SMG_NO_VC)

#	ifdef	_WIN32_WCE

#		ifndef	_WIN32_WCE_EMULATION

#			define	SMG_APICALLTYPE		__cdecl

#		else

#			define	SMG_APICALLTYPE		__stdcall

#		endif

#	else

#		define	SMG_APICALLTYPE		__stdcall

#	endif



#	define	SMG_CCALLTYPE		__cdecl

#	define	SMG_StaticCast(x,p)	(static_cast<x>(p))

#	define	SMG_NOVT			__declspec(novtable)

#else

#	define	SMG_APICALLTYPE		

#	define	SMG_CCALLTYPE		

#	define	SMG_StaticCast(x,p)	(static_cast<x>(p))

#	define	SMG_NOVT			

#endif



#ifdef __cplusplus

#	define	_SMG_CPP

#endif



#ifndef EXTERN_C

#	ifdef _SMG_CPP

#		define EXTERN_C			extern "C"

#	else

#		define EXTERN_C

#	endif

#endif



#ifndef	CONST

#	ifdef	_SMG_CPP

#		define	CONST				const

#	else

#		define	CONST

#	endif

#endif



#ifdef	DEBUG

#	define	SMG_DEBUG

#endif



#ifdef	_DEBUG

#	define	SMG_DEBUG

#	define	DEBUG

#endif




typedef	unsigned long		SMG_ULONG;

typedef	unsigned long		SMG_DWORD;

typedef	unsigned char		SMG_BYTE;

typedef	long				SMG_LONG;

typedef	int					SMG_BOOL;

typedef	int					SMG_INT;

typedef	unsigned int		SMG_UINT;

typedef	float				SMG_FLOAT;

typedef	double				SMG_DOUBLE;

typedef	short				SMG_SHORT;

typedef	unsigned short		SMG_USHORT;

typedef	long				SMG_RESULT;

typedef	unsigned short		SMG_WORD;

typedef	unsigned short 		SMG_WCHAR;

typedef	SMG_WCHAR *			SMG_BSTR;

typedef SMG_WCHAR*			SMG_LPWSTR;

typedef CONST SMG_WCHAR*	SMG_LPCWSTR;

typedef	char				SMG_CHAR;

typedef SMG_CHAR *			SMG_LPSTR;

typedef	CONST SMG_CHAR *	SMG_LPCSTR;

typedef	void *				SMG_HANDLE;





typedef struct  _SMG_GUID

		{

		SMG_DWORD Data1;

		SMG_WORD Data2;

		SMG_WORD Data3;

		SMG_BYTE Data4[ 8 ];

		}					SMG_GUID;

typedef	SMG_GUID			SMG_XID;

typedef	SMG_GUID			SMG_CLSID;

typedef	void				SMG_VOID;

typedef	short				SMG_VARIANT_BOOL;

typedef union _SMG_LARGE_INTEGER {

    struct {

        SMG_DWORD LowPart;

        SMG_LONG HighPart;

    };

    struct {

        SMG_DWORD LowPart;

        SMG_LONG HighPart;

    } u;

} SMG_LARGE_INTEGER;

typedef union _SMG_ULARGE_INTEGER {

    struct {

        SMG_DWORD LowPart;

        SMG_DWORD HighPart;

    };

    struct {

        SMG_DWORD LowPart;

        SMG_DWORD HighPart;

    } u;

} SMG_ULARGE_INTEGER;

typedef	SMG_DOUBLE			SMG_DATE;

typedef SMG_USHORT			SMG_VARTYPE;

typedef	SMG_LARGE_INTEGER	SMG_CY;







typedef struct  _SMG_SAFEARRAYBOUND

    {

    SMG_ULONG	cElements;

    SMG_LONG	lLbound;

    }	SMG_SAFEARRAYBOUND;



typedef struct  _SMG_SAFEARRAY

    {

    SMG_USHORT	cDims;

    SMG_USHORT	fFeatures;

    SMG_ULONG	cbElements;

    SMG_ULONG	cLocks;

    SMG_VOID*	pvData;

    SMG_SAFEARRAYBOUND	

				rgsabound[ 1 ];

    }	SMG_SAFEARRAY;








typedef struct _SMG_SIZE

{

    SMG_LONG        cx;

    SMG_LONG        cy;

} SMG_SIZE;



typedef CONST SMG_SIZE *        LPCSMG_SIZE;

typedef SMG_SIZE *              LPSMG_SIZE;





typedef struct _SMG_SysTime{

	SMG_WORD	wYear;		//2003,...

	SMG_WORD	wMonth;		//1,2,3,..12

	SMG_WORD	wDay;		//1,2,3,...

	SMG_WORD	wDayOfWeek;	//0(Sunday),1,2,...6

	SMG_WORD	wHour;

	SMG_WORD	wMinute;

	SMG_WORD	wSecond;

	SMG_WORD	wPad;

}SMG_SysTime;



//移植区	结束



#ifdef	_SMG_CPP

#	define	SMG_REFXID			CONST SMG_XID&

#	define	SMG_REFCLSID		CONST SMG_CLSID&

#else

#	define	SMG_REFXID			CONST SMG_XID*

#	define	SMG_REFCLSID		CONST SMG_CLSID*

#endif



//COM

#undef	SUCCEEDED

#undef	FAILED

#define SUCCEEDED(Status)	((HRESULT)(Status) >= 0)

#define FAILED(Status)		((HRESULT)(Status)<0)

#undef	STDMETHODCALLTYPE

#undef	STDMETHOD

#undef	STDMETHOD_

#undef	STDMETHODIMP

#define	STDMETHODCALLTYPE		SMG_APICALLTYPE

#define STDMETHODIMP            SMG_RESULT STDMETHODCALLTYPE



#undef	THIS_

#undef	THIS

#undef	DECLARE_INTERFACE

#undef	DECLARE_INTERFACE_



#ifdef	_SMG_CPP

#	define interface				struct

#	define PREVDEFINE_INTERFACE(iface)	interface SMG_NOVT iface;

#	define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method

#	define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method

#	define PURE                    = 0

#	define THIS_

#	define THIS                    

#	define DECLARE_INTERFACE(iface)    interface SMG_NOVT iface

#	define DECLARE_INTERFACE_(iface, baseiface)    interface SMG_NOVT iface : public baseiface

#else

#	define interface               struct

#	define PREVDEFINE_INTERFACE(iface)	interface iface;	\

										typedef interface iface iface;

#	define STDMETHOD(method)       HRESULT (STDMETHODCALLTYPE * method)

#	define STDMETHOD_(type,method) type (STDMETHODCALLTYPE * method)

#	define PURE

#	define THIS_                   INTERFACE * This,

#	define THIS                    INTERFACE * This

#	ifdef CONST_VTABLE

#		define DECLARE_INTERFACE(iface)    interface iface { \

											const struct iface##Vtbl * lpVtbl; \

										} ; \

										typedef const struct iface##Vtbl iface##Vtbl; \

										const struct iface##Vtbl

#	else

#		define DECLARE_INTERFACE(iface)    interface iface { \

											struct iface##Vtbl * lpVtbl; \

										} ; \

										typedef struct iface##Vtbl iface##Vtbl; \

										struct iface##Vtbl

#	endif

#	define DECLARE_INTERFACE_(iface, baseiface)    DECLARE_INTERFACE(iface)

#endif



PREVDEFINE_INTERFACE(XUnknown)



typedef struct  _SMG_VARIANT

    {

        SMG_VARTYPE vt;

		SMG_USHORT wReserved1;

		SMG_USHORT wReserved2;

		SMG_USHORT wReserved3;

		union 

            {

            SMG_LONG			lVal;

            SMG_BYTE			bVal;

            SMG_SHORT			iVal;

            SMG_FLOAT			fltVal;

            SMG_DOUBLE			dblVal;

            SMG_VARIANT_BOOL	boolVal;

            SMG_DATE			date;

            SMG_BSTR			bstrVal;

            XUnknown  *			punkVal;

            SMG_CHAR			cVal;

            SMG_USHORT			uiVal;

            SMG_ULONG			ulVal;

            SMG_INT				intVal;

            SMG_UINT			uintVal;

			SMG_CY				cyVal;

			SMG_SAFEARRAY *		parray;

            };

    }SMG_VARIANT;



enum SMG_VARENUM

    {	

	SMG_VT_EMPTY	= 0,

	SMG_VT_I2		= 2,

	SMG_VT_I4		= 3,

	SMG_VT_R4		= 4,

	SMG_VT_R8		= 5,

	SMG_VT_DATE		= 7,

	SMG_VT_BSTR		= 8,

	SMG_VT_BOOL		= 11,

	SMG_VT_UNKNOWN	= 13,

	SMG_VT_I1		= 16,

	SMG_VT_UI1		= 17,

	SMG_VT_UI2		= 18,

	SMG_VT_UI4		= 19,

	SMG_VT_I8		= 20,

	SMG_VT_UI8		= 21,

	SMG_VT_INT		= 22,

	SMG_VT_UINT		= 23,

	SMG_VT_CY		= 6,

	SMG_VT_ARRAY	= 0x2000,

    };



#define	ULONG				SMG_ULONG

#define	DWORD				SMG_DWORD

#define COLORREF            SMG_DWORD

#define	BYTE				SMG_BYTE

#define	LONG				SMG_LONG

#define	BOOL				SMG_BOOL

#define	INT					SMG_INT

#define	UINT				SMG_UINT

#define	FLOAT				SMG_FLOAT

#define	DOUBLE				SMG_DOUBLE

#define	SHORT				SMG_SHORT

#define	USHORT				SMG_USHORT

#define	HRESULT				SMG_RESULT

#define	WORD				SMG_WORD

#undef	VOID

#define	VOID				SMG_VOID

#define	VARIANT_BOOL		SMG_VARIANT_BOOL

#define	BSTR				SMG_BSTR

#define LPWSTR				SMG_LPWSTR

#define LPCWSTR				SMG_LPCWSTR

#define	CHAR				SMG_CHAR

#define	WCHAR				SMG_WCHAR

#define LPSTR				SMG_LPSTR

#define	LPCSTR				SMG_LPCSTR

/*

#define	DATE				SMG_DATE

#define	CY					SMG_CY

*/

#define	VT_EMPTY			SMG_VT_EMPTY	

#define	VT_I2				SMG_VT_I2		

#define	VT_I4				SMG_VT_I4		

#define	VT_R4				SMG_VT_R4		

#define	VT_R8				SMG_VT_R8		

#define	VT_DATE				SMG_VT_DATE		

#define	VT_BSTR				SMG_VT_BSTR		

#define	VT_BOOL				SMG_VT_BOOL		

#define	VT_UNKNOWN			SMG_VT_UNKNOWN	

#define	VT_I1				SMG_VT_I1		

#define	VT_UI1				SMG_VT_UI1		

#define	VT_UI2				SMG_VT_UI2		

#define	VT_UI4				SMG_VT_UI4		

#define	VT_I8				SMG_VT_I8		

#define	VT_UI8				SMG_VT_UI8		

#define	VT_INT				SMG_VT_INT		

#define	VT_UINT				SMG_VT_UINT		

#define	VT_CY				SMG_VT_CY		

#define	VT_ARRAY			SMG_VT_ARRAY	



#ifndef	VARIANT_TRUE

#	define VARIANT_TRUE ((VARIANT_BOOL)0xffff)

#endif



#ifndef	VARIANT_FALSE

#	define VARIANT_FALSE ((VARIANT_BOOL)0)

#endif



typedef unsigned int SMG_size_t;

#define	size_t	SMG_size_t



#ifndef	offsetof

#	define offsetof(s,m) (((size_t)&(((s*)8)->m))-8)

#endif



typedef DWORD (SMG_APICALLTYPE *SMG_ThreadProc)(VOID * param);

typedef enum _SMG_ThreadPriority{

	SMG_TP_NORMAL	= 0,

	SMG_TP_LOW		= 1,

	SMG_TP_HIGH		= 2,

	SMG_TP_IDLE		= 3,

	SMG_TP_HIGHEST	= 4,

}SMG_ThreadPriority;



#define	SMG_INFINITE		0xFFFFFFFF



#ifndef	EXCLUDE_COMMON_FUNCTIONS

#	ifndef	USE_LIB

#		ifdef	WIN32

#			ifdef	_WIN32_WCE

#				pragma	comment(lib,"SmCommon.lib")

#			else

#				pragma	comment(lib,"SmCommonW.lib")

#			endif

#		endif

#	endif



//COM Functions

EXTERN_C SMG_RESULT 

SMG_APICALLTYPE SMG_CoCreateInstance(SMG_REFCLSID clsid,XUnknown * pUnkOuter,SMG_DWORD dwClsCtx,SMG_REFXID riid,VOID** ppObj);

EXTERN_C SMG_RESULT 

SMG_APICALLTYPE SMG_CoGetClassObject(SMG_REFCLSID clsid,SMG_WORD dwClsContext,VOID * pServerInfo,SMG_REFXID riid,VOID** ppv );

EXTERN_C SMG_RESULT

SMG_APICALLTYPE SMG_RegisterDLL(SMG_LPCWSTR dllName);

EXTERN_C SMG_RESULT SMG_APICALLTYPE SMG_Uninitialize();

//BSTR Functions

EXTERN_C BSTR SMG_APICALLTYPE SMG_AllocString(SMG_LPCWSTR pSrc);

EXTERN_C VOID SMG_APICALLTYPE SMG_FreeString(SMG_BSTR bstr);

EXTERN_C UINT SMG_APICALLTYPE SMG_StringLen(SMG_BSTR bstr);

EXTERN_C BSTR SMG_APICALLTYPE SMG_AllocStringLen(UINT cch);

EXTERN_C UINT SMG_APICALLTYPE SMG_StringByteLen(SMG_BSTR bstr);

EXTERN_C BSTR SMG_APICALLTYPE SMG_AllocStringByteLen(UINT cch);

EXTERN_C BSTR SMG_APICALLTYPE SMG_AllocStringForAnsi(LPCSTR str);

//Mem Functions

EXTERN_C VOID * SMG_APICALLTYPE SMG_AllocMem(ULONG memSize);

EXTERN_C VOID SMG_APICALLTYPE SMG_FreeMem(VOID * mem);

//Array Functions

EXTERN_C SMG_SAFEARRAY * 

SMG_APICALLTYPE SMG_CreateArray(SMG_USHORT vt,SMG_UINT cDims,SMG_SAFEARRAYBOUND *  rgsabound);

EXTERN_C SMG_RESULT 

SMG_APICALLTYPE SMG_DestroyArray(SMG_SAFEARRAY * parray);

EXTERN_C SMG_RESULT

SMG_APICALLTYPE SMG_LockArray(SMG_SAFEARRAY * parray,VOID ** data);

EXTERN_C SMG_RESULT

SMG_APICALLTYPE SMG_UnlockArray(SMG_SAFEARRAY * parray);

EXTERN_C SMG_RESULT

SMG_APICALLTYPE SMG_CopyArray(SMG_SAFEARRAY * parray,SMG_SAFEARRAY ** arrayOut);

EXTERN_C SMG_HANDLE

SMG_APICALLTYPE SMG_CreateHeap(SMG_DWORD dwInitSize);

EXTERN_C SMG_BOOL

SMG_APICALLTYPE SMG_DestroyHeap(SMG_HANDLE hHeap);

EXTERN_C SMG_VOID *

SMG_APICALLTYPE SMG_HeapAlloc(SMG_HANDLE hHeap,SMG_DWORD dwSize);

EXTERN_C SMG_BOOL

SMG_APICALLTYPE SMG_HeapFree(SMG_HANDLE hHeap,SMG_VOID *lpMem);

EXTERN_C SMG_DWORD

SMG_APICALLTYPE SMG_HeapSize(SMG_HANDLE hHeap,SMG_VOID *lpMem);

EXTERN_C SMG_VOID 

SMG_APICALLTYPE SMG_ConvertWToA(LPCWSTR lpw,LPSTR lpa,LONG bufCnt);

EXTERN_C LONG

SMG_APICALLTYPE SMG_IncrLONG(LONG * value);

EXTERN_C LONG

SMG_APICALLTYPE SMG_DecrLONG(LONG * value);

EXTERN_C LONG

SMG_APICALLTYPE SMG_GetCurrentThreadID();

EXTERN_C VOID

SMG_APICALLTYPE SMG_OutputDbgMsg(LPCSTR msg);

EXTERN_C DWORD

SMG_APICALLTYPE SMG_GetTickCount();

EXTERN_C VOID *

SMG_APICALLTYPE SMG_InitCS();

EXTERN_C VOID

SMG_APICALLTYPE SMG_FiniCS(VOID * pSection);

EXTERN_C VOID

SMG_APICALLTYPE SMG_EnterCS(VOID * pSection);

EXTERN_C VOID

SMG_APICALLTYPE SMG_LeaveCS(VOID * pSection);

EXTERN_C VOID *

SMG_APICALLTYPE SMG_CreateThread(SMG_ThreadProc startAddr,VOID * param,SMG_ThreadPriority priority);

EXTERN_C VOID

SMG_APICALLTYPE SMG_WaitThreadExit(VOID *pThread);

EXTERN_C VOID * 

SMG_APICALLTYPE SMG_CreateSemaphore(LONG lInitCnt,LONG lMaxCnt);

EXTERN_C VOID 

SMG_APICALLTYPE SMG_CloseSemaphore(VOID * pSemaphore);

EXTERN_C BOOL 

SMG_APICALLTYPE SMG_WaitSemaphore(VOID * pSemaphore,LONG timeOut);

EXTERN_C VOID 

SMG_APICALLTYPE SMG_ReleaseSemaphore(VOID * pSemaphore);

EXTERN_C VOID 

SMG_APICALLTYPE SMG_Sleep(LONG dwMilliseconds);

EXTERN_C VOID * 

SMG_APICALLTYPE SMG_CreateEvent(BOOL bInitialSignal);

EXTERN_C VOID 

SMG_APICALLTYPE SMG_CloseEvent(VOID * pEvent);

EXTERN_C BOOL 

SMG_APICALLTYPE SMG_WaitEvent(VOID * pEvent,LONG timeOut);

EXTERN_C VOID 

SMG_APICALLTYPE SMG_SetEvent(VOID * pEvent);

EXTERN_C VOID *

SMG_APICALLTYPE SMG_GetCurrentThread();

EXTERN_C VOID

SMG_APICALLTYPE SMG_TerminateThread(VOID *pThread);

EXTERN_C BOOL

SMG_APICALLTYPE SMG_GetLocalTime(SMG_SysTime * time);

EXTERN_C BOOL

SMG_APICALLTYPE SMG_EnumModule(LONG lIndex,LPCSTR *moduleName,LPCSTR *versionInfo);

EXTERN_C BOOL

SMG_APICALLTYPE SMG_RegisterModule(LPCSTR moduleName,LPCSTR versionInfo);

EXTERN_C DWORD

SMG_APICALLTYPE SMG_TlsAlloc();

EXTERN_C BOOL

SMG_APICALLTYPE SMG_TlsFree(DWORD dwIndex);

EXTERN_C VOID *

SMG_APICALLTYPE SMG_TlsGetValue(DWORD dwInex);

EXTERN_C BOOL

SMG_APICALLTYPE SMG_TlsSetValue(DWORD dwIndex,VOID * value);

EXTERN_C VOID

SMG_APICALLTYPE SMG_TransString(CHAR *string,LONG num);

EXTERN_C VOID

SMG_APICALLTYPE SMG_UntransString(CHAR *string,LONG num);

EXTERN_C VOID 

SMG_APICALLTYPE SMG_GetSubDirs(LPCWSTR connStr,BSTR * outStr);



#endif//	EXCLUDE_COMMON_FUNCTIONS



EXTERN_C SMG_GUID SMG_APICALLTYPE SMG_ConvertStringToGUID(SMG_LPCSTR str);








#if	!defined(_LIB) && !defined(USE_LIB)

#	if	defined(INIT_SMG_GUID)

#		undef	DEFINE_SMG_GUID_DEC				

#		undef	DEFINE_SMG_GUID_BYSTRING_DEC	

#		define DEFINE_SMG_GUID_DEC				DEFINE_SMG_GUID_DEF

#		define	DEFINE_SMG_GUID_BYSTRING_DEC	DEFINE_SMG_GUID_BYSTRING_DEF

#	endif

#endif



#define DEFINE_SMG_GUID					DEFINE_SMG_GUID_DEC

#define	DEFINE_SMG_GUID_BYSTRING		DEFINE_SMG_GUID_BYSTRING_DEC



#ifndef NULL

#	ifdef _SMG_CPP

#		define NULL    0

#	else

#		define NULL    ((void *)0)

#	endif

#endif



#ifndef FALSE

#	define FALSE               0

#endif



#ifndef TRUE

#	define TRUE                1

#endif



#ifndef	S_OK

#	define E_POINTER					    0x80004003L

#	define S_OK                             0x00000000L

#	define S_FALSE                          0x00000001L

#	define E_OUTOFMEMORY                    0x8007000EL

#	define E_FAIL                           0x80004005L

#	define E_NOINTERFACE                    0x80004002L

#	define CLASS_E_NOAGGREGATION            0x80040110L

#	define E_NOTIMPL                        0x80004001L

#endif






#ifndef max

#	define max(a,b)            (((a) > (b)) ? (a) : (b))

#endif



#ifndef min

#	define min(a,b)            (((a) < (b)) ? (a) : (b))

#endif



#ifndef	MAKEWORD

#	define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))

#endif



#ifndef	MAKELONG

#	define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))

#endif



#ifndef	LOWORD

#	define LOWORD(l)           ((WORD)(l))

#	define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))

#endif



#ifndef	LOBYTE

#	define LOBYTE(w)           ((BYTE)(w))

#	define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#endif



#ifndef _WINDOWS_

#	ifndef	__WINDOWS__

#		define	POINT				SMG_POINT

#		define	SIZE				SMG_SIZE

#		define	RECT				SMG_RECT

#		define LPCPOINT            LPCSMG_POINT

#		define LPPOINT             LPSMG_POINT

#		define LPCSIZE             LPCSMG_SIZE

#		define LPSIZE              LPSMG_SIZE

#		define LPCRECT             LPCSMG_RECT

#		define LPRECT              LPSMG_RECT

#		ifndef	RGB	

#		define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#		define GetRValue(rgb)      ((BYTE)(rgb))

#		define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))

#		define GetBValue(rgb)      ((BYTE)((rgb)>>16))

#		endif

#	endif

#endif



#define SMG_T(x)			L##x



#ifndef TEXT

#	define	TEXT				SMG_T

#	define	_T					TEXT

#endif



#ifdef	_SMG_CPP

#	include "PortMem.h"

#		if	(defined(INIT_SMG_GUID) && !defined(USE_LIB) && !defined(_LIB)) || defined(INIT_CMN_ALLOC)

			CSMGMemAllocator			g_CmnAlloc(0xFFFFFFFF);

#		else

			extern CSMGMemAllocator		g_CmnAlloc;

#		endif

#	endif

#endif

