#pragma once
#include <EngineCore/Actor.h>
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"

class UBattleLevel;
class ABattleCanvas;
 
class ABattleTurnStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		PlayerEscapeFail,
		Switch,
		UseItem,
		MoveFail,
		MoveAnim,
		MoveDamage,
		MoveSecondaryEffect,
		EndOfTurn,
		End,
	};

	enum class EMoveResultMsg
	{
		None,
		Critical,
		TypeEffect,
	};

	enum class ESecondaryEffectState
	{
		None,
		StatStageEffect,
		ShowSEMessage,
	};
public:
	// constructor destructor
	ABattleTurnStateMachine();
	~ABattleTurnStateMachine();

	// delete Function
	ABattleTurnStateMachine(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine(ABattleTurnStateMachine&& _Other) noexcept = delete;
	ABattleTurnStateMachine& operator=(const ABattleTurnStateMachine& _Other) = delete;
	ABattleTurnStateMachine& operator=(ABattleTurnStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(
		ABattleCanvas* _Canvas, 
		UPokemon* _PlayerPokemon, UPokemon* _EnemyPokemon, 
		UStatStage& _PlayerStatStage, UStatStage& _EnemyStatStage, 
		EBattleAction _PlayerAction, int _PlayerMoveIndex
	);

	void Reset() {};

protected:

private:
	// 배틀 레벨 데이터
	ABattleCanvas* Canvas = nullptr;
	UPokemon* PlayerPokemon = nullptr; 
	UPokemon* EnemyPokemon = nullptr;
	UStatStage* PlayerStatStage = nullptr; 
	UStatStage* EnemyStatStage = nullptr;
	EBattleAction PlayerAction = EBattleAction::None;
	EBattleAction EnemyAction = EBattleAction::None;
	EPokemonMove PlayerMoveId = EPokemonMove::None;
	EPokemonMove EnemyMoveId = EPokemonMove::None;

	// 상수
	const float BattleMsgShowTime = 2.0f;
	const float DamageTime = 2.0f;
	const float SEEffectShowTime = 2.0f;

	// 고유 데이터
	ESubstate State = ESubstate::None;
	float Timer = 0.0f;
	bool IsFirstTurn = true;
	bool IsPlayerFirst = true;
	bool IsPlayerFirstEOT = true;
	UPokemon* Attacker = nullptr;
	UPokemon* Defender = nullptr;
	UStatStage* AttackerStatStage = nullptr;
	UStatStage* DefenderStatStage = nullptr;
	EPokemonMove AttackMoveId = EPokemonMove::None;
	EMoveResultMsg MoveResultMsg = EMoveResultMsg::None;

	// SubState
	ESecondaryEffectState SEState = ESecondaryEffectState::None;
	
	// Temporal Data
	FDamageResult Result;
	int PrevHp = 0;
	int NextHp = 0;
	std::wstring SEMessage;

	// 로직
	void Tick(float _DeltaTime) override;

	void GenerateEnemyAction();
	void DispatchTurn();
	void DispatchFight();
	void DispatchNextPhase();		// 다음 차례가 상대의 턴인지, 턴 종료인지 결정
	void DispatchSecondaryEffect();
	void DispatchEndOfTurn();

	// 처리 함수
	void ProcessEscapeFail(float _DeltaTime);
	void ProcessMoveFail(float _DeltaTime);
	void ProcessMoveAnim(float _DeltaTime);
	void ProcessMoveDamage(float _DeltaTime);
	void ProcessMoveSecondaryEffect(float _DeltaTime);
	void ProcessEndOfTurn(float _DeltaTime);

	// 유틸 함수
	void SetPlayerAsAttacker();
	void SetEnemyAsAttacker();
};

