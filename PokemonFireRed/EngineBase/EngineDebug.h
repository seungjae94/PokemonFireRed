#pragma once
#include <crtdbg.h>
#include <Windows.h>
#include <assert.h>
#include <string>

// _CrtSetDbgFlag: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/crtsetdbgflag?view=msvc-170
#define LeakCheck _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

// MessageBoxA: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa
// assert: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/assert-macro-assert-wassert?view=msvc-170
#define MsgBoxAssert(TEXT) \
std::string STR_TEXT = std::string(TEXT); \
MessageBoxA(nullptr, STR_TEXT.c_str(), "치명적 에러", MB_OK); assert(false);