#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"
#include "BattleCanvas.h"
#include "BattleMoveStateMachine.h"

// 액션을 수행하는 역할
class ABattleActionStateMachine : public AActor
{
private:
	/*
	* MoveAnim -> MoveDamage -> MoveBE -> MoveSE
	*/
	enum class ESubstate
	{
		None,
		EscapeFail,
		Switch,
		UseItem,
		Move,
		TestFaint,
		Faint,
		TestExpGain,
		ExpGain,
		End
	};

	enum class EMoveEffectState
	{
		None,
		ShowMoveEffect,
		ShowEffectResultMessage,
	};

	enum class EFaintState
	{
		None,
		HidePokemon,
		ShowFaintMessage,
	};

	enum class EExpGainState
	{
		None,
		ExpGainMessage,
		LevelUpMessage,
		LearnMove,
	};

public:
	// constructor destructor
	ABattleActionStateMachine();
	~ABattleActionStateMachine();

	// delete Function
	ABattleActionStateMachine(const ABattleActionStateMachine& _Other) = delete;
	ABattleActionStateMachine(ABattleActionStateMachine&& _Other) noexcept = delete;
	ABattleActionStateMachine& operator=(const ABattleActionStateMachine& _Other) = delete;
	ABattleActionStateMachine& operator=(ABattleActionStateMachine&& _Other) noexcept = delete;

	void Start(ABattleCanvas* _Canvas, UBattler* _Attacker, UBattler* _Defender);
	
	void SetBMSM(ABattleMoveStateMachine* _BMSM)
	{
		BattleMoveSM = _BMSM;
	}

	bool IsEnd() const 
	{
		return State == ESubstate::End;
	}

protected:
	void Tick(float _DeltaTime) override;
private:
	ABattleCanvas* Canvas = nullptr;
	UBattler* Attacker = nullptr; 
	UBattler* Defender = nullptr;

	ESubstate State = ESubstate::None;
	float Timer = 0.0f;
	bool ProcessEnd = false;
	const float BattleMsgShowTime = 1.5f;
	
	// 페인트 이펙트
	EFaintState FaintState = EFaintState::None;
	UBattler* Fainter = nullptr;
	std::list<UBattler*> Fainters;
	const float FaintTime = 0.5f;

	// 경험치 획득 이펙트
	EExpGainState ExpGainState = EExpGainState::None;
	std::map<const UPokemon*, int> ExpGainResult;

	// 상태 틱 함수
	void ProcessEscapeFail();
	void ProcessSwitch();
	void ProcessItem();
	void ProcessMove();
	void ProcessTestFaint();
	void ProcessFaint();
	void ProcessTestExpGain();
	void ProcessExpGain();

	void StateChangeToFaint();
	void StateChangeToExpGain();

	// SM
	ABattleMoveStateMachine* BattleMoveSM = nullptr;
};

