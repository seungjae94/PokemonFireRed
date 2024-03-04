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
		Faint,
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

	// 상태 틱 함수
	void ProcessEscapeFail();
	void ProcessSwitch();
	void ProcessItem();
	void ProcessMove();
	void ProcessFaint();

	void StateChangeToFaint();

	// SM
	ABattleMoveStateMachine* BattleMoveSM = nullptr;
};

