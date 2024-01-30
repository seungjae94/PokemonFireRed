#pragma once
#include <string_view>
#include <Windows.h>

// ������(â)�� ���� �� ������ ���ɼ��� �ִ�.
class EngineWindow
{
public:
	// constructor destructor
	EngineWindow();
	~EngineWindow();

	// delete Function
	EngineWindow(const EngineWindow& _Other) = delete;
	EngineWindow(EngineWindow&& _Other) noexcept = delete;
	EngineWindow& operator=(const EngineWindow& _Other) = delete;
	EngineWindow& operator=(EngineWindow&& _Other) noexcept = delete;

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
	static int WindowMessageLoop(void (*_UpdateCallback)(), void (*_EndCallback)());

	// static ��� ���� hInstance�� �ʱ�ȭ�Ѵ�.
	static void Init(HINSTANCE _hInstance)
	{
		hInstance = _hInstance;
	}

protected:

private:
	// ������ �޽����� ó���ϴ� �⺻ ���ν���
	// - ��� �����찡 ������ ���̱� ������ static���� �����ߴ�.
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// �ν��Ͻ� �ڵ�
	// - ������ ������ ����� �� ����Ѵ�.
	// - ��� �����찡 ������ ���̱� ������ static���� �����ߴ�.
	static HINSTANCE hInstance;

	// ������ �ڵ�
	HWND hWnd = nullptr;
};

