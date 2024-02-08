#pragma once

#include <Windows.h>
#include <map>
#include <EngineBase/EngineDebug.h>

// 설명 :
class EngineInput
{
	friend class InputInitCreator;
private:
	class EngineKey
	{
		friend EngineInput;
	public:
		bool Down = false;	// 이번 프레임에 키를 눌렀는가?
		bool Up = false;	// 이번 프레임에 키를 뗐는가?
		bool Press = false; // 키가 눌려있는가?
		bool Free = true;   // 키가 떼어져있는가?

		int Key = -1;		// 키의 이름

		float PressTime = 0.0f;

		EngineKey()
		{
		}

		EngineKey(int _Key)
			: Key(_Key)
		{
		}

		// GetAsyncKeyState 함수의 반환값을 이용해 키의 상태를 갱신해주는 함수
		void KeyCheck();
	};
public:
	// constructor destructor
	EngineInput();
	~EngineInput();

	// delete Function
	EngineInput(const EngineInput& _Other) = delete;
	EngineInput(EngineInput&& _Other) noexcept = delete;
	EngineInput& operator=(const EngineInput& _Other) = delete;
	EngineInput& operator=(EngineInput&& _Other) noexcept = delete;

	// 키 상태 확인 함수

	static bool IsDown(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력 설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Down;
	}

	static bool IsPress(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력 설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Press;
	}

	static bool IsUp(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력 설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Up;
	}

	static bool IsFree(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("입력설정이 존재하지 않는 키 입니다");
		}

		return AllKeys[_Key].Free;
	}

	// 매 프레임마다 EngineCore::EngineTick에서 호출해주는 함수
	static void KeyCheckTick(float _DeltaTime);

	// '아무 키나 누르세요' 기능을 위한 함수
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
	// AllKeys에 키 객체를 매핑해주는 함수
	static void InputInit();
};

