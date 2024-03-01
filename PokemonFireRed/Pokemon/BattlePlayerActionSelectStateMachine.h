#pragma once
#include <EngineCore/Actor.h>
#include "PlayerData.h"
#include "Pokemon.h"

class UBattleLevel;
class ABattleCanvas;

enum class EBattlePlayerAction
{
	None,
	Fight,
	EscapeSuccess,
	EscapeFail,
	Shift,
	Item,
};

class ABattlePlayerActionSelectStateMachine : public AActor
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
		MoveSelect,
		ShowEscapeSuccessMsg,
		ShowEscapeFailMsg,
		End
	};
public:
	// constructor destructor
	ABattlePlayerActionSelectStateMachine();
	~ABattlePlayerActionSelectStateMachine();

	// delete Function
	ABattlePlayerActionSelectStateMachine(const ABattlePlayerActionSelectStateMachine& _Other) = delete;
	ABattlePlayerActionSelectStateMachine(ABattlePlayerActionSelectStateMachine&& _Other) noexcept = delete;
	ABattlePlayerActionSelectStateMachine& operator=(const ABattlePlayerActionSelectStateMachine& _Other) = delete;
	ABattlePlayerActionSelectStateMachine& operator=(ABattlePlayerActionSelectStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	EBattlePlayerAction GetPlayerActionResult() const
	{
		if (ActionResult == EBattlePlayerAction::None)
		{
			MsgBoxAssert("플레이어 액션이 제대로 결정되지 않았습니다.");
			return ActionResult;
		}

		return ActionResult;
	}

	int GetSelectedMoveIndex() const
	{
		return SelectedMoveIndex;
	}

	void Start(ABattleCanvas* _Canvas, const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon);

	void Reset();

protected:

private:
	const UPokemon* PlayerPokemon = nullptr;
	const UPokemon* EnemyPokemon = nullptr;
	EBattlePlayerAction PlayerAction = EBattlePlayerAction::None;

	ABattleCanvas* Canvas = nullptr;
	float Timer = 0.0f;

	ESubstate State = ESubstate::None;
	EBattlePlayerAction ActionResult = EBattlePlayerAction::None;
	int SelectedMoveIndex = 0;
	int RunAttemptCount = 0;

	// 로직
	void Tick(float _DeltaTime) override;

	void ProcessSelect(float _DeltaTime);
	bool CalcRunResult();

	void ProcessMoveSelect(float _DeltaTime);
	void ProcessShowEscapeSuccessMsg(float _DeltaTime);
	void ProcessShowEscapeFailMsg(float _DeltaTime);
};

