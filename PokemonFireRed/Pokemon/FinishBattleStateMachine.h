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

	void Start(ABattleCanvas* _Canvas , APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy, EBattleEndReason _BattleEndReason);

protected:

private:
	ESubstate State = ESubstate::None;
	ABattleCanvas* Canvas = nullptr;
	APokemonMsgBox* MsgBox = nullptr;
	UBattler* Player = nullptr;
	UBattler* Enemy = nullptr;
	EBattleEndReason Reason = EBattleEndReason::None;
};

