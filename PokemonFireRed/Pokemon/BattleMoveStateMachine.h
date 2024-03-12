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

// 직접 데미지, 상태 변화 로직을 처리하는 SM
// 포켓몬이 기절한다면 기절 상태로 만들어야 한다.
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

	// Move 결과
	FDamageResult DamageResult;
	FMoveEffectTestResult BEResult;
	FMoveEffectTestResult SEResult;

	// Move 애니메이션 연출
	AAnimator* Animator = nullptr;
	const float MoveAnimationShowTime = 1.5f;

	// Move로 인한 체력바 감소 연출
	const float HpBarDecreaseBaseTime = 0.5f;
	const float HpBarDecreaseTimeVariance = 1.5f;
	float HpBarDecreaseTime = 0.0f;
	int PrevHp = 0;
	int NextHp = 0;

	// Move BE, Move SE 연출
	EMoveEffectState MoveEffectState = EMoveEffectState::None;
	const float MoveEffectShowTime = 1.5f;

	// 상태 틱
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

	// 유틸
	void StateChangeToMoveFail(std::wstring _FailMessage);
	void StateChangeToMoveBE();
	void StateChangeToMoveSE();
};

