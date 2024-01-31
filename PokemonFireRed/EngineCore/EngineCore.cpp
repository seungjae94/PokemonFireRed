#include "EngineCore.h"

#include <EnginePlatform/EngineInput.h>
#include "Level.h"

UEngineCore* GEngine = nullptr;

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::EngineStart(HINSTANCE _hInstance, UEngineCore* _UserCore)
{
	GEngine = _UserCore;

	// MainTimer 시간 측정 시작
	GEngine->MainTimer.TimeCheckStart();

	// MainWindow 윈도우 띄우기
	GEngine->CoreInit(_hInstance);

	// 유저가 정의한 게임 로직 시작
	GEngine->BeginPlay();

	// 메시지 루프 시작
	UEngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
}


void UEngineCore::CoreInit(HINSTANCE _hInstance)
{
	// CoreInit은 한 번만 호출되어야 한다.
	if (EngineInit)
	{
		return;
	}

	// 윈도우 띄우기
	UEngineWindow::Init(_hInstance);
	MainWindow.Open("Title");

	EngineInit = true;
}

void UEngineCore::ChangeLevel(std::string_view _View)
{
	std::string UpperName = UEngineString::ToUpper(_View);

	if (!AllLevel.contains(UpperName))
	{
		MsgBoxAssert(UpperName + "는 존재하지 않는 레벨입니다. 레벨을 변경할 수 없습니다.");
		return;
	}

	CurLevel = AllLevel[UpperName];
}

void UEngineCore::LevelInit(ULevel* _Level)
{
	_Level->BeginPlay();
}

void UEngineCore::CoreTick()
{
	float DeltaTime = MainTimer.TimeCheck();

	// 프레임 제한
	if (Frame >= 1)
	{
		CurFrameTime += DeltaTime;

		if (CurFrameTime <= FrameTime)
		{
			return;
		}

		// CurFrameTime이 FrameTime을 초과하는 경우 잔여 수치를 보존한다.
		CurFrameTime -= FrameTime;

		// 보통 프레임 제한을 걸 때는 모든 프레임에서 일정한 수치만큼 업데이트 하길 원한다.
		DeltaTime = FrameTime;
	}


	// 키 입력 체크
	EngineInput::KeyCheckTick(DeltaTime);

	// 예외 처리: 현재 레벨이 설정되지 않은 경우
	if (CurLevel == nullptr)
	{
		MsgBoxAssert("엔진을 시작할 레벨이 지정되지 않았습니다.");
		return;
	}

	// 업데이트 구조 1: 레벨이 매 틱마다 할 행동
	CurLevel->Tick(DeltaTime);

	// 업데이트 구조 2: 레벨에 포함된 오브젝트(e.g. 액터)들이 매 틱마다 할 행동 
	CurLevel->LevelTick(DeltaTime);

	// 렌더링 구조 (차후 구현)
	MainWindow.ScreenClear();
	CurLevel->LevelRender(DeltaTime);
	MainWindow.ScreenUpdate();

	// 릴리즈 구조: Destroy한 오브젝트 릴리즈
	CurLevel->LevelRelease(DeltaTime);
}

void UEngineCore::EngineTick()
{
	GEngine->CoreTick();
}

void UEngineCore::EngineEnd()
{
	for (std::pair<const std::string, ULevel*>& Pair : GEngine->AllLevel)
	{
		ULevel* Level = Pair.second;

		if (Level == nullptr)
		{
			continue;
		}

		delete Level;
		Pair.second = nullptr;
	}

	GEngine->AllLevel.clear();
}