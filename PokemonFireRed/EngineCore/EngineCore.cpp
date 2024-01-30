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