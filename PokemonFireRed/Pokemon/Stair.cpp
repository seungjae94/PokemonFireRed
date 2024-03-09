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

	UEventCondition StairCond = UEventCondition(EEventTriggerAction::ArrowClick);
	StairCond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	FVector TargetPosition = TargetPoint.ToFVector();
	for (FVector MoveDirection : SecondPath)
	{
		TargetPosition -= MoveDirection;
	}

	UEventManager::RegisterEvent(this, StairCond,
		ES::Start(true)
		>> ES::FadeOut(0.5f)
		>> ES::MoveWithoutRestriction(Global::Player, FirstPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePosition(TargetMapName, Global::Player, TargetPosition)
		>> ES::ChangeDirection(TargetMapName, Global::Player, TargetDirection.ToFVector())
		>> ES::CameraFocus(Global::Player)
		>> ES::FadeIn(0.5f)
		>> ES::MoveWithoutRestriction(Global::Player, SecondPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangePoint(TargetMapName, Global::Player, TargetPoint)
		>> ES::Wait(0.25f)
		>> ES::End(true)
	);
}
