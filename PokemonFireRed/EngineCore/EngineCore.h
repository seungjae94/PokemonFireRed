#pragma once
#include <EngineBase\EngineDebug.h>
#include <EngineBase\EngineString.h>
#include <EngineBase\EngineTime.h>

#include <EnginePlatform\EngineWindow.h>
#include <EnginePlatform\EngineInput.h>
#include <map>


// 하위 개념인 레벨을 헤더로 직접 참조할 때 발생할 수 있는 순환 참조를 방지하기 위해 클래스 전방 선언
class ULevel;

class UEngineCore
{
public:
	~UEngineCore();

	// delete Function
	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore& _Other) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

	// MainWindow, MainTimer는 객체로서 스스로를 보호할 능력을 갖고 있다.
	// 따라서 public으로 외부에 공개해도 큰 문제 없다.
	UEngineWindow MainWindow;
	EngineTime MainTimer;

	// 엔진을 시작한다. 
	// - MainTimer, MainWindow 초기 작업
	// - 유저 코어의 BeginPlay 호출
	// - 윈도우 메시지 루프 시작
	void EngineStart(HINSTANCE _hInstance);

	void CoreInit(HINSTANCE _Init);

	virtual void BeginPlay();
	virtual void Tick(float _DeltaTime);
	virtual void End();

	// 레벨을 생성하는 함수
	// - 레벨의 BeginPlay 함수를 호출해준다.
	// - 레벨을 맵에 저장한다.		
	template<typename LevelType>
	void CreateLevel(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);

		if (true == AllLevel.contains(UpperName))
		{
			MsgBoxAssert(std::string(_Name) + "이라는 이름의 Level을 또 만들려고 했습니다");
		}

		LevelType* NewLevel = new LevelType();
		LevelInit(NewLevel);
		AllLevel.insert(std::pair<std::string, ULevel*>(UpperName, NewLevel));
	}

	void ChangeLevel(std::string_view _Name);

	void SetFrame(int _Frame)
	{
		Frame = _Frame;
		FrameTime = 1 / static_cast<float>(Frame);
	}

protected:
	UEngineCore();

private:
	// 프레임 제한 기능 관련
	int Frame = -1;					// 프레임 제한 수치. Frame <= 0은 프레임을 제한하지 않음을 의미.
	float FrameTime = 0.0f;			// (1 / Frame)과 동일.
	float CurFrameTime = 0.0f;

	// 레벨 관련
	bool EngineInit = false;
	std::map<std::string, ULevel*> AllLevel;
	ULevel* CurLevel = nullptr;
	ULevel* NextLevel = nullptr;
	void LevelInit(ULevel* _Level); // 레벨의 BeginPlay 함수를 호출해주는 함수

	// 메시지 루프 콜백 함수
	static void EngineTick();
	void CoreTick();
	static void EngineEnd();		// 코어가 레벨을 생성했기 때문에 코어가 레벨을 릴리즈해야 한다.

};

extern UEngineCore* GEngine;


#define ENGINESTART(USERCORE) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
    LeakCheck; \
	USERCORE NewUserCore = USERCORE(); \
	NewUserCore.EngineStart(hInstance); \
}