#pragma once
#include <EngineCore/Actor.h>
#include "PlayerData.h"
#include "Pokemon.h"
#include "BattleEnums.h"

class UBattleLevel;
class ABattleCanvas;

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

	EBattleAction GetPlayerActionResult() const
	{
		if (ActionResult == EBattleAction::None)
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

	int GetRunResult() const
	{
		return RunResult;
	}

	void Start(ABattleCanvas* _Canvas, const UPokemon* _PlayerPokemon, const UPokemon* _EnemyPokemon);

	void Reset();

protected:

private:
	// 배틀 레벨 데이터
	ABattleCanvas* Canvas = nullptr;
	const UPokemon* PlayerPokemon = nullptr;
	const UPokemon* EnemyPokemon = nullptr;
	EBattleAction PlayerAction = EBattleAction::None;

	// 고유 데이터
	float Timer = 0.0f;
	int RunAttemptCount = 0;

	// 상태
	ESubstate State = ESubstate::None;

	// 출력 데이터 (배틀 레벨에서 참조할 데이터)
	EBattleAction ActionResult = EBattleAction::None;
	int SelectedMoveIndex = 0;
	bool RunResult = false;
	int SelectedPokemonIndex = 0;
	// TODO: SelectedItem

	// 로직
	void Tick(float _DeltaTime) override;

	void ProcessSelect(float _DeltaTime);
	bool CalcRunResult();

	void ProcessMoveSelect(float _DeltaTime);
};

