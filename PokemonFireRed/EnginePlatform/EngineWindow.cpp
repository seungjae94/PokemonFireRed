#include "EngineWindow.h"
#include <EngineBase\EngineDebug.h>
#include "WindowImage.h"

bool UEngineWindow::WindowLive = true;
HINSTANCE UEngineWindow::hInstance;


LRESULT CALLBACK UEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		WindowLive = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void UEngineWindow::Init(HINSTANCE _hInst)
{
	hInstance = _hInst;
}


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

void UEngineWindow::Open(std::string_view _Title /*= "Title"*/, std::string_view _IconPath /*= ""*/)
{
	HICON hIcon = nullptr;
	if ("" != _IconPath)
	{
		hIcon = (HICON)LoadImage( // returns a HANDLE so we have to cast to HICON
			NULL,             // hInstance must be NULL when loading from a file
			_IconPath.data(),   // the icon file name
			IMAGE_ICON,       // specifies that the file is an icon
			0,                // width of the image (we'll specify default later on)
			0,                // height of the image
			LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
			LR_DEFAULTSIZE |   // default metrics based on the type (IMAGE_ICON, 32x32)
			LR_SHARED         // let the system release the handle when it's no longer used
		);

	}

	// 윈도우 정보 등록
	WNDCLASSEXA wcex;							// 멀티바이트 문자를 사용할 것이기 때문에 WNDCLASSEXW 대신 WNDCLASSEXA를 사용

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;						// 아이콘은 사용하지 않는다.
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;				// 메뉴는 사용하지 않는다.
	wcex.lpszClassName = "DefaultWindow";		// 아이콘은 사용하지 않는다.
	wcex.hIconSm = nullptr;

	RegisterClassExA(&wcex);					// 멀티바이트 문자를 사용할 것이기 때문에 WNDCLASSEXW 대신 WNDCLASSEXA를 사용

	// 기본 옵션으로 윈도우 생성
	int Style = WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_THICKFRAME |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX;

	hWnd = CreateWindowA("DefaultWindow", _Title.data(), Style,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// 예외 처리: 윈도우 생성 실패
	if (!hWnd)
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

void UEngineWindow::WindowMessageLoop(std::function<void()> _Update, std::function<void()> _End)
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
		*/

		// 윈도우 메시지가 있는지 확인한다.
		// - PM_REMOVE: 읽은 메시지를 큐에서 제거한다.
		// - PM_NOREMOVE: 읽은 메시지를 큐에서 제거하지 않는다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// EngineWindow::WndProc로 윈도우 메시지를 처리한다.
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (false == WindowLive)
			{
				break;
			}
		}

		if (nullptr != _Update)
		{
			_Update();
		}
	}

	if (nullptr != _End)
	{
		_End();
	}
}

FVector UEngineWindow::GetMousePosition()
{
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(hWnd, &MousePoint);

	return FVector(MousePoint.x, MousePoint.y);
}

void UEngineWindow::SetWindowPosition(const FVector& _Pos)
{
	Position = _Pos;
	::SetWindowPos(hWnd, nullptr, Position.iX(), Position.iY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
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
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(ClearColor.Color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(BackBufferImage->ImageDC, myBrush);

	// 백버퍼 이미지를 ClearColor.Color 색으로 채운다.
	Rectangle(BackBufferImage->ImageDC, -1, -1, Scale.iX() + 1, Scale.iY() + 1);
	
	SelectObject(BackBufferImage->ImageDC, oldBrush);
	DeleteObject(myBrush);
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

void UEngineWindow::SetWindowSmallIcon()
{

}

void UEngineWindow::CursorOff()
{
	ShowCursor(FALSE);
}