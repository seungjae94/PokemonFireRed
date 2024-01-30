#pragma once
#include <Windows.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>

// 설명 :
class EngineCore
{
public:
	// constructor destructor
	~EngineCore();

	// delete Function
	EngineCore(const EngineCore& _Other) = delete;
	EngineCore(EngineCore&& _Other) noexcept = delete;
	EngineCore& operator=(const EngineCore& _Other) = delete;
	EngineCore& operator=(EngineCore&& _Other) noexcept = delete;

	// 엔진을 시작한다. 
	// 1. 엔진이 MainWindow를 생성하고, MainWindow.Open 함수를 호출한다.
	// 2. 
	static void EngineStart(HINSTANCE _hInstance, EngineCore* _UserCore);

	void CoreInit(HINSTANCE _hInstance);

	virtual void BeginPlay() {}

protected:
	EngineCore();
private:
	EngineWindow MainWindow;
	bool EngineInit = false;

	static void EngineTick();
	static void EngineEnd();
};

extern EngineCore* GEngine;

//#define ENGINESTART(USERCORE) \
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
//	_In_opt_ HINSTANCE hPrevInstance, \
//	_In_ LPWSTR    lpCmdLine, \
//	_In_ int       nCmdShow) \
//{ \
//	LeakCheck; \
//	USERCORE MainCore = USERCORE(); \
//	EngineCore::EngineStart(hInstance, &MainCore); \	
//}