#include "FinishBattleStateMachine.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "Battler.h"

AFinishBattleStateMachine::AFinishBattleStateMachine() 
{
}

AFinishBattleStateMachine::~AFinishBattleStateMachine() 
{
}

void AFinishBattleStateMachine::Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Player, UBattler* _Enemy, EBattleEndReason _BattleEndReason)
{
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	Player = _Player;
	Enemy = _Enemy;
	Reason = _BattleEndReason;


}

