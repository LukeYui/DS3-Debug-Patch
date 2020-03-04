#include "d3d11.h"

#define HookSite_Menu 0x14236F86D
#define HookSite_GUI 0x1423A77EE

struct SDebugPrint;

class CDS3Debug {
public:
	virtual VOID Start();
	static LONG WINAPI UHFilter(struct _EXCEPTION_POINTERS* apExceptionInfo);
	virtual VOID Run();
	virtual BOOL Hook(DWORD64 qAddress, DWORD64* pReturn, DWORD64 qDetour, DWORD dByteGap);
	virtual BOOL Unhook(DWORD64 qAddress);
	virtual BOOL TweakMem(DWORD64 qAddress, DWORD dSize, VOID* pBytes);
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

extern "C" VOID tDS3DebugMenuPrint();
extern "C" DWORD64 bDS3DebugMenuPrint;
extern "C" VOID fDS3DebugMenuPrint(SDebugPrint* D);

extern "C" VOID tDS3DebugGUIPrint();
extern "C" DWORD64 bDS3DebugGUIPrint;
extern "C" VOID fDS3DebugGUIPrint(FLOAT fX, FLOAT fY, wchar_t* pwcText);