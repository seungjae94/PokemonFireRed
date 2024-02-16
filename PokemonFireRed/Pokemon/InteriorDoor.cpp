#include "InteriorDoor.h"

AInteriorDoor::AInteriorDoor()
{
}

AInteriorDoor::~AInteriorDoor()
{
}

void AInteriorDoor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition Cond = UEventCondition(EEventTriggerAction::Notice);
	Cond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::FadeOut(0.5f)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PLAYER_NAME, TargetPoint)
		>> ES::ChangeDirection(TargetMapName, Global::PLAYER_NAME, TargetDirection.ToFVector())
		// >> ES::HideActor()
		// >> ES::UIShowLevelNameAsync(TargetMapName)
		>> ES::FadeIn(0.5f) // >> ES::UIOpenCurtainAsync()
		// >> ES::PlayAnimation(ExteriorDoorName, ERenderTarget::LowerBody, "DoorOpen")
		// >> ES::ShowActor()
		>> ES::Move(GetWorld()->GetName(), Global::PLAYER_NAME, { TargetDirection.ToFVector() }, 1.8f)
		// >> ES::PlayAnimation(ExteriorDoorName, ERenderTarget::LowerBody, "DoorClose")
		>> ES::End(true)
	);
}