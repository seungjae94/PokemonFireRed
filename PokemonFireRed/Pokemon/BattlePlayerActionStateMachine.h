#pragma once
#include <EngineCore/Actor.h>
#include "PlayerData.h"
#include "Pokemon.h"

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
		MoveSelect,
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
		if (ActionResult == EBattlePlayerAction::None)
		{
			MsgBoxAssert("플레이어 액션이 제대로 결정되지 않았습니다.");
			return ActionResult;
		}

		return ActionResult;
	}

	void Start(ABattleCanvas* _Canvas, int _PlayerPokemonIndex, const UPokemon* _EnemyPokemon)
	{
		Canvas = _Canvas;
		PlayerPokemonIndex = _PlayerPokemonIndex;
		EnemyPokemon = _EnemyPokemon;
		State = ESubstate::Select;
	}

	void Reset()
	{
		RunAttemptCount = 0;
	}

protected:

private:
	int PlayerPokemonIndex = 0;
	const UPokemon* EnemyPokemon = nullptr;
	EBattlePlayerAction PlayerAction = EBattlePlayerAction::None;

	ABattleCanvas* Canvas = nullptr;
	float Timer = 0.0f;

	ESubstate State = ESubstate::None;
	EBattlePlayerAction ActionResult = EBattlePlayerAction::None;
	int RunAttemptCount = 0;

	// 로직
	void Tick(float _DeltaTime) override;

	void ProcessSelect(float _DeltaTime);
	bool CalcRunResult();

	void ProcessMoveSelect(float _DeltaTime);
	void ProcessShowEscapeSuccessMsg(float _DeltaTime);
	void ProcessShowEscapeFailMsg(float _DeltaTime);

	// 유틸
	UPokemon& GetCurPlayerPokemon()
	{
		return UPlayerData::GetPokemonInEntry(PlayerPokemonIndex);
	}
};

