#pragma once
#include "d3d11.h"

#define HookSite_Menu 0x14236E070//0x14236F86D
#define HookSite_GUI 0x1423A77EE
#define MaxPrint 299

struct SDebugPrint;

class CDS3Debug {
public:
	virtual VOID Start();
	static LONG WINAPI UHFilter(struct _EXCEPTION_POINTERS* apExceptionInfo);
	virtual VOID Run();
	virtual BOOL Hook(DWORD64 qAddress, DWORD64* pReturn, DWORD64 qDetour, DWORD dByteGap);
	virtual BOOL Unhook(DWORD64 qAddress);
	virtual BOOL TweakMem(DWORD64 qAddress, DWORD dSize, VOID* pBytes);
	virtual VOID MinimiseShelvesOptions(FLOAT* pFontSize, DWORD dY);
	virtual VOID ClearStrings();
};

struct SDS3Debug {
	DWORD dIsActive = 1;
	IFW1FontWrapper* pFontWrapper;
};

struct SDebugPrint {
	FLOAT fX;
	FLOAT fY;
	BYTE pUnk[0x10];
	wchar_t wcText[300];
};

struct SDrawStruct {
	DWORD dIsActive;
	FLOAT fFontSize;
	DWORD dColour;
	SDebugPrint sDebugPrint;
};

extern "C" VOID tDS3DebugMenuPrint();
extern "C" DWORD64 bDS3DebugMenuPrint;
extern "C" VOID fDS3DebugMenuPrint(SDebugPrint* D);

extern "C" VOID tDS3DebugGUIPrint();
extern "C" DWORD64 bDS3DebugGUIPrint;
extern "C" VOID fDS3DebugGUIPrint(FLOAT fX, FLOAT fY, wchar_t* pwcText);

extern "C" VOID tDS3DebugMenuNEXT();
extern "C" DWORD64 bDS3DebugMenuNEXT;
extern "C" VOID fDS3DebugMenuNEXT(wchar_t* string, float, float);

extern "C" VOID tIPV4Hook();
extern "C" DWORD64 bIPV4Hook;