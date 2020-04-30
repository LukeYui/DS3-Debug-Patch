#include "DS3Debug.h"

DWORD64 bDS3DebugMenuPrint = 0;
DWORD64 bDS3DebugGUIPrint = 0;
DWORD64 bDS3DebugMenuNEXT = 0;
DWORD64 bIPV4Hook = 0;

extern CDS3Debug* DS3Debug;
extern SDS3Debug* DS3DebugStruct;

extern ID3D11RenderTargetView* RenderTargetView;

BYTE pNopBytes[5] = { 0x90, 0x90, 0x90, 0x90, 0x90, };
BYTE pRetBytes[1] = { 0xC3 };
BYTE pXorRaxBytes[5] = { 0x48, 0x31, 0xC0, 0x90, 0x90 };
BYTE pSetAlBytes[5] = { 0xB0, 0x01, 0x90, 0x90, 0x90 };
DWORD dHMAVCheck1 = 0x00CE9840;
DWORD dHMAVCheck2 = 0x036973AA;

BYTE pGestureBytes[1] = { 0x75 };

BYTE pFreeCamBytes1[5] = { 0xE8, 0xBD, 0xB0, 0xD0, 01 };
BYTE pFreeCamBytes2[35] = { 0x44, 0x88, 0xA6, 0x98, 0x00, 0x00, 0x00, 0x8B, 0x83, 0xE0, 0x00, 0x00, 0x00, 0xFF, 0xC8, 0x83, 0xF8, 0x01, 0x0F, 0x87, 0x35, 0x01, 0x00, 0x00, 0x44, 0x88, 0xBE, 0x98, 0x00, 0x00, 0x00 };
BYTE pJmpSwapBytes[1] = { 0xEB };
BYTE pDecalBytes[1] = { 0x4F };

extern SDrawStruct DrawStruct[300];

extern CRITICAL_SECTION CriticalSection;

VOID CDS3Debug::Start() {
	Run();
	return;
};

VOID CDS3Debug::Run() {

	DWORD dBypassCheck1 = 0;
	DWORD dBypassCheck2 = 0;

	Hook(HookSite_Menu, &bDS3DebugMenuNEXT, (DWORD64)&tDS3DebugMenuNEXT, 11);//7);
	//Hook(HookSite_GUI, &bDS3DebugGUIPrint, (DWORD64)&tDS3DebugGUIPrint, 5); //Flickers too badly.
	
	//Disable network access
	DS3Debug->Hook(0x1418C0E60, &bIPV4Hook, (DWORD64)&tIPV4Hook, 0);

	//TweakMem(0x14236E076, 5, pNopBytes); //-- Disable Font
	TweakMem(0x142352600, 1, pRetBytes); //-- Disable Font
	TweakMem(0x1423B7670, 1, pRetBytes); //-- Disable Font
	TweakMem(0x141915370, 1, pRetBytes); //-- Disable Font
	TweakMem(0x1403A44A0, 1, pRetBytes); //-- Disable Font
	TweakMem(0x140CDB500, 1, pRetBytes); //-- Disable Font (WIND WORLD) (全クロス風デバッグ描画)
	

	TweakMem(0x140CDC6E6, 1, pJmpSwapBytes); //-- Disable WindWorldCrash
	TweakMem(0x14080A2F0, 2, pSetAlBytes); //-- Enable INS
	TweakMem(0x14080A2E0, 2, pSetAlBytes); //-- Enable Event
	TweakMem(0x142356840, 1, pRetBytes); //Enable active entity control (skip cameraparam crash)
	TweakMem(0x140E82DEE, 5, pXorRaxBytes); //-- (HeatMap Menu) Crash on load so it's disabled

	//Features -- Freecam (A + L3)
	TweakMem(0x14062C3AE, 5, pFreeCamBytes1);
	TweakMem(0x14062C401, 31, pFreeCamBytes2);

	//Disable Gesture Menu
	TweakMem(0x140B2D583, 1, pGestureBytes);

	dBypassCheck1 = 0x0030EEA3;
	TweakMem(0x1408B1CF1, 4, &dBypassCheck1); //Bypass the check that bricks saves
	dBypassCheck2 = 0xFFE48E31;
	TweakMem(0x140EE7C01, 4, &dBypassCheck2); //This check restores this one ^

	while (DS3DebugStruct->dIsActive) {
		SetUnhandledExceptionFilter(UHFilter);
		//EnterCriticalSection(&CriticalSection);
		//ClearStrings();
		//LeaveCriticalSection(&CriticalSection);
		Sleep(100);
	};

	Unhook(HookSite_Menu);
	//Unhook(HookSite_GUI);

	return;
};

BOOL CDS3Debug::Hook(DWORD64 qAddress, DWORD64* pReturn, DWORD64 qDetour, DWORD dByteGap) {

	*pReturn = (qAddress + dByteGap);
	if (MH_CreateHook((LPVOID)qAddress, (LPVOID)qDetour, 0) != MH_OK) return false;
	if (MH_EnableHook((LPVOID)qAddress) != MH_OK) return false;

	return true;
};

BOOL CDS3Debug::Unhook(DWORD64 qAddress) {
	if (MH_DisableHook((LPVOID)qAddress)) return false;
	return true;
};

BOOL CDS3Debug::TweakMem(DWORD64 qAddress, DWORD dSize, VOID* pBytes) {

	DWORD dOldProtect = 0;

	if (!VirtualProtect((LPVOID)qAddress, dSize, PAGE_EXECUTE_READWRITE, &dOldProtect)) return false;
	memcpy((VOID*)qAddress, pBytes, dSize);
	VirtualProtect((LPVOID)qAddress, dSize, dOldProtect, &dOldProtect);

	return true;
};

VOID fDS3DebugMenuPrint(SDebugPrint* D) {

	FLOAT fFontSize = 20.00f;

	SDebugPrint S = SDebugPrint();

	if (!D) return;
	if (!D->fX || !D->fY || !D->wcText[0]) return;

	DS3Debug->MinimiseShelvesOptions(&fFontSize, (DWORD)D->fY);

	//EnterCriticalSection(&CriticalSection);

	for (int i = 0; i < MaxPrint; i++) {
		if (!DrawStruct[i].dIsActive) {
			DrawStruct[i].dIsActive = 1;
			DrawStruct[i].fFontSize = fFontSize;
			DrawStruct[i].dColour = C_WHITE;
			DrawStruct[i].sDebugPrint.fX = D->fX;
			DrawStruct[i].sDebugPrint.fY = D->fY;
			memcpy(DrawStruct[i].sDebugPrint.wcText, D->wcText, 250);
			break;
		};
	};

	//LeaveCriticalSection(&CriticalSection);

	return;
};

VOID fDS3DebugMenuNEXT(wchar_t* string, float x, float y)
{
	FLOAT fFontSize = 20.00f;

	if (!string)
		return;

	DS3Debug->MinimiseShelvesOptions(&fFontSize, (DWORD)y);

	//EnterCriticalSection(&CriticalSection);

	for (int i = 0; i < MaxPrint; i++)
	{
		if (!DrawStruct[i].dIsActive) {
			DrawStruct[i].dIsActive = 1;
			DrawStruct[i].fFontSize = 20.f;
			DrawStruct[i].dColour = C_WHITE;
			DrawStruct[i].sDebugPrint.fX = x;
			DrawStruct[i].sDebugPrint.fY = y;
			memcpy(DrawStruct[i].sDebugPrint.wcText, string, 250);
			break;
		};
	}

	//LeaveCriticalSection(&CriticalSection);
}

VOID fDS3DebugGUIPrint(FLOAT fX, FLOAT fY, wchar_t* pwcText) {

	FLOAT fFontSize = 10.00f;

	SDebugPrint D = SDebugPrint();

	if (!fX || !fY || !pwcText) return;
	return;
	for (int i = 0; i < 0; i++) {
		if (!DrawStruct[i].dIsActive) {
			DrawStruct[i].dIsActive = 1;
			DrawStruct[i].fFontSize = fFontSize;
			DrawStruct[i].dColour = C_BLACK;
			DrawStruct[i].sDebugPrint.fX = fX;
			DrawStruct[i].sDebugPrint.fY = fY;
			memcpy(DrawStruct[i].sDebugPrint.wcText, pwcText, 250);
			break;
		};
	};

	return;
};

VOID CDS3Debug::MinimiseShelvesOptions(FLOAT* pFontSize, DWORD dY) {

	//I've had to manually do this bit to look slick

	switch (dY) {
		case(80): {
			*pFontSize = 18.00f;
			return;
		};
		case(67): {
			*pFontSize = 16.00f;
			return;
		};
		case(56): {
			*pFontSize = 14.00f;
			return;
		};
		case(48): {
			*pFontSize = 12.00f;
			return;
		};
		case(42): {
			*pFontSize = 10.00f;
			return;
		};
		case(36): {
			*pFontSize = 8.00f;
			return;
		};
		case(32): {
			*pFontSize = 7.00f;
			return;
		};
		default: return;
	};

	return;
};

VOID CDS3Debug::ClearStrings() {

	for (int i = 0; i < MaxPrint; i++) {
		DrawStruct[i].dIsActive = 0;
	};

	return;
};

LONG WINAPI CDS3Debug::UHFilter(struct _EXCEPTION_POINTERS* apExceptionInfo) {

	DWORD ExceptionCode = apExceptionInfo->ExceptionRecord->ExceptionCode;
	DWORD ExceptionFlags = apExceptionInfo->ExceptionRecord->ExceptionFlags;
	DWORD64 FaultOffset = (DWORD64)apExceptionInfo->ExceptionRecord->ExceptionAddress;
	DWORD NumberParameters = apExceptionInfo->ExceptionRecord->NumberParameters;
	FILE* fp;
	LPCWSTR ExceptionString = L"INVALID";

	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);

	switch (ExceptionCode) {
	case(EXCEPTION_ACCESS_VIOLATION): ExceptionString = L"ACCESS VIOLATION"; break;
	case(EXCEPTION_ARRAY_BOUNDS_EXCEEDED): ExceptionString = L"OUT OF BOUNDS"; break;
	case(EXCEPTION_BREAKPOINT): ExceptionString = L"BREAKPOINT REACHED"; break;
	case(EXCEPTION_DATATYPE_MISALIGNMENT): ExceptionString = L"MISALIGNED DATA"; break;
	case(EXCEPTION_FLT_DENORMAL_OPERAND): ExceptionString = L"FLOAT TOO SMALL"; break;
	case(EXCEPTION_FLT_DIVIDE_BY_ZERO): ExceptionString = L"FLOAT DIV 0"; break;
	case(EXCEPTION_FLT_INEXACT_RESULT): ExceptionString = L"FLOAT RESULT ERROR"; break;
	case(EXCEPTION_FLT_INVALID_OPERATION): ExceptionString = L"FLOAT ERROR"; break;
	case(EXCEPTION_FLT_OVERFLOW): ExceptionString = L"FLOAT OVERFLOW"; break;
	case(EXCEPTION_FLT_STACK_CHECK): ExceptionString = L"FLOAT DERIVED STACK ERROR"; break;
	case(EXCEPTION_FLT_UNDERFLOW): ExceptionString = L"FLOAT UNDERFLOW"; break;
	case(EXCEPTION_ILLEGAL_INSTRUCTION): ExceptionString = L"ILLEGAL INSTRUCTION"; break;
	case(EXCEPTION_IN_PAGE_ERROR): ExceptionString = L"PAGE ERROR"; break;
	case(EXCEPTION_INT_DIVIDE_BY_ZERO): ExceptionString = L"INT DIV 0"; break;
	case(EXCEPTION_INT_OVERFLOW): ExceptionString = L"INTEGER OVERFLOW"; break;
	case(EXCEPTION_INVALID_DISPOSITION): ExceptionString = L"INVALID DISPOSITION"; break;
	case(EXCEPTION_NONCONTINUABLE_EXCEPTION): ExceptionString = L"BYPASSED UNHANDLED EXCEPTION"; break;
	case(EXCEPTION_PRIV_INSTRUCTION): ExceptionString = L"PRIVATE INSTRUCTION"; break;
	case(EXCEPTION_SINGLE_STEP): ExceptionString = L"SINGLE STEP OPERATION COMPLETED"; break;
	case(EXCEPTION_STACK_OVERFLOW): ExceptionString = L"STACK OVERFLOW"; break;
	};

	wprintf_s(L"----- [YuiSpy Activated] -----\n\n");
	wprintf_s(L"Exception Code: 0x%X (%s)\n", ExceptionCode, ExceptionString);
	wprintf_s(L"Exception Flags: %X\n", ExceptionFlags);
	wprintf_s(L"Fault Offset: 0x%llX\n", FaultOffset);
	wprintf_s(L"Number Parameters: %X\n", NumberParameters);

	if (ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
		DWORD64 InaccessbileMem = (DWORD64)apExceptionInfo->ExceptionRecord->ExceptionInformation[1];
		if (apExceptionInfo->ExceptionRecord->ExceptionInformation[0] == 0) {
			wprintf_s(L"The instruction at 0x%llX referenced memory at 0x%llX. The memory could not be read.\n", FaultOffset, InaccessbileMem);
		}
		else {
			wprintf_s(L"The instruction at 0x%llX referenced memory at 0x%llX. The memory could not be written.\n", FaultOffset, InaccessbileMem);
		};
	};

	wprintf_s(L"Generating register dump:\n");
	wprintf_s(L"RAX: 0x%llX\n", apExceptionInfo->ContextRecord->Rax);
	wprintf_s(L"RBX: 0x%llX\n", apExceptionInfo->ContextRecord->Rbx);
	wprintf_s(L"RCX: 0x%llX\n", apExceptionInfo->ContextRecord->Rcx);
	wprintf_s(L"RDX: 0x%llX\n", apExceptionInfo->ContextRecord->Rdx);
	wprintf_s(L"RDI: 0x%llX\n", apExceptionInfo->ContextRecord->Rdi);
	wprintf_s(L"RSI: 0x%llX\n", apExceptionInfo->ContextRecord->Rsi);
	wprintf_s(L"R8:  0x%llX\n", apExceptionInfo->ContextRecord->R8);
	wprintf_s(L"R9:  0x%llX\n", apExceptionInfo->ContextRecord->R9);
	wprintf_s(L"R10: 0x%llX\n", apExceptionInfo->ContextRecord->R10);
	wprintf_s(L"R11: 0x%llX\n", apExceptionInfo->ContextRecord->R11);
	wprintf_s(L"R12: 0x%llX\n", apExceptionInfo->ContextRecord->R12);
	wprintf_s(L"R13: 0x%llX\n", apExceptionInfo->ContextRecord->R13);
	wprintf_s(L"R14: 0x%llX\n", apExceptionInfo->ContextRecord->R14);
	wprintf_s(L"R15: 0x%llX\n", apExceptionInfo->ContextRecord->R15);

	wprintf_s(L"RBP: 0x%llX\n", apExceptionInfo->ContextRecord->Rbp);
	wprintf_s(L"RSP: 0x%llX\n\n", apExceptionInfo->ContextRecord->Rsp);

	wprintf_s(L"------ [YuiSpy Complete] -----");

	MessageBoxA(NULL, "The application has crashed.", "YuiSpy", MB_ICONERROR);

	FreeConsole();

	exit(1);

};