#pragma once
#include <EngineBase\EngineDebug.h>
#include <EngineBase\EngineString.h>
#include <EngineBase\EngineTime.h>
#include <EnginePlatform\EngineWindow.h>
#include <EnginePlatform\EngineInput.h>
#include <map>
#include <vector>


// ���� ������ ������ ����� ���� ������ �� �߻��� �� �ִ� ��ȯ ������ �����ϱ� ���� Ŭ���� ���� ����
class ULevel;

class UEngineCore
{
public:
	~UEngineCore();

	// delete Function
	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore& _Other) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

	// MainWindow, MainTimer�� ��ü�μ� �����θ� ��ȣ�� �ɷ��� ���� �ִ�.
	// ���� public���� �ܺο� �����ص� ū ���� ����.
	UEngineWindow MainWindow;
	EngineTime MainTimer;

	// ������ �����Ѵ�. 
	// - MainTimer, MainWindow �ʱ� �۾�
	// - ���� �ھ��� BeginPlay ȣ��
	// - ������ �޽��� ���� ����
	void EngineStart(HINSTANCE _hInstance);

	void CoreInit(HINSTANCE _Init);

	virtual void BeginPlay();
	virtual void Tick(float _DeltaTime);
	virtual void End();

	// ������ �����ϴ� �Լ�
	// - ������ BeginPlay �Լ��� ȣ�����ش�.
	// - ������ �ʿ� �����Ѵ�.		
	template<typename LevelType>
	void CreateLevel(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);

		if (true == AllLevel.contains(UpperName))
		{
			MsgBoxAssert(std::string(_Name) + "�̶�� �̸��� Level�� �� ������� �߽��ϴ�");
		}

		LevelType* NewLevel = new LevelType();
		LevelInit(NewLevel, _Name);
		AllLevel.insert(std::pair<std::string, ULevel*>(UpperName, NewLevel));
	}

	virtual void WindowOpen(std::string& _OutWindowTitle, std::string& _SmallIconPath) {}

	void DestroyLevel(std::string_view _Name);

	void ChangeLevel(std::string_view _Name);

	void SetFrame(int _Frame)
	{
		Frame = _Frame;
		FrameTime = 1 / static_cast<float>(Frame);
	}

	static bool IsDebug()
	{
		return IsDebugValue;
	}

	static void EngineDebugSwitch() {
		IsDebugValue = !IsDebugValue;
	}

protected:
	UEngineCore();

private:
	static bool IsDebugValue;

	// ������ ���� ��� ����
	int Frame = -1;					// ������ ���� ��ġ. Frame <= 0�� �������� �������� ������ �ǹ�.
	float FrameTime = 0.0f;			// (1 / Frame)�� ����.
	float CurFrameTime = 0.0f;

	// ������ ���� �� ������ ����
	std::string WindowTitle;
	std::string WindowIconPath;

	// ���� ����
	bool EngineInit = false;
	std::map<std::string, ULevel*> AllLevel;
	ULevel* CurLevel = nullptr;
	ULevel* NextLevel = nullptr;
	std::vector<std::string> DestroyLevelName;

	// �޽��� ���� �ݹ� �Լ�
	static void EngineTick();
	void CoreTick();
	static void EngineEnd();		// �ھ ������ �����߱� ������ �ھ ������ �������ؾ� �Ѵ�.

	void LevelInit(ULevel* _Level, std::string_view _Name); // ������ BeginPlay �Լ��� ȣ�����ִ� �Լ�
};

extern UEngineCore* GEngine;


#define ENGINESTART(USERCORE) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
    LeakCheck; \
	USERCORE NewUserCore = USERCORE(); \
	NewUserCore.EngineStart(hInstance); \
}