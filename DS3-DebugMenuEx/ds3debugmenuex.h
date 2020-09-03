#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <stdio.h>

#include "dinput8/dinputWrapper.h"
#include "MinHook/include/MinHook.h"

BYTE mov1ToAlBytes[5] = { 0xB0, 0x01, 0x90, 0x90, 0x90 };
BYTE nopBytes[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
BYTE xorRaxBytes[5] = { 0x48, 0x31, 0xC0, 0x90, 0x90 };
BYTE jmpBytes[2] = { 0xEB, 0x71 };

extern "C" wchar_t DSFontTpfPath[] = L"font:/DSFont24.tpf";
extern "C" wchar_t DSFontCcmPath[] = L"font:/DSFont24.ccm";
extern "C" wchar_t TalkFontTpfPath[] = L"font:/TalkFont.tpf";
extern "C" wchar_t TalkFontCcmPath[] = L"font:/TalkFont.ccm";

extern "C" wchar_t ezSelectBotDebugBootMenuString[] = L"SprjEzSelectBot.DebugBootMenuStep";

extern "C" wchar_t debugBootMenuStringModelViewer[] = L"MODEL_VIEWER";
extern "C" wchar_t debugBootMenuStringSoundTest[] = L"SOUND_TEST";
extern "C" wchar_t debugBootMenuStringMapViewer[] = L"MAP_VIEWER";
extern "C" wchar_t debugBootMenuStringStageSelect[] = L"STAGE_SELECT";
extern "C" wchar_t debugBootMenuStringAutoTest[] = L"AUTO_TEST";
extern "C" wchar_t debugBootMenuStringTitleBoot[] = L"TITLE_BOOT";
extern "C" wchar_t debugBootMenuStringE3TitleBoot[] = L"E3_TITLE_BOOT";
extern "C" wchar_t debugBootMenuStringGamescomTitleBoot[] = L"GAMESCOM_TITLE_BOOT";
extern "C" wchar_t debugBootMenuStringLoadTest[] = L"LOAD_TEST";
extern "C" wchar_t debugBoogMenuStringDebugMoveMapTest[] = L"DEBUG_MOVEMAP_TEST";
extern "C" wchar_t debugBootMenuStringMoveMapTest[] = L"MOVEMAP_TEST";
extern "C" wchar_t debugBootMenuStringMoveMapTestCeremony[] = L"MOVEMAP_TEST_CEREMONY";
extern "C" wchar_t debugBootMenuStringModuleTest[] = L"MODULE_TEST";
extern "C" wchar_t debugBootMenuStringMenuViewer[] = L"MENU_VIEWER";
extern "C" wchar_t debugBootMenuStringSfxViewer[] = L"SFX_VIEWER";

extern "C" wchar_t debugBootMenuTextlistPath[] = L"testdata:/debugBootMenu.textlist";
extern "C" wchar_t debugBootMenuString[] = L"DEBUG BOOT MENU <デバッグ起動メニュー>";

extern "C" VOID tLoadDbgFont();
extern "C" DWORD64 bLoadDbgFont;

extern "C" VOID fDbgDispLoading();

extern "C" DWORD64 bInitDebugBootMenuStep;
extern "C" DWORD64 bInitMoveMapListStep;
extern "C" DWORD64 bGameStepSelection;
extern "C" DWORD64 bCheckDebugDashSwitch;
extern "C" DWORD64 bLoadGameProperties;
extern "C" DWORD64 bDecWindowCounter;
extern "C" DWORD64 bIncWindowCounter;
extern "C" VOID tInitDebugBootMenuStep();
extern "C" VOID tInitMoveMapListStep();
extern "C" VOID tGameStepSelection();
extern "C" VOID tCheckDebugDashSwitch();
extern "C" VOID tLoadGameProperties();
extern "C" VOID initDebugBootMenuStepFunctions();
extern "C" VOID sub_140CF3520();
extern "C" VOID patchMoveMapFinishAntiTamper();
extern "C" VOID decWindowCounter();
extern "C" VOID incWindowCounter();

extern "C" VOID debugBootMenuStepDtor();
extern "C" VOID EzTextlistSelectorDtor();
extern "C" VOID MoveMapListStepDtor();

extern "C" DWORD64* debugBootMenuStepVtable;
extern "C" DWORD64* EzTextlistSelectorVtable;
extern "C" DWORD64* MoveMapListStepVtable;

LONG WINAPI UHFilter(struct _EXCEPTION_POINTERS* apExceptionInfo);