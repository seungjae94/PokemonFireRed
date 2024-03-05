#pragma once
#include <EngineCore/Actor.h>
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"
#include "Battler.h"
#include "BattleActionStateMachine.h"
#include "BattleEOTStateMachine.h"
#include "BattleFaintStateMachine.h"

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
		Action1Faint,
		TestAction2,
		Action2,
		Action2Faint,
		StartEndOfTurn,
		TestEndOfTurn1,
		EndOfTurn1,
		EndOfTurn1Faint,
		TestEndOfTurn2,
		EndOfTurn2,
		EndOfTurn2Faint,
		End
	};

	enum class EFaintState
	{
		None,
		HidePokemon,
		ShowFaintMessage,
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

	void SetFSM(ABattleFaintStateMachine* _FSM)
	{
		BattleFaintSM = _FSM;
	}

	void Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy);

protected:

private:
	// ��Ʋ ���� ������
	ABattleCanvas* Canvas = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	// ���� ƽ �Լ�
	void ProcessAction1();
	void ProcessAction1Faint();  // Faint ó���� �� �ϰ� ���� ��Ʋ ����, ������ ��ŵ ���� ó��
	void ProcessTestAction2();
	void ProcessAction2();
	void ProcessAction2Faint();
	void ProcessTestEndOfTurn1();
	void ProcessStartEndOfTurn();
	void ProcessEndOfTurn1();
	void ProcessEndOfTurn1Faint();
	void ProcessTestEndOfTurn2();
	void ProcessEndOfTurn2();
	void ProcessEndOfTurn2Faint();

	// ���� ���� �Լ�
	void StateChangeToFaint();
	void StateChangeToExpGain();

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
	void SwapAttackerAndDefender();
	void EndTurnWithReason();

	// SM
	ABattleActionStateMachine* BattleActionSM = nullptr;
	ABattleEOTStateMachine* BattleEOTSM = nullptr;
	ABattleFaintStateMachine* BattleFaintSM = nullptr;
};

