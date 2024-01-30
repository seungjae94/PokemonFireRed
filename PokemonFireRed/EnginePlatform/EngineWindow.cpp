#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

// static 멤버 변수
HINSTANCE EngineWindow::hInstance = nullptr;

// 생성자, 소멸자
EngineWindow::EngineWindow()
{
}

EngineWindow::~EngineWindow()
{
}

void EngineWindow::Open(std::string_view _Title)
{
	// 윈도우 정보 등록
	WNDCLASSEXA wcex;									// 멀티바이트 문자를 사용할 것이기 때문에 WNDCLASSEXW 대신 WNDCLASSEXA를 사용

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;								// 아이콘은 사용하지 않는다.
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;						// 메뉴는 사용하지 않는다.
	wcex.lpszClassName = "DefaultWindow";
	wcex.hIconSm = nullptr;								// 아이콘은 사용하지 않는다.

	RegisterClassExA(&wcex);							// 멀티바이트 문자를 사용할 것이기 때문에 WNDCLASSEXW 대신 WNDCLASSEXA를 사용
	
	// 기본 옵션으로 윈도우 생성
	hWnd = CreateWindowA(wcex.lpszClassName, _Title.data(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// 예외 처리: 윈도우 생성 실패
	if (hWnd == nullptr)
	{
		MsgBoxAssert("윈도우 생성에 실패했습니다.");
		return;
	}

	// 윈도우를 실제로 화면에 띄운다.
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}


int EngineWindow::WindowMessageLoop(void(*_UpdateCallback)(), void(*_EndCallback)())
{
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		// hAccTable = nullptr : 메뉴 단축키는 사용하지 않는다.
		if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK EngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}