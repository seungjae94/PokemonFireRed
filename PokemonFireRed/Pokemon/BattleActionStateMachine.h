#pragma once
#include "BattleStateMachine.h"
#include "Battler.h"
#include "ExpCalculator.h"
#include "BattleMoveStateMachine.h"
#include "BattlePlayerShiftStateMachine.h"
#include "BattleExpGainStateMachine.h"

class ABattleCanvas;
class APokemonMsgBox;

// 액션을 수행하는 역할
class ABattleActionStateMachine : public ABattleStateMachine
{
private:
	/*
	* MoveAnim -> MoveDamage -> MoveBE -> MoveSE
	*/
	enum class ESubstate
	{
		None,
		EscapeFail1,
		EscapeFail2,
		Shift,
		Ball,
		Move,
		End
	};

	enum class EMoveEffectState
	{
		None,
		ShowMoveEffect,
		ShowEffectResultMessage,
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

	void Start(UBattler* _Attacker, UBattler* _Defender);

	void SetBMSM(ABattleMoveStateMachine* _BMSM)
	{
		BattleMoveSM = _BMSM;
	}

	void SetBPSSM(ABattlePlayerShiftStateMachine* _BSSM)
	{
		BattleShiftSM = _BSSM;
	}

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

protected:
	void Tick(float _DeltaTime) override;
private:
	void Start() override {};
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;

	ESubstate State = ESubstate::None;
	float Timer = 0.0f;
	bool ProcessEnd = false;
	const float BattleMsgShowTime = 1.5f;

	// 상태 틱 함수
	void ProcessEscapeFail1();
	void ProcessEscapeFail2();
	void ProcessShift();
	void ProcessBall();
	void ProcessMove();

	// SM
	ABattleMoveStateMachine* BattleMoveSM = nullptr;
	ABattlePlayerShiftStateMachine* BattleShiftSM = nullptr;
};

