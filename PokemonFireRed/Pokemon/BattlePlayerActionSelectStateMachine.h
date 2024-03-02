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
			MsgBoxAssert("�÷��̾� �׼��� ����� �������� �ʾҽ��ϴ�.");
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
	// ��Ʋ ���� ������
	ABattleCanvas* Canvas = nullptr;
	const UPokemon* PlayerPokemon = nullptr;
	const UPokemon* EnemyPokemon = nullptr;
	EBattleAction PlayerAction = EBattleAction::None;

	// ���� ������
	float Timer = 0.0f;
	int RunAttemptCount = 0;

	// ����
	ESubstate State = ESubstate::None;

	// ��� ������ (��Ʋ �������� ������ ������)
	EBattleAction ActionResult = EBattleAction::None;
	int SelectedMoveIndex = 0;
	bool RunResult = false;
	int SelectedPokemonIndex = 0;
	// TODO: SelectedItem

	// ����
	void Tick(float _DeltaTime) override;

	void ProcessSelect(float _DeltaTime);
	bool CalcRunResult();

	void ProcessMoveSelect(float _DeltaTime);
};

