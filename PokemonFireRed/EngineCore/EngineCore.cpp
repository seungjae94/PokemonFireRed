#include "EngineCore.h"
#include <Windows.h>
#include "Level.h"
#include <EnginePlatform\EngineSound.h>
#include <EnginePlatform\EngineInput.h>

bool UEngineCore::IsDebugValue = false;
UEngineCore* GEngine = nullptr;

UEngineCore::UEngineCore()
	: MainWindow()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::CoreTick()
{
	float DeltaTime = MainTimer.TimeCheck();
	double dDeltaTime = MainTimer.GetDeltaTime();

	// 프레임 제한
	if (1 <= Frame)
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

	// 디버깅 편의성을 위한 옵션 기능
	// - 디버깅으로 코드를 멈춰놓았을 때 DeltaTime이 커지면서 캐릭터가 한 번에 움직이는 문제를 해결
	if (1.0f / 60.0f <= DeltaTime)
	{
		DeltaTime = 1.0f / 60.0f;
	}

	// 사운드 시스템 업데이트
	UEngineSound::Update();

	// 키 입력 체크
	UEngineInput::KeyCheckTick(DeltaTime);

	for (size_t i = 0; i < DestroyLevelName.size(); i++)
	{
		std::string UpperName = UEngineString::ToUpper(DestroyLevelName[i]);

		ULevel* Level = AllLevel[UpperName];

		AllLevel.erase(DestroyLevelName[i]);

		if (Level == CurLevel)
		{
			CurLevel = nullptr;
		}

		Level->End();

		if (nullptr != Level)
		{
			delete Level;
			Level = nullptr;
		}
	}
	DestroyLevelName.clear();

	// 레벨 변경
	// - 한 프레임 동안에는 레벨이 유지되어야 동작을 예상하기 쉽다.
	if (nullptr != NextLevel)
	{
		if (nullptr != CurLevel)
		{
			CurLevel->LevelEnd(NextLevel);
		}
		NextLevel->LevelStart(CurLevel);
		CurLevel = NextLevel;
		NextLevel = nullptr;

		MainTimer.TimeCheckStart();
		DeltaTime = MainTimer.TimeCheck();
		CurFrameTime = 0.0f;
	}

	// 예외 처리: 현재 레벨이 설정되지 않은 경우
	if (nullptr == CurLevel)
	{
		MsgBoxAssert("엔진을 시작할 레벨이 지정되지 않았습니다 치명적인 오류입니다");
	}

	GEngine->Tick(DeltaTime);

	// 업데이트 구조 1: 레벨이 매 틱마다 할 행동
	CurLevel->Tick(DeltaTime);
	
	// 업데이트 구조 2: 레벨에 포함된 오브젝트(e.g. 액터)들이 매 틱마다 할 행동 
	CurLevel->LevelTick(DeltaTime);

	// 렌더링 구조
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
	for (std::pair<const std::string, ULevel*>& _Pair : GEngine->AllLevel)
	{
		if (nullptr == _Pair.second)
		{
			continue;
		}

		delete _Pair.second;
		_Pair.second = nullptr;
	}

	GEngine->AllLevel.clear();
}

void UEngineCore::EngineStart(HINSTANCE _hInstance)
{
	GEngine = this;
	// MainTimer 시간 측정 시작
	MainTimer.TimeCheckStart();

	// MainWindow 윈도우 띄우기
	CoreInit(_hInstance);

	// 유저가 정의한 게임 로직 시작
	BeginPlay();

	// 메시지 루프 시작
	UEngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
}

void UEngineCore::CoreInit(HINSTANCE _HINSTANCE)
{
	// CoreInit은 한 번만 호출되어야 한다.
	if (true == EngineInit)
	{
		return;
	}

	// 윈도우 띄우기
	UEngineWindow::Init(_HINSTANCE);
	MainWindow.Open();

	EngineInit = true;
}

void UEngineCore::BeginPlay()
{

}

void UEngineCore::Tick(float _DeltaTime)
{

}

void UEngineCore::End()
{

}

void UEngineCore::ChangeLevel(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllLevel.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "라는 존재하지 않는 레벨로 체인지 하려고 했습니다");
	}

	NextLevel = AllLevel[UpperName];
}

void UEngineCore::LevelInit(ULevel* _Level, std::string_view _Name)
{
	_Level->SetName(_Name);
	_Level->BeginPlay();
}

void UEngineCore::DestroyLevel(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllLevel.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "존재하지 않는 레벨을 파괴할수는 없습니다");
	}

	DestroyLevelName.push_back(UpperName);
}