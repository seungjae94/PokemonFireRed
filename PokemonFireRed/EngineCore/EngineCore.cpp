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

void UEngineCore::EngineTick()
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

	// ����� ���Ǽ��� ���� �ɼ� ���
	// - ��������� �ڵ带 ��������� �� DeltaTime�� Ŀ���鼭 ĳ���Ͱ� �� ���� �����̴� ������ �ذ�
	if (1.0f / 60.0f <= DeltaTime)
	{
		DeltaTime = 1.0f / 60.0f;
	}

	// ���� �ý��� ������Ʈ
	UEngineSound::Update();

	// Ű �Է� üũ
	UEngineInput::KeyCheckTick(DeltaTime);

	for (size_t i = 0; i < DestroyLevelName.size(); i++)
	{
		std::string UpperName = UEngineString::ToUpper(DestroyLevelName[i]);

		ULevel* Level = Levels[UpperName];

		Levels.erase(DestroyLevelName[i]);

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

	// ���� ����
	// - �� ������ ���ȿ��� ������ �����Ǿ�� ������ �����ϱ� ����.
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

	// ���� ó��: ���� ������ �������� ���� ���
	if (nullptr == CurLevel)
	{
		MsgBoxAssert("������ ������ ������ �������� �ʾҽ��ϴ� ġ������ �����Դϴ�");
	}

	GEngine->Tick(DeltaTime);

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

void UEngineCore::EngineEnd()
{
	for (std::pair<const std::string, ULevel*>& _Pair : GEngine->Levels)
	{
		if (nullptr == _Pair.second)
		{
			continue;
		}

		delete _Pair.second;
		_Pair.second = nullptr;
	}

	GEngine->Levels.clear();
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
	UEngineWindow::WindowMessageLoop(std::bind(&UEngineCore::EngineTick, this), std::bind(&UEngineCore::EngineEnd, this));
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

	if (false == Levels.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "��� �������� �ʴ� ������ ü���� �Ϸ��� �߽��ϴ�");
	}

	NextLevel = Levels[UpperName];
}

void UEngineCore::LevelInit(ULevel* _Level, std::string_view _Name)
{
	_Level->SetName(_Name);
	_Level->BeginPlay();
}

void UEngineCore::DestroyLevel(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == Levels.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "�������� �ʴ� ������ �ı��Ҽ��� �����ϴ�");
	}

	DestroyLevelName.push_back(UpperName);
}