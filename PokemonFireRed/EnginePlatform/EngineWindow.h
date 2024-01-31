#pragma once
#include <string_view>
#include <Windows.h>
#include <EngineBase\EngineMath.h>

class UWindowImage;

// ������(â)�� ���� �� ������ ���ɼ��� �ִ�.
class UEngineWindow
{
public:
	// constructor destructor
	UEngineWindow();
	~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	// ���� �����츦 ȭ�鿡 ����.
	void Open(std::string_view _Title = "Title");

	/// <summary>
	/// ������ �޽��� ������ ������ �Լ�.
	/// ���α׷��� �޽��� ������ �ϳ��� �����ϱ� ������ static���� �����ߴ�.
	/// </summary>
	/// <param name="_UpdateCallback">
	///		���� �ȿ��� ȣ���� �ݹ�
	/// </param>
	/// <param name="_EndCallback">
	///		������ ����� �� ȣ���� �ݹ�
	/// </param>
	static unsigned __int64 WindowMessageLoop(void (*_Update)(), void (*_End)());

	// static ��� ���� hInstance�� �ʱ�ȭ�Ѵ�.
	static void Init(HINSTANCE _hInstance)
	{
		hInstance = _hInstance;
	}

	UWindowImage* GetWindowImage()
	{
		return WindowImage;
	}

	UWindowImage* GetBackBufferImage()
	{
		return BackBufferImage;
	}

	void SetWindowScale(const FVector& _Scale);

	void ScreenClear();
	void ScreenUpdate();

protected:

private:
	// ������ �޽����� ó���ϴ� �⺻ ���ν���
	// - ��� �����찡 ������ ���̱� ������ static���� �����ߴ�.
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// �ν��Ͻ� �ڵ�
	// - ������ ������ ����� �� ����Ѵ�.
	// - ��� �����찡 ������ ���̱� ������ static���� �����ߴ�.
	static HINSTANCE hInstance;

	// �޽��� ������ ���� ����
	static bool WindowLive;

	// ������ �ڵ�
	HWND hWnd = nullptr;

	// ������ �̹���, ����� �̹��� (���� ���۸�)
	UWindowImage* WindowImage = nullptr;
	UWindowImage* BackBufferImage = nullptr;

	// ������ ũ��
	FVector Scale;
};

