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
		>> ES::MoveWithoutRestriction(GetWorld()->GetName(), Global::PLAYER_NAME, FirstPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePosition(TargetMapName, Global::PLAYER_NAME, TargetPosition)
		>> ES::ChangeDirection(TargetMapName, Global::PLAYER_NAME, TargetDirection.ToFVector())
		>> ES::CameraFocus(Global::PLAYER_NAME)
		>> ES::FadeIn(0.5f)
		>> ES::MoveWithoutRestriction(TargetMapName, Global::PLAYER_NAME, SecondPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangePoint(TargetMapName, Global::PLAYER_NAME, TargetPoint)
		>> ES::Wait(0.25f)
		>> ES::End(true)
	);
}
