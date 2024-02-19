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
		>> ES::PlayAnimation(Global::PlayerName, Global::PlayerName + "Idle" + TargetDirection.ToDirectionString())
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PlayerName, TargetPoint - TargetDirection)
		>> ES::ChangeDirection(TargetMapName, Global::PlayerName, TargetDirection)
		>> ES::CameraFocus(Global::PlayerName)
		>> ES::HideActor(Global::PlayerName)
		>> ES::HideUI("BlackScreen")
		>> ES::HideUI("MapNameWindow")
		>> ES::ShowMapName(TargetMapNameText)
		>> ES::FadeIn(0.75f, EFadeType::Curtain)
		>> ES::Wait(0.75f)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorOpen")
		>> ES::ShowActor(Global::PlayerName)
		>> ES::Move(TargetMapName, Global::PlayerName, { TargetDirection }, 1.8f)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorClose")
		>> ES::End(true)
	);
}