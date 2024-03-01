#pragma once
#include <EngineCore/Actor.h>

class UBattleLevel;
class ABattleCanvas;

enum class EBattlePlayerAction
{
	None,
	EscapeSuccess,
	EscapeFail,
};

class ABattlePlayerActionStateMachine : public AActor
{
private:
	enum ECursorName
	{
		Fight,
		Bag,
		Pokemon,
		Run
	};

	enum class ESubstate
	{
		None,
		Select,
		ShowEscapeSuccessMsg,
		ShowEscapeFailMsg,
		End
	};
public:
	// constructor destructor
	ABattlePlayerActionStateMachine();
	~ABattlePlayerActionStateMachine();

	// delete Function
	ABattlePlayerActionStateMachine(const ABattlePlayerActionStateMachine& _Other) = delete;
	ABattlePlayerActionStateMachine(ABattlePlayerActionStateMachine&& _Other) noexcept = delete;
	ABattlePlayerActionStateMachine& operator=(const ABattlePlayerActionStateMachine& _Other) = delete;
	ABattlePlayerActionStateMachine& operator=(ABattlePlayerActionStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	EBattlePlayerAction GetPlayerActionResult() const
	{
		if (Result == EBattlePlayerAction::None)
		{
			MsgBoxAssert("플레이어 액션이 제대로 결정되지 않았습니다.");
			return Result;
		}

		return Result;
	}

	void Start(ABattleCanvas* _Canvas)
	{
		Canvas = _Canvas;
		State = ESubstate::Select;
	}

protected:

private:
	ABattleCanvas* Canvas = nullptr;
	float Timer = 0.0f;

	ESubstate State = ESubstate::None;
	EBattlePlayerAction Result = EBattlePlayerAction::None;

	void Tick(float _DeltaTime) override;

	void ProcessSelect(float _DeltaTime);
};

