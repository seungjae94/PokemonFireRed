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

	UEventCondition Cond = UEventCondition(EEventTriggerAction::ArrowClick);
	Cond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::PlayAnimation(Global::PLAYER_NAME, Global::PLAYER_NAME + "Idle" + TargetDirection.ToDirectionString())
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PLAYER_NAME, TargetPoint - TargetDirection)
		>> ES::ChangeDirection(TargetMapName, Global::PLAYER_NAME, TargetDirection)
		>> ES::CameraFocus(Global::PLAYER_NAME)
		>> ES::HideActor(Global::PLAYER_NAME)
		// >> ES:ShowMapNamePanel(TargetMapName)
		>> ES::FadeIn(0.5f) // >> ES::FadeIn(0.5f, EFadeType::Curtain)
		>> ES::Wait(0.5f)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorOpen")
		>> ES::ShowActor(Global::PLAYER_NAME)
		>> ES::Move(TargetMapName, Global::PLAYER_NAME, { TargetDirection }, 1.8f)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorClose")
		>> ES::End(true)
	);
}