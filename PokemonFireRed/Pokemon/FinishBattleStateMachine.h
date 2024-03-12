#pragma once
#include "BattleStateMachine.h"
#include "BattleTurnStateMachine.h"

class ABattleCanvas;
class APokemonMsgBox;
class UBattler;

class AFinishBattleStateMachine : public ABattleStateMachine
{
private:
	enum class ESubstate
	{
		None,
		OutOfPokemonMessage1,
		OutOfPokemonMessage2,
		PanicAndLost1,
		PanicAndLost2,
		ManyDots1,
		ManyDots2,
		WhitedOut1,
		WhitedOut2,
		PlayerLostAgainst1,
		PlayerLostAgainst2,
		PaidAsThePrizeMoney1,
		PaidAsThePrizeMoney2,
		PlayerDefeated1,
		PlayerDefeated2,
		EnemyBattlerMove,
		TestEnemyBattlerMessage,
		EnemyBattlerMessage1,
		EnemyBattlerMessage2,
		GotMoneyForWining1,
		GotMoneyForWining2,
		End
	};
public:
	// constructor destructor
	AFinishBattleStateMachine();
	~AFinishBattleStateMachine();

	// delete Function
	AFinishBattleStateMachine(const AFinishBattleStateMachine& _Other) = delete;
	AFinishBattleStateMachine(AFinishBattleStateMachine&& _Other) noexcept = delete;
	AFinishBattleStateMachine& operator=(const AFinishBattleStateMachine& _Other) = delete;
	AFinishBattleStateMachine& operator=(AFinishBattleStateMachine&& _Other) noexcept = delete;

	bool IsEnd() const
	{
		return State == ESubstate::End;
	}

	void Start(EBattleEndReason _BattleEndReason);

protected:

private:
	void Start() override {};
	EBattleEndReason Reason = EBattleEndReason::None;

	// 데이터
	ESubstate State = ESubstate::None;
	const float EnemyBattlerMoveTime = 0.5f;
	float Timer = 0.0f;

	// 틱 함수
	void Tick(float _DeltaTime) override;
	void ProcessOutOfPokemonMessage1();
	void ProcessOutOfPokemonMessage2();
	void ProcessPanicAndLost1();
	void ProcessPanicAndLost2();
	void ProcessManyDots1();
	void ProcessManyDots2();
	void ProcessWhitedOut1();
	void ProcessWhitedOut2();
	void ProcessPlayerLostAgainst1();
	void ProcessPlayerLostAgainst2();
	void ProcessPaidAsThePrizeMoney1();
	void ProcessPaidAsThePrizeMoney2();
	void ProcessPlayerDefeated1();
	void ProcessPlayerDefeated2();
	void ProcessEnemyBattlerMove();
	void ProcessTestEnemyBattlerMessage();
	void ProcessEnemyBattlerMessage1();
	void ProcessEnemyBattlerMessage2();
	void ProcessGotMoneyForWining1();
	void ProcessGotMoneyForWining2();

	// 유틸 함수
	int CalcMaxLevel();
	int CalcLostMoney();
	int CalcPrizeMoney();
};

