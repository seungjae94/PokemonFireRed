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
		MsgBoxAssert("��Ʋ ������ �ƴ� ������ BattleStateMachine�� �����߽��ϴ�.");
		return;
	}

	Canvas = BattleLevel->Canvas;
	MsgBox = BattleLevel->MsgBox;
	AnimatorGenerator = BattleLevel->AnimatorGenerator;
	Player = &BattleLevel->Player;
	Enemy = &BattleLevel->Enemy;
}

