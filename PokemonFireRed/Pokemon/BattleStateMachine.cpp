#include "BattleStateMachine.h"
#include "BattleLevel.h"

ABattleStateMachine::ABattleStateMachine() 
{
}

ABattleStateMachine::~ABattleStateMachine() 
{
}

void ABattleStateMachine::Start()
{
	UBattleLevel* BattleLevel = dynamic_cast<UBattleLevel*>(GetWorld());

	if (nullptr == BattleLevel)
	{
		MsgBoxAssert("배틀 레벨이 아닌 곳에서 BattleStateMachine을 생성했습니다.");
		return;
	}

	Canvas = BattleLevel->Canvas;
	MsgBox = BattleLevel->MsgBox;
	AnimatorGenerator = BattleLevel->AnimatorGenerator;
	Player = &BattleLevel->Player;
	Enemy = &BattleLevel->Enemy;
}

