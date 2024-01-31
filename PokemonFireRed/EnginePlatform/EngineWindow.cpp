#include "EngineWindow.h"

#include <EngineBase/EngineDebug.h>
#include <EngineBase/Transform.h>
#include "WindowImage.h"

// static ��� ����
HINSTANCE UEngineWindow::hInstance = nullptr;
bool UEngineWindow::WindowLive = true;

// ������, �Ҹ���
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
	// ������ ���� ���
	WNDCLASSEXA wcex;									// ��Ƽ����Ʈ ���ڸ� ����� ���̱� ������ WNDCLASSEXW ��� WNDCLASSEXA�� ���

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;								// �������� ������� �ʴ´�.
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;						// �޴��� ������� �ʴ´�.
	wcex.lpszClassName = "DefaultWindow";
	wcex.hIconSm = nullptr;								// �������� ������� �ʴ´�.

	RegisterClassExA(&wcex);							// ��Ƽ����Ʈ ���ڸ� ����� ���̱� ������ WNDCLASSEXW ��� WNDCLASSEXA�� ���
	
	// �⺻ �ɼ����� ������ ����
	int Style = WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_THICKFRAME |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX;

	hWnd = CreateWindowA(wcex.lpszClassName, _Title.data(), Style,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// ���� ó��: ������ ���� ����
	if (hWnd == nullptr)
	{
		MsgBoxAssert("������ ������ �����߽��ϴ�.");
		return;
	}

	HDC hDC = GetDC(hWnd);
	if (nullptr == WindowImage)
	{
		WindowImage = new UWindowImage();
		WindowImage->Create(hDC);
	}

	// �����츦 ������ ȭ�鿡 ����.
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}


unsigned __int64 UEngineWindow::WindowMessageLoop(void(*_Update)(), void(*_End)())
{
	MSG msg = {};

	while (WindowLive)
	{
		/*
		* GetMessage �Լ��� ������ �޽��� ť�� �޽����� ���� ������ ��� ����Ѵ�.
		* ���� ������ �޽����� ���� ������ �ڵ� ������ �ߴܵȴ�.
		* GetMessage �Լ��� WM_QUIT �޽����� �о��� ���� false�� ��ȯ�Ѵ�.
		* 
		* �ݸ� PeekMessage �Լ��� ������ �޽��� ť�� �޽����� ������ �ٷ� false�� ��ȯ�Ѵ�.
		* 
		*/

		// ������ �޽����� �ִ��� Ȯ���Ѵ�.
		// - PM_REMOVE: ���� �޽����� ť���� �����Ѵ�.
		// - PM_NOREMOVE: ���� �޽����� ť���� �������� �ʴ´�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// EngineWindow::WndProc�� ������ �޽����� ó���Ѵ�.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ������Ʈ �ݹ�
		if (_Update != nullptr)
		{
			_Update();
		}
	}

	// ���� �ݹ�
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

	// ���ο� ȭ�� ũ�⿡ �´� ����� �̹����� �����Ѵ�.
	if (nullptr != BackBufferImage)
	{
		delete BackBufferImage;
		BackBufferImage = nullptr;
	}

	BackBufferImage = new UWindowImage();
	BackBufferImage->Create(WindowImage, Scale);

	// �޴� ���� ��Ҹ� ������ �������� ũ�⸦ ����Ѵ�.
	RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

	// �������� ũ�⸦ �����Ѵ�.
	// https://learn.microsoft.com/ko-kr/windows/win32/api/winuser/nf-winuser-setwindowpos
	::SetWindowPos(hWnd, nullptr, 0, 0, Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

void UEngineWindow::ScreenClear()
{
	// ����� �̹����� ������� ä���.
	Rectangle(BackBufferImage->ImageDC, -1, -1, Scale.iX() + 1, Scale.iY() + 1);
}

void UEngineWindow::ScreenUpdate()
{
	// ȭ�� ��ü�� ä��� Ʈ�������� �����.
	FTransform CopyTrans;
	CopyTrans.SetPosition({ Scale.ihX(), Scale.ihY() });
	CopyTrans.SetScale({ Scale.iX(), Scale.iY() });

	// ������ �̹��� ��ü�� ����� �̹����� �׸���.
	WindowImage->BitCopy(BackBufferImage, CopyTrans);
}