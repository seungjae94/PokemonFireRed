#pragma once
#include <EngineCore/Actor.h>
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"
#include "Battler.h"
#include "BattleActionStateMachine.h"
#include "BattleEOTStateMachine.h"

class UBattleLevel;
class ABattleCanvas;
struct FMoveEffectTestResult;

// �� ������ �����ϴ� ����
class ABattleTurnStateMachine : public AActor
{
public:
	enum class EEndReason
	{
		None,
		WinToWild,
		WinToTrainer,
		LoseToWild,
		LoseToTrainer,
	};
private:
	enum class ESubstate
	{
		None,
		Action1,
		Action2,
		EndOfTurn1,
		EndOfTurn2,
		End
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

	EEndReason WhyEnd() const
	{
		return Reason;
	}

	void SetBASM(ABattleActionStateMachine* _BASM)
	{
		BattleActionSM = _BASM;
	}

	void SetEOTSM(ABattleEOTStateMachine* _EOTSM)
	{
		BattleEOTSM = _EOTSM;
	}

	void Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy);

protected:

private:
	// ��Ʋ ���� ������
	ABattleCanvas* Canvas = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	void ProcessAction1(float _DeltaTime);
	void ProcessAction2(float _DeltaTime);
	void ProcessEndOfTurn1(float _DeltaTime);
	void ProcessEndOfTurn2(float _DeltaTime);

	void SwapAttackerAndDefender();
	
	void EndTurnWithReason();

	// ���� ������
	ESubstate State = ESubstate::None;
	EEndReason Reason = EEndReason::None;

	float Timer = 0.0f;
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;
	UBattler* EOTTarget = nullptr;
	UBattler* EOTNextTarget = nullptr;

	// ����
	void Tick(float _DeltaTime) override;

	// ��ƿ �Լ�
	void SetPlayerAsAttacker();
	void SetEnemyAsAttacker();
	void SetPlayerAsEOTTarget();
	void SetEnemyAsEOTTarget();

	// SM
	ABattleActionStateMachine* BattleActionSM = nullptr;
	ABattleEOTStateMachine* BattleEOTSM = nullptr;
};

