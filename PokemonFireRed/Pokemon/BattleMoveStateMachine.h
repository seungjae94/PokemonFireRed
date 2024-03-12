#pragma once
#include "BattleStateMachine.h"
#include "Battler.h"
#include "AccuracyChecker.h"
#include "MoveEffectTester.h"
#include "MoveEffectApplier.h"
#include "DamageCalculator.h"

class AAnimator;
class ABattleCanvas;
class APokemonMsgBox;

// ���� ������, ���� ��ȭ ������ ó���ϴ� SM
// ���ϸ��� �����Ѵٸ� ���� ���·� ������ �Ѵ�.
class ABattleMoveStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		MoveFail1,
		MoveFail2,
		MoveUseMessage1,
		MoveUseMessage2,
		MoveAnim,
		MoveDamage,
		MoveCriticalMessage1,
		MoveCriticalMessage2,
		MoveEffectiveMessage1,
		MoveEffectiveMessage2,
		MoveBEStart,
		MoveBEAnim,
		MoveBEMessage1,
		MoveBEMessage2,
		TestSE,
		MoveSEStart,
		MoveSEAnim,
		MoveSEMessage1,
		MoveSEMessage2,
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

	void Start(UBattler* _Attacker, UBattler* _Defender);

protected:
	void Tick(float _DeltaTime) override;
private:
	void Start() override {};
	UBattler* Attacker = nullptr;
	UBattler* Defender = nullptr;

	ESubstate State = ESubstate::None;
	float Timer = 0.0f;
	const float BattleMsgShowTime = 0.75f;

	// Move ���
	FDamageResult DamageResult;
	FMoveEffectTestResult BEResult;
	FMoveEffectTestResult SEResult;

	// Move �ִϸ��̼� ����
	AAnimator* Animator = nullptr;
	const float MoveAnimationShowTime = 1.5f;

	// Move�� ���� ü�¹� ���� ����
	const float HpBarDecreaseBaseTime = 0.5f;
	const float HpBarDecreaseTimeVariance = 1.5f;
	float HpBarDecreaseTime = 0.0f;
	int PrevHp = 0;
	int NextHp = 0;

	// Move BE, Move SE ����
	EMoveEffectState MoveEffectState = EMoveEffectState::None;
	const float MoveEffectShowTime = 1.5f;

	// ���� ƽ
	void ProcessMoveFail1();
	void ProcessMoveFail2();
	void ProcessMoveUseMessage1();
	void ProcessMoveUseMessage2();
	void ProcessMoveAnim();
	void ProcessMoveDamage();
	void ProcessMoveCriticalMessage1();
	void ProcessMoveCriticalMessage2();
	void ProcessMoveEffectiveMessage1();
	void ProcessMoveEffectiveMessage2();
	void ProcessMoveBEStart();
	void ProcessMoveBEAnim();
	void ProcessMoveBEMessage1();
	void ProcessMoveBEMessage2();
	void ProcessTestSE();
	void ProcessMoveSEStart();
	void ProcessMoveSEAnim();
	void ProcessMoveSEMessage1();
	void ProcessMoveSEMessage2();

	// ��ƿ
	void StateChangeToMoveFail(std::wstring _FailMessage);
	void StateChangeToMoveBE();
	void StateChangeToMoveSE();
};

