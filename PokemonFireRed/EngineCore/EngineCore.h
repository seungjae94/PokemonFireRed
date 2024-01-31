#pragma once
#include <map>
#include <string>
#include <string_view>
#include <Windows.h>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineTime.h>
#include <EnginePlatform/EngineWindow.h>

// 하위 개념인 레벨을 헤더로 직접 참조할 때 발생할 수 있는 순환 참조를 방지하기 위해 클래스 전방 선언
class ULevel;

// 설명 :
class EngineCore
{
public:
	// MainWindow, MainTimer는 객체로서 스스로를 보호할 능력을 갖고 있다.
	// 따라서 public으로 외부에 공개해도 큰 문제 없다.
	EngineWindow MainWindow;
	EngineTime MainTimer;

	// constructor destructor
	virtual ~EngineCore();

	// delete Function
	EngineCore(const EngineCore& _Other) = delete;
	EngineCore(EngineCore&& _Other) noexcept = delete;
	EngineCore& operator=(const EngineCore& _Other) = delete;
	EngineCore& operator=(EngineCore&& _Other) noexcept = delete;

	// 엔진을 시작한다. 
	// - MainTimer, MainWindow 초기 작업
	// - 유저 코어의 BeginPlay 호출
	// - 윈도우 메시지 루프 시작
	static void EngineStart(HINSTANCE _hInstance, EngineCore* _UserCore);

	void CoreInit(HINSTANCE _hInstance);

	virtual void BeginPlay() {}
	virtual void Tick(float _DeltaTime) {}
	virtual void End() {}

	// 레벨을 생성하는 함수
	// - 레벨의 BeginPlay 함수를 호출해준다.
	// - 레벨을 맵에 저장한다.
	template<typename LevelType>
	void CreateLevel(std::string_view _View)
	{
		std::string UpperName = UEngineString::ToUpper(_View);

		if (AllLevel.contains(UpperName))
		{
			MsgBoxAssert(std::string(_View) + " 레벨을 반복해서 생성하려고 했습니다.");
			return;
		}

		LevelType* NewLevel = new LevelType();
		LevelInit(NewLevel);
		AllLevel.insert(std::pair<std::string, ULevel*>(UpperName, NewLevel));
	}

	void ChangeLevel(std::string_view _View);

	// 프레임 제한 관련
	void SetFrame(int _Frame)
	{
		Frame = _Frame;
		FrameTime = 1 / static_cast<float>(Frame);
	}

protected:
	EngineCore();
private:
	bool EngineInit = false;

	// 레벨 관련
	std::map<std::string, ULevel*> AllLevel;
	ULevel* CurLevel = nullptr;
	// 레벨의 BeginPlay 함수를 호출해주는 함수
	void LevelInit(ULevel* _Level);


	// 메시지 루프 콜백 함수
	void CoreTick();
	static void EngineTick();
	// 코어가 레벨을 생성했기 때문에 코어가 레벨을 릴리즈해야 한다.
	static void EngineEnd();


	// 프레임 제한 기능 관련
	int Frame = -1;					// 프레임 제한 수치. Frame <= 0은 프레임을 제한하지 않음을 의미.
	float FrameTime = 0.0f;			// (1 / Frame)과 동일.
	float CurFrameTime = 0.0f;
};

extern EngineCore* GEngine;

#define ENGINESTART(USERCORE) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	LeakCheck; \
	USERCORE MainCore = USERCORE(); \
	EngineCore::EngineStart(hInstance, &MainCore); \
}