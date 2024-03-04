#pragma once
#include <EngineCore/Actor.h>
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"
#include "Battler.h"

class UBattleLevel;
class ABattleCanvas;
struct FMoveEffectTestResult;
 
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
		MoveBattleEffect,
		MoveSecondaryEffect,
		Faint,
		EndOfTurn,
		End,
	};

	enum class EMoveDamageState
	{
		None,
		Critical,
		TypeEffect,
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

	void Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy);

	void Reset() {};

protected:

private:
	// ��Ʋ ���� ������
	ABattleCanvas* Canvas = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	// ���
	const float BattleMsgShowTime = 1.5f;
	const float DamageTime = 1.5f;
	const float MoveEffectShowTime = 1.5f;
	const float FaintTime = 0.5f;

	// ���� ������
	ESubstate State = ESubstate::None;
	float Timer = 0.0f;
	bool IsFirstTurn = true;
	bool IsPlayerFirst = true;
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;
	UBattler* EOTBattler = nullptr;
	EMoveDamageState MoveResultMsg = EMoveDamageState::None;

	// SubState
	EMoveEffectState MoveEffectState = EMoveEffectState::None;
	EFaintState FaintState = EFaintState::None;
	
	// Temporal Data
	FDamageResult Result;
	int PrevHp = 0;
	int NextHp = 0;
	std::wstring MoveEffectMessage;

	// ����
	void Tick(float _DeltaTime) override;

	void DispatchTurn();
	void DispatchFight();
	void DispatchNextPhase();		// ���� ���ʰ� ����� ������, �� �������� ����
	void DispatchSecondaryEffect();
	void DispatchEndOfTurn();
	void DispatchFaintResult();				// ������ ����� �� �ִ��� ���� ����

	// ó�� �Լ�
	void ProcessEscapeFail(float _DeltaTime);
	void ProcessMoveFail(float _DeltaTime);
	void ProcessMoveAnim(float _DeltaTime);
	void ProcessMoveDamage(float _DeltaTime);
	void ProcessMoveBattleEffect(float _DeltaTime);
	void ProcessMoveSecondaryEffect(float _DeltaTime);
	void ProcessFaint(float _DeltaTime);
	void ProcessEndOfTurn(float _DeltaTime);

	// ��ƿ �Լ�
	void SetPlayerAsAttacker();
	void SetEnemyAsAttacker();
	void ChangeStatStage(EMoveEffectTarget _METarget, EStatStageChangeType _MEStatStageId, int _MEStatStageValue);
	void ChangeStatus(EMoveEffectTarget _METarget, EPokemonStatus _MEStatus);

	// ���� ��ȭ �Լ�
	void StateChangeToMoveAnim();
	void StateChangeToMoveDamage();
	void StateChangeToMoveBattleEffect();
	void StateChangeToMoveSecondaryEffect();
	void StateChangeToMoveFail(std::wstring _FailMessage);
};

