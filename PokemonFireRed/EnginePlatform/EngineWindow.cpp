#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

// static 멤버 변수
HINSTANCE EngineWindow::hInstance = nullptr;
bool EngineWindow::WindowLive = true;

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


unsigned __int64 EngineWindow::WindowMessageLoop(void(*_Update)(), void(*_End)())
{
	MSG msg = {};

	while (WindowLive)
	{
		/*
		* GetMessage 함수는 윈도우 메시지 큐에 메시지가 들어올 때까지 계속 대기한다.
		* 따라서 윈도우 메시지가 들어올 때까지 코드 실행이 중단된다.
		* GetMessage 함수는 WM_QUIT 메시지를 읽었을 때만 false를 반환한다.
		* 
		* 반면 PeekMessage 함수는 윈도우 메시지 큐에 메시지가 없으면 바로 false를 반환한다.
		* 
		*/

		// 윈도우 메시지가 있는지 확인한다.
		// - PM_REMOVE: 읽은 메시지를 큐에서 제거한다.
		// - PM_NOREMOVE: 읽은 메시지를 큐에서 제거하지 않는다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// EngineWindow::WndProc로 윈도우 메시지를 처리한다.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 업데이트 콜백
		if (_Update != nullptr)
		{
			_Update();
		}
	}

	// 종료 콜백
	if (_End != nullptr)
	{
		_End();
	}

	return msg.wParam;
}

LRESULT CALLBACK EngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		WindowLive = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}