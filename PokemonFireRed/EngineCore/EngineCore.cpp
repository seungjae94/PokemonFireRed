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

	// ���� �ʱ� �۾�
	// - MainWindow ������ ����
	GEngine->CoreInit(_hInstance);

	// ������ ������ ���� ���� ����
	GEngine->BeginPlay();

	// �޽��� ���� ����
	EngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
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

void EngineCore::ChangeLevel(std::string_view _View)
{
	std::string UpperName = UEngineString::ToUpper(_View);

	if (!AllLevel.contains(UpperName))
	{
		MsgBoxAssert(UpperName + "�� �������� �ʴ� �����Դϴ�. ������ ������ �� �����ϴ�.");
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

	// ������Ʈ ���� 1: ������ �� ƽ���� �� �ൿ
	GEngine->CurLevel->Tick(DeltaTime);

	// ������Ʈ ���� 2: ������ ���Ե� ������Ʈ(e.g. ����)���� �� ƽ���� �� �ൿ 
	GEngine->CurLevel->LevelTick(DeltaTime);

	// ������ ���� (���� ����)
	GEngine->CurLevel->LevelRender(DeltaTime);
	
	// ������ ����: Destroy�� ������Ʈ ������
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