#include "Stair.h"

AStair::AStair()
{
}

AStair::~AStair()
{
}

void AStair::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition StairCond = UEventCondition(EEventTriggerAction::Notice);
	StairCond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, StairCond,
		ES::Start(true)
		>> ES::MoveWithoutRestriction(GetWorld()->GetName(), Global::PLAYER_NAME, FirstPath, 6.0f)
		>> ES::MoveWithoutRestriction(GetWorld()->GetName(), Global::PLAYER_NAME, SecondPath, 2.2f)
		>> ES::FadeOut(0.5f)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PLAYER_NAME, TargetPoint)
		>> ES::ChangeDirection(TargetMapName, Global::PLAYER_NAME, TargetDirection.ToFVector())
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);
}
