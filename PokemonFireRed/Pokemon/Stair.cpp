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
		>> ES::PlaySE(RN::SEExitMap)
		>> ES::FadeOut(0.5f)
		>> ES::MoveWithoutRestriction(Global::PlayerCharacter, FirstPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePosition(TargetMapName, Global::PlayerCharacter, TargetPosition)
		>> ES::ChangeDirection(TargetMapName, Global::PlayerCharacter, TargetDirection.ToFVector())
		>> ES::CameraFocus(Global::PlayerCharacter)
		>> ES::FadeIn(0.5f)
		>> ES::MoveWithoutRestriction(Global::PlayerCharacter, SecondPath, 0.75f * Global::CharacterWalkSpeed)
		>> ES::ChangePoint(TargetMapName, Global::PlayerCharacter, TargetPoint)
		>> ES::Wait(0.25f)
		>> ES::End(true)
	);
}
