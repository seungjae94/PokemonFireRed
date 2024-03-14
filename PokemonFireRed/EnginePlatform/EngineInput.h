#pragma once

#include <Windows.h>
#include <map>
#include <EngineBase/EngineDebug.h>

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

		float PressTime = 0.0f;
		float UpTime = 0.0f;

		int Key = -1;		// Ű�� �̸�

		// GetAsyncKeyState �Լ��� ��ȯ���� �̿��� Ű�� ���¸� �������ִ� �Լ�
		void KeyCheck(float _DeltaTime);

		EngineKey()
		{
		}

		EngineKey(int _Key)
			: Key(_Key)
		{
		}
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
	static bool IsDoubleClick(int _Key, float _ClickTime)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�");
		}

		bool Value = AllKeys[_Key].Down;
		float Time = AllKeys[_Key].UpTime;

		if (true == AllKeys[_Key].Down && AllKeys[_Key].UpTime < _ClickTime)
		{
			return true;
		}

		return false;
	}

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

	// �� �����Ӹ��� EngineCore::EngineTick���� ȣ�����ִ� �Լ�
	static void KeyCheckTick(float _DeltaTime);

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

