#include "ExteriorDoor.h"

AExteriorDoor::AExteriorDoor()
{
}

AExteriorDoor::~AExteriorDoor()
{
}

void AExteriorDoor::RegisterPredefinedEvent()
{
	AEventTrigger::RegisterPredefinedEvent();

	UEventCondition Cond = UEventCondition(EEventTriggerAction::Notice);
	Cond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		// >> ES::PlayAnimation(GetName(), ERenderTarget::LowerBody, "DoorOpen")
		>> ES::Move(GetWorld()->GetName(), Global::PLAYER_NAME, { TargetDirection.ToFVector() }, 1.8f)
		// >> ES::HideActor(GetWorld()->GetName(), Global::PLAYER_NAME)
		// >> ES::PlayAnimation(ExteriorDoorName, ERenderTarget::LowerBody, "DoorClose")
		>> ES::FadeOut(0.5f)
		// >> ES::ShowActor(GetWorld()->GetName(), Global::PLAYER_NAME)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PLAYER_NAME, TargetPoint)
		>> ES::ChangeDirection(TargetMapName, Global::PLAYER_NAME, TargetDirection.ToFVector())
		>> ES::Wait(0.5f)
		>> ES::FadeIn(0.5f)
		>> ES::End(true)
	);
}
