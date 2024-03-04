#pragma once
#include <EngineCore/Actor.h>
#include "Battler.h"
#include "BattleCanvas.h"
#include "AccuracyChecker.h"
#include "MoveEffectTester.h"
#include "MoveEffectApplier.h"
#include "DamageCalculator.h"

// ���� ������, ���� ��ȭ ������ ó���ϴ� SM
// ���ϸ��� �����Ѵٸ� ���� ���·� ������ �Ѵ�.
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

	// Move ���
	FDamageResult DamageResult;
	FMoveEffectTestResult BEResult;
	FMoveEffectTestResult SEResult;

	// Move �ִϸ��̼� ����
	const float MoveAnimationShowTime = 1.5f;

	// Move�� ���� ü�¹� ���� ����
	const float HpBarDecreaseTime = 1.5f;
	int PrevHp = 0;
	int NextHp = 0;

	// Move BE, Move SE ����
	EMoveEffectState MoveEffectState = EMoveEffectState::None;
	const float MoveEffectShowTime = 1.5f;

	// ���� ƽ
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

	// ��ƿ
	void StateChangeToMoveFail(std::wstring _FailMessage);
	void StateChangeToMoveBE();
	void StateChangeToMoveSE();
};

