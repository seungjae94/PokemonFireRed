#include "BattleEOTstateMachine.h"

ABattleEOTStateMachine::ABattleEOTStateMachine()
{
}

ABattleEOTStateMachine::~ABattleEOTStateMachine()
{
}

void ABattleEOTStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Target)
{
	Canvas = _Canvas;
	Target = _Target;

	State = ESubstate::End;
}
