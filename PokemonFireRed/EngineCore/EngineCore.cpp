#include "EngineCore.h"

#include "Level.h"

EngineCore* GEngine = nullptr;

EngineCore::EngineCore()
{
}

EngineCore::~EngineCore()
{
}

void EngineCore::EngineStart(HINSTANCE _hInstance, EngineCore* _UserCore)
{
	GEngine = _UserCore;

	// 엔진 초기 작업
	// - MainWindow 윈도우 띄우기
	GEngine->CoreInit(_hInstance);

	// 유저가 정의한 게임 로직 시작
	GEngine->BeginPlay();

	// 메시지 루프 시작
	EngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
}


void EngineCore::CoreInit(HINSTANCE _hInstance)
{
	// CoreInit은 한 번만 호출되어야 한다.
	if (EngineInit)
	{
		return;
	}

	EngineWindow::Init(_hInstance);
	MainWindow.Open("Title");

	EngineInit = true;
}

void EngineCore::ChangeLevel(std::string_view _View)
{
	std::string UpperName = UEngineString::ToUpper(_View);

	if (!AllLevel.contains(UpperName))
	{
		MsgBoxAssert(UpperName + "는 존재하지 않는 레벨입니다. 레벨을 변경할 수 없습니다.");
		return;
	}

	CurLevel = AllLevel[UpperName];
}

void EngineCore::LevelInit(ULevel* _Level)
{
	_Level->BeginPlay();
}

void EngineCore::EngineTick()
{
	float DeltaTime = GEngine->MainTimer.TimeCheck();

	// 업데이트 구조 1: 레벨이 매 틱마다 할 행동
	GEngine->CurLevel->Tick(DeltaTime);

	// 업데이트 구조 2: 레벨에 포함된 오브젝트(e.g. 액터)들이 매 틱마다 할 행동 
	GEngine->CurLevel->LevelTick(DeltaTime);

	// 렌더링 구조 (차후 구현)
	GEngine->CurLevel->LevelRender(DeltaTime);
	
	// 릴리즈 구조: Destroy한 오브젝트 릴리즈
	GEngine->CurLevel->LevelRelease(DeltaTime);
}

void EngineCore::EngineEnd()
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