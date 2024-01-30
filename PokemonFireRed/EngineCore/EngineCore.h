#pragma once
#include <Windows.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>

// ���� :
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

	// ������ �����Ѵ�. 
	// 1. ������ MainWindow�� �����ϰ�, MainWindow.Open �Լ��� ȣ���Ѵ�.
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