#include "EngineCore.h"
#include <Windows.h>
#include "Level.h"
#include "EnginePlatform\EngineInput.h"


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

	// ������ ����
	if (1 <= Frame)
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
	if (nullptr == CurLevel)
	{
		MsgBoxAssert("������ ������ ������ �������� �ʾҽ��ϴ� ġ������ �����Դϴ�");
	}

	// ������Ʈ ���� 1: ������ �� ƽ���� �� �ൿ
	CurLevel->Tick(DeltaTime);
	
	// ������Ʈ ���� 2: ������ ���Ե� ������Ʈ(e.g. ����)���� �� ƽ���� �� �ൿ 
	CurLevel->LevelTick(DeltaTime);

	// ������ ����
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
	// MainTimer �ð� ���� ����
	MainTimer.TimeCheckStart();

	// MainWindow ������ ����
	CoreInit(_hInstance);

	// ������ ������ ���� ���� ����
	BeginPlay();

	// �޽��� ���� ����
	UEngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
}

void UEngineCore::CoreInit(HINSTANCE _HINSTANCE)
{
	// CoreInit�� �� ���� ȣ��Ǿ�� �Ѵ�.
	if (true == EngineInit)
	{
		return;
	}

	// ������ ����
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
		MsgBoxAssert(std::string(_Name) + "��� �������� �ʴ� ������ ü���� �Ϸ��� �߽��ϴ�");
	}

	CurLevel = AllLevel[UpperName];
}

void UEngineCore::LevelInit(ULevel* _Level)
{
	_Level->BeginPlay();
}