#pragma once
#include <EngineCore/Actor.h>
#include "BattleTurnStateMachine.h"

class ABattleCanvas;
class APokemonMsgBox;
class UBattler;

class AFinishBattleStateMachine : public AActor
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
		EnemyBattlerMessage,
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

	void Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy, EBattleEndReason _BattleEndReason);

protected:

private:
	ESubstate State = ESubstate::None;
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;
	EBattleEndReason Reason = EBattleEndReason::None;

	// Æ½ ÇÔ¼ö
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
	void ProcessEnemyBattlerMessage();
	void ProcessGotMoneyForWining1();
	void ProcessGotMoneyForWining2();
};

