#pragma once
#include <map>
#include <string>
#include <string_view>
#include <Windows.h>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineTime.h>
#include <EnginePlatform/EngineWindow.h>

// ���� ������ ������ ����� ���� ������ �� �߻��� �� �ִ� ��ȯ ������ �����ϱ� ���� Ŭ���� ���� ����
class ULevel;

// ���� :
class UEngineCore
{
public:
	// MainWindow, MainTimer�� ��ü�μ� �����θ� ��ȣ�� �ɷ��� ���� �ִ�.
	// ���� public���� �ܺο� �����ص� ū ���� ����.
	UEngineWindow MainWindow;
	EngineTime MainTimer;

	// constructor destructor
	virtual ~UEngineCore();

	// delete Function
	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore& _Other) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

	// ������ �����Ѵ�. 
	// - MainTimer, MainWindow �ʱ� �۾�
	// - ���� �ھ��� BeginPlay ȣ��
	// - ������ �޽��� ���� ����
	static void EngineStart(HINSTANCE _hInstance, UEngineCore* _UserCore);

	void CoreInit(HINSTANCE _hInstance);

	virtual void BeginPlay() {}
	virtual void Tick(float _DeltaTime) {}
	virtual void End() {}

	// ������ �����ϴ� �Լ�
	// - ������ BeginPlay �Լ��� ȣ�����ش�.
	// - ������ �ʿ� �����Ѵ�.
	template<typename LevelType>
	void CreateLevel(std::string_view _View)
	{
		std::string UpperName = UEngineString::ToUpper(_View);

		if (AllLevel.contains(UpperName))
		{
			MsgBoxAssert(std::string(_View) + " ������ �ݺ��ؼ� �����Ϸ��� �߽��ϴ�.");
		}

		LevelType* NewLevel = new LevelType();
		LevelInit(NewLevel);
		AllLevel.insert(std::pair<std::string, ULevel*>(UpperName, NewLevel));
	}

	void ChangeLevel(std::string_view _View);

	// ������ ���� ����
	void SetFrame(int _Frame)
	{
		Frame = _Frame;
		FrameTime = 1 / static_cast<float>(Frame);
	}

protected:
	UEngineCore();
private:
	bool EngineInit = false;

	// ���� ����
	std::map<std::string, ULevel*> AllLevel;
	ULevel* CurLevel = nullptr;
	void LevelInit(ULevel* _Level); // ������ BeginPlay �Լ��� ȣ�����ִ� �Լ�


	// �޽��� ���� �ݹ� �Լ�
	void CoreTick();
	static void EngineTick();
	static void EngineEnd(); // �ھ ������ �����߱� ������ �ھ ������ �������ؾ� �Ѵ�.


	// ������ ���� ��� ����
	int Frame = -1;					// ������ ���� ��ġ. Frame <= 0�� �������� �������� ������ �ǹ�.
	float FrameTime = 0.0f;			// (1 / Frame)�� ����.
	float CurFrameTime = 0.0f;
};

extern UEngineCore* GEngine;

#define ENGINESTART(USERCORE) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	LeakCheck; \
	USERCORE MainCore = USERCORE(); \
	UEngineCore::EngineStart(hInstance, &MainCore); \
}