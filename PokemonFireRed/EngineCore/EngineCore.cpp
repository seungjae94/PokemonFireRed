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
	
	// ���� �ʱ� �۾�
	// - MainWindow ������ ����
	GEngine->CoreInit(_hInstance);

	// ������ ������ ���� ���� ����
	GEngine->BeginPlay();

	// �޽��� ���� ����
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
	// CoreInit�� �� ���� ȣ��Ǿ�� �Ѵ�.
	if (EngineInit)
	{
		return;
	}

	EngineWindow::Init(_hInstance);
	MainWindow.Open("Title");

	EngineInit = true;
}