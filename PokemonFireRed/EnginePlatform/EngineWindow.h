#pragma once
#include <Windows.h>
#include <string>
#include <EngineBase\EngineMath.h>

class UWindowImage;

// ������(â)�� ���� �� ������ ���ɼ��� �ִ�.
class UEngineWindow
{
public:
	UEngineWindow();
	~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	// ���� �����츦 ȭ�鿡 ����.
	void Open(std::string_view _Title = "Title");

	// static ��� ���� hInstance�� �ʱ�ȭ�Ѵ�.
	static void Init(HINSTANCE _hInst);

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
	static unsigned __int64 WindowMessageLoop(void(*_Update)(), void(*_End)());

	UWindowImage* GetWindowImage()
	{
		return WindowImage;
	}

	UWindowImage* GetBackBufferImage()
	{
		return BackBufferImage;
	}

	void SetWindowPosition(const FVector& _Pos);
	void SetWindowScale(const FVector& _Scale);

	void ScreenClear();
	void ScreenUpdate();

	FVector GetWindowScale()
	{
		return Scale;
	}

	FVector GetMousePosition();

protected:

private:
	// �޽��� ������ ���� ����
	static bool WindowLive;	

	// �ν��Ͻ� �ڵ�
	// - ������ ������ ����� �� ����Ѵ�.
	// - ��� �����찡 ������ ���̱� ������ static���� �����ߴ�.
	static HINSTANCE hInstance;

	// ������ �޽����� ó���ϴ� �⺻ ���ν���
	// - ��� �����찡 ������ ���̱� ������ static���� �����ߴ�.
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// ������ �ڵ�
	HWND hWnd = nullptr;

	// ������ �̹���, ����� �̹��� (���� ���۸�)
	UWindowImage* WindowImage = nullptr;
	UWindowImage* BackBufferImage = nullptr;

	// ������ ������
	FVector Scale;
	FVector Position;
};

