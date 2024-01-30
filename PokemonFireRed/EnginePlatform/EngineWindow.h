#pragma once
#include <string_view>
#include <Windows.h>

// 윈도우(창)는 여러 개 존재할 가능성이 있다.
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

	// 실제 윈도우를 화면에 띄운다.
	void Open(std::string_view _Title = "Title");

	/// <summary>
	/// 윈도우 메시지 루프를 돌리는 함수.
	/// 프로그램에 메시지 루프는 하나만 존재하기 때문에 static으로 선언했다.
	/// </summary>
	/// <param name="_UpdateCallback">
	///		루프 안에서 호출할 콜백
	/// </param>
	/// <param name="_EndCallback">
	///		루프가 종료된 뒤 호출할 콜백
	/// </param>
	static int WindowMessageLoop(void (*_UpdateCallback)(), void (*_EndCallback)());

	// static 멤버 변수 hInstance를 초기화한다.
	static void Init(HINSTANCE _hInstance)
	{
		hInstance = _hInstance;
	}

protected:

private:
	// 윈도우 메시지를 처리하는 기본 프로시저
	// - 모든 윈도우가 공유할 것이기 때문에 static으로 선언했다.
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 인스턴스 핸들
	// - 윈도우 정보를 등록할 때 사용한다.
	// - 모든 윈도우가 공유할 것이기 때문에 static으로 선언했다.
	static HINSTANCE hInstance;

	// 윈도우 핸들
	HWND hWnd = nullptr;
};

