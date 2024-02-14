#pragma once

#include <Windows.h>
#include <map>
#include <EngineBase/EngineDebug.h>

// ���� :
class UEngineInput
{
	friend class InputInitCreator;
private:
	class EngineKey
	{
		friend UEngineInput;
	public:
		bool Down = false;	// �̹� �����ӿ� Ű�� �����°�?
		bool Up = false;	// �̹� �����ӿ� Ű�� �ô°�?
		bool Press = false; // Ű�� �����ִ°�?
		bool Free = true;   // Ű�� �������ִ°�?

		int Key = -1;		// Ű�� �̸�

		float PressTime = 0.0f;

		EngineKey()
		{
		}

		EngineKey(int _Key)
			: Key(_Key)
		{
		}

		// GetAsyncKeyState �Լ��� ��ȯ���� �̿��� Ű�� ���¸� �������ִ� �Լ�
		void KeyCheck(float _DeltaTime);
	};
public:
	// constructor destructor
	UEngineInput();
	~UEngineInput();

	// delete Function
	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& _Other) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;

	// Ű ���� Ȯ�� �Լ�

	static bool IsDown(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է� ������ �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Down;
	}

	static float GetPressTime(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է� ������ �������� �ʴ� Ű �Դϴ�.");
		}

		return AllKeys[_Key].PressTime;
	}

	static bool IsPress(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է� ������ �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Press;
	}

	static bool IsUp(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է� ������ �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Up;
	}

	static bool IsFree(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Free;
	}

	// �� �����Ӹ��� EngineCore::EngineTick���� ȣ�����ִ� �Լ�
	static void KeyCheckTick(float _DeltaTime);

	// '�ƹ� Ű�� ��������' ����� ���� �Լ�
	static bool IsAnykeyDown()
	{
		return AnykeyDown;
	}
	static bool IsAnykeyPress()
	{
		return AnykeyPress;
	}
	static bool IsAnykeyUp()
	{
		return AnykeyUp;
	}
	static bool IsAnykeyFree()
	{
		return AnykeyFree;
	}

protected:
	static std::map<int, EngineKey> AllKeys;

	static bool AnykeyDown;
	static bool AnykeyPress;
	static bool AnykeyUp;
	static bool AnykeyFree;

private:
	// AllKeys�� Ű ��ü�� �������ִ� �Լ�
	static void InputInit();
};

