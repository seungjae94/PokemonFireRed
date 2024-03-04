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
	// 배틀 레벨 데이터
	ABattleCanvas* Canvas = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;

	// 상수
	const float BattleMsgShowTime = 2.0f;
	const float DamageTime = 2.0f;
	const float MoveEffectShowTime = 2.0f;

	// 고유 데이터
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

	// 로직
	void Tick(float _DeltaTime) override;

	void DispatchTurn();
	void DispatchFight();
	void DispatchNextPhase();		// 다음 차례가 상대의 턴인지, 턴 종료인지 결정
	void DispatchSecondaryEffect();
	void DispatchEndOfTurn();
	void DispatchFaint();				// 전투를 계속할 수 있는지 등을 결정

	// 처리 함수
	void ProcessEscapeFail(float _DeltaTime);
	void ProcessMoveFail(float _DeltaTime);
	void ProcessMoveAnim(float _DeltaTime);
	void ProcessMoveDamage(float _DeltaTime);
	void ProcessMoveBattleEffect(float _DeltaTime);
	void ProcessMoveSecondaryEffect(float _DeltaTime);
	void ProcessFaint(float _DeltaTime);
	void ProcessEndOfTurn(float _DeltaTime);

	// 유틸 함수
	void SetPlayerAsAttacker();
	void SetEnemyAsAttacker();
	void ChangeStatStage(EMoveEffectTarget _METarget, EStatStageChangeType _MEStatStageId, int _MEStatStageValue);
	void ChangeStatus();
	std::wstring GetStatStageMessageSuffix(int _Value);
};

