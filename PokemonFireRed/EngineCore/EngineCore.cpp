#include "EngineCore.h"

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

void EngineCore::EngineTick()
{
}

void EngineCore::EngineEnd()
{
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