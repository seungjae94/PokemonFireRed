#pragma once
#include <EngineCore/Actor.h>
#include "PlayerData.h"
#include "Pokemon.h"
#include "BattleEnums.h"
#include "Battler.h"

class UBattleLevel;
class ABattleCanvas;

class ABattlePlayerActionSelectStateMachine : public AActor
{
public:
	enum class EPlayerActionResult
	{

	};
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
		PokemonSelect,
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


	void Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy);

	void Reset();

	// Pokemon UI에서 호출하는 함수
	UBattler* GetPlayer() const
	{
		return Player;
	}

protected:

private:
	// 배틀 레벨 데이터
	ABattleCanvas* Canvas = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	// 고유 데이터
	float Timer = 0.0f;
	int RunAttemptCount = 0;
	int SelectedPokemonIndex = 0;

	// 상태
	ESubstate State = ESubstate::None;

	// 로직
	void Tick(float _DeltaTime) override;

	// 상태 틱 함수
	void ProcessSelect();
	void ProcessMoveSelect();
	void ProcessPokemonSelect();

	// 유틸 함수
	bool CalcRunResult() const;
};

