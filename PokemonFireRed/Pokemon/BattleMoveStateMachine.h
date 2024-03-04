#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"
#include "BattleCanvas.h"
#include "AccuracyChecker.h"
#include "MoveEffectTester.h"
#include "MoveEffectApplier.h"
#include "DamageCalculator.h"

// 직접 데미지, 상태 변화 로직을 처리하는 SM
// 포켓몬이 기절한다면 기절 상태로 만들어야 한다.
class ABattleMoveStateMachine : public AActor
{
private:
	enum class ESubstate
	{
		None,
		MoveFail,
		MoveAnim,
		MoveDamage,
		MoveCriticalMessage,
		MoveEffectiveMessage,
		MoveBE,
		MoveBEMessage,
		TestSE,
		MoveSE,
		MoveSEMessage,
		End,
	};

	enum class EMoveDamageState
	{
		None,
		HpBarDecrease,
		Critical,
		TypeEffect,
	};

	enum class EMoveEffectState
	{
		None,
		MoveEffect,
		Message,
	};
public:
	// constructor destructor
	ABattleMoveStateMachine();
	~ABattleMoveStateMachine();

	// delete Function
	ABattleMoveStateMachine(const ABattleMoveStateMachine& _Other) = delete;
	ABattleMoveStateMachine(ABattleMoveStateMachine&& _Other) noexcept = delete;
	ABattleMoveStateMachine& operator=(const ABattleMoveStateMachine& _Other) = delete;
	ABattleMoveStateMachine& operator=(ABattleMoveStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(ABattleCanvas* _Canvas, UBattler* _Attacker, UBattler* _Defender);

protected:
	void Tick(float _DeltaTime) override;
private:
	ABattleCanvas* Canvas = nullptr;
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;

	ESubstate State = ESubstate::None;
	float Timer = 0.0f;
	const float BattleMsgShowTime = 1.5f;

	// Move 결과
	FDamageResult DamageResult;
	FMoveEffectTestResult BEResult;
	FMoveEffectTestResult SEResult;

	// Move 애니메이션 연출
	const float MoveAnimationShowTime = 1.5f;

	// Move로 인한 체력바 감소 연출
	const float HpBarDecreaseTime = 1.5f;
	int PrevHp = 0;
	int NextHp = 0;

	// Move BE, Move SE 연출
	EMoveEffectState MoveEffectState = EMoveEffectState::None;
	const float MoveEffectShowTime = 1.5f;

	// 상태 틱
	void ProcessMoveFail();
	void ProcessMoveAnim();
	void ProcessMoveDamage();
	void ProcessMoveCriticalMessage();
	void ProcessMoveEffectiveMessage();
	void ProcessMoveBE();
	void ProcessMoveBEMessage();
	void ProcessTestSE();
	void ProcessMoveSE();
	void ProcessMoveSEMessage();

	// 유틸
	void StateChangeToMoveFail(std::wstring _FailMessage);
	void StateChangeToMoveBE();
	void StateChangeToMoveSE();
};

