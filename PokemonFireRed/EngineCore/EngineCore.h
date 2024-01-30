#pragma once
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>

// Ό³Έν :
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

protected:
	EngineCore();
private:

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
	GEngine = &MainCore; \
	\
	EngineWindow::Init(hInstance); \
	EngineWindow NewWindow; \
	NewWindow.Open("Pokemon Fire Red"); \
	return EngineWindow::WindowMessageLoop(nullptr, nullptr); \
}