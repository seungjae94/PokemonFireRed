#include "EngineWindow.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/Transform.h>
#include "WindowImage.h"

// static 멤버 변수
HINSTANCE UEngineWindow::hInstance = nullptr;
bool UEngineWindow::WindowLive = true;

// 생성자, 소멸자
UEngineWindow::UEngineWindow()
{
}

UEngineWindow::~UEngineWindow()
{
	if (nullptr != BackBufferImage)
	{
		delete BackBufferImage;
		BackBufferImage = nullptr;
	}

	if (nullptr != WindowImage)
	{
		delete WindowImage;
		WindowImage = nullptr;
	}
}

void UEngineWindow::Open(std::string_view _Title)
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
	int Style = WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_THICKFRAME |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX;

	hWnd = CreateWindowA(wcex.lpszClassName, _Title.data(), Style,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// 예외 처리: 윈도우 생성 실패
	if (hWnd == nullptr)
	{
		MsgBoxAssert("윈도우 생성에 실패했습니다.");
		return;
	}

	HDC hDC = GetDC(hWnd);
	if (nullptr == WindowImage)
	{
		WindowImage = new UWindowImage();
		WindowImage->Create(hDC);
	}

	// 윈도우를 실제로 화면에 띄운다.
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}


unsigned __int64 UEngineWindow::WindowMessageLoop(void(*_Update)(), void(*_End)())
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

LRESULT CALLBACK UEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

void UEngineWindow::SetWindowScale(const FVector& _Scale)
{
	Scale = _Scale;

	// 새로운 화면 크기에 맞는 백버퍼 이미지를 생성한다.
	if (nullptr != BackBufferImage)
	{
		delete BackBufferImage;
		BackBufferImage = nullptr;
	}

	BackBufferImage = new UWindowImage();
	BackBufferImage->Create(WindowImage, Scale);

	// 메뉴 등의 요소를 포함한 윈도우의 크기를 계산한다.
	RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 윈도우의 크기를 조정한다.
	// https://learn.microsoft.com/ko-kr/windows/win32/api/winuser/nf-winuser-setwindowpos
	::SetWindowPos(hWnd, nullptr, 0, 0, Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

void UEngineWindow::ScreenClear()
{
	// 백버퍼 이미지를 흰색으로 채운다.
	Rectangle(BackBufferImage->ImageDC, -1, -1, Scale.iX() + 1, Scale.iY() + 1);
}

void UEngineWindow::ScreenUpdate()
{
	// 화면 전체를 채우는 트랜스폼을 만든다.
	FTransform CopyTrans;
	CopyTrans.SetPosition({ Scale.ihX(), Scale.ihY() });
	CopyTrans.SetScale({ Scale.iX(), Scale.iY() });

	// 윈도우 이미지 전체에 백버퍼 이미지를 그린다.
	WindowImage->BitCopy(BackBufferImage, CopyTrans);
}