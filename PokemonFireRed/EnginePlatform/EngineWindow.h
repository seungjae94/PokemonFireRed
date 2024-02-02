#pragma once
#include <Windows.h>
#include <string>
#include <EngineBase\EngineMath.h>

class UWindowImage;

// 윈도우(창)는 여러 개 존재할 가능성이 있다.
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

	// 실제 윈도우를 화면에 띄운다.
	void Open(std::string_view _Title = "Title");

	// static 멤버 변수 hInstance를 초기화한다.
	static void Init(HINSTANCE _hInst);

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
	// 메시지 루프의 지속 여부
	static bool WindowLive;	

	// 인스턴스 핸들
	// - 윈도우 정보를 등록할 때 사용한다.
	// - 모든 윈도우가 공유할 것이기 때문에 static으로 선언했다.
	static HINSTANCE hInstance;

	// 윈도우 메시지를 처리하는 기본 프로시저
	// - 모든 윈도우가 공유할 것이기 때문에 static으로 선언했다.
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// 윈도우 핸들
	HWND hWnd = nullptr;

	// 윈도우 이미지, 백버퍼 이미지 (더블 버퍼링)
	UWindowImage* WindowImage = nullptr;
	UWindowImage* BackBufferImage = nullptr;

	// 윈도우 데이터
	FVector Scale;
	FVector Position;
};

