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

	// MainTimer �ð� ���� ����
	GEngine->MainTimer.TimeCheckStart();

	// MainWindow ������ ����
	GEngine->CoreInit(_hInstance);

	// ������ ������ ���� ���� ����
	GEngine->BeginPlay();

	// �޽��� ���� ����
	UEngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
}


void UEngineCore::CoreInit(HINSTANCE _hInstance)
{
	// CoreInit�� �� ���� ȣ��Ǿ�� �Ѵ�.
	if (EngineInit)
	{
		return;
	}

	// ������ ����
	UEngineWindow::Init(_hInstance);
	MainWindow.Open("Title");

	EngineInit = true;
}

void UEngineCore::ChangeLevel(std::string_view _View)
{
	std::string UpperName = UEngineString::ToUpper(_View);

	if (!AllLevel.contains(UpperName))
	{
		MsgBoxAssert(UpperName + "�� �������� �ʴ� �����Դϴ�. ������ ������ �� �����ϴ�.");
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

	// ������ ����
	if (Frame >= 1)
	{
		CurFrameTime += DeltaTime;

		if (CurFrameTime <= FrameTime)
		{
			return;
		}

		// CurFrameTime�� FrameTime�� �ʰ��ϴ� ��� �ܿ� ��ġ�� �����Ѵ�.
		CurFrameTime -= FrameTime;

		// ���� ������ ������ �� ���� ��� �����ӿ��� ������ ��ġ��ŭ ������Ʈ �ϱ� ���Ѵ�.
		DeltaTime = FrameTime;
	}


	// Ű �Է� üũ
	EngineInput::KeyCheckTick(DeltaTime);

	// ���� ó��: ���� ������ �������� ���� ���
	if (CurLevel == nullptr)
	{
		MsgBoxAssert("������ ������ ������ �������� �ʾҽ��ϴ�.");
		return;
	}

	// ������Ʈ ���� 1: ������ �� ƽ���� �� �ൿ
	CurLevel->Tick(DeltaTime);

	// ������Ʈ ���� 2: ������ ���Ե� ������Ʈ(e.g. ����)���� �� ƽ���� �� �ൿ 
	CurLevel->LevelTick(DeltaTime);

	// ������ ���� (���� ����)
	MainWindow.ScreenClear();
	CurLevel->LevelRender(DeltaTime);
	MainWindow.ScreenUpdate();

	// ������ ����: Destroy�� ������Ʈ ������
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