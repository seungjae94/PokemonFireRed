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
		>> ES::MoveWithoutRestriction(GetWorld()->GetName(), Global::PlayerName, FirstPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePosition(TargetMapName, Global::PlayerName, TargetPosition)
		>> ES::ChangeDirection(TargetMapName, Global::PlayerName, TargetDirection.ToFVector())
		>> ES::CameraFocus(Global::PlayerName)
		>> ES::FadeIn(0.5f)
		>> ES::MoveWithoutRestriction(TargetMapName, Global::PlayerName, SecondPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangePoint(TargetMapName, Global::PlayerName, TargetPoint)
		>> ES::Wait(0.25f)
		>> ES::End(true)
	);
}
