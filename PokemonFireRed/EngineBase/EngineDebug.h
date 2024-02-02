#pragma once
#include <Windows.h>
#include <assert.h>
#include <string>
#include <string_view>

namespace EngineDebug
{
// _CrtSetDbgFlag: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/crtsetdbgflag?view=msvc-170
#define LeakCheck _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

// MessageBoxA: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messageboxa
// assert: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/assert-macro-assert-wassert?view=msvc-170
#define MsgBoxAssert(Value) \
	std::string ErrorText = std::string(Value); \
	MessageBoxA(nullptr, ErrorText.c_str(), "치명적 에러", MB_OK); assert(false);

	void OutPutDebugText(std::string_view _DebugText);
}

