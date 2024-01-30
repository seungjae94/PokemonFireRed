#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

// static ��� ����
HINSTANCE EngineWindow::hInstance = nullptr;

// ������, �Ҹ���
EngineWindow::EngineWindow()
{
}

EngineWindow::~EngineWindow()
{
}

void EngineWindow::Open(std::string_view _Title)
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
	hWnd = CreateWindowA(wcex.lpszClassName, _Title.data(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// ���� ó��: ������ ���� ����
	if (hWnd == nullptr)
	{
		MsgBoxAssert("������ ������ �����߽��ϴ�.");
		return;
	}

	// �����츦 ������ ȭ�鿡 ����.
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}


int EngineWindow::WindowMessageLoop(void(*_UpdateCallback)(), void(*_EndCallback)())
{
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		// hAccTable = nullptr : �޴� ����Ű�� ������� �ʴ´�.
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