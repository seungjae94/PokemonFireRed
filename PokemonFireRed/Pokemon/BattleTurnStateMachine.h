#pragma once
#include <EngineCore/Actor.h>
#include "BattleEnums.h"
#include "StatStage.h"
#include "Pokemon.h"
#include "Battler.h"

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
		MoveBattleEffect,
		MoveSecondaryEffect,
		Faint,
		EndOfTurn,
		End,
	};

	enum class EMoveResultMsg
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
	const float BattleMsgShowTime = 2.0f;
	const float DamageTime = 2.0f;
	const float MoveEffectShowTime = 2.0f;

	// ���� ������
	ESubstate State = ESubstate::None;
	float Timer = 0.0f;
	bool IsFirstTurn = true;
	bool IsPlayerFirst = true;
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;
	EMoveResultMsg MoveResultMsg = EMoveResultMsg::None;

	// SubState
	EMoveEffectState MoveEffectState = EMoveEffectState::None;
	
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
	void DispatchFaint();				// ������ ����� �� �ִ��� ���� ����

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
	void ChangeStatus();
	std::wstring GetStatStageMessageSuffix(int _Value);
};

