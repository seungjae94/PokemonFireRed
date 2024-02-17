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
		// >> ES::PlayAnimation(Global::PLAYER_NAME, Global::PLAYER_NAME + "Idle" + TargetDirection.ToDirectionString())
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PLAYER_NAME, TargetPoint - TargetDirection)
		>> ES::ChangeDirection(TargetMapName, Global::PLAYER_NAME, TargetDirection)
		// >> ES::HideActor()
		// >> ES::UIShowLevelNameAsync(TargetMapName)
		>> ES::FadeIn(0.5f) // >> ES::UIOpenCurtainAsync()
		>> ES::Wait(0.3f)
		// >> ES::PlayAnimation(ExteriorDoorName, ERenderTarget::LowerBody, "DoorOpen")
		// >> ES::ShowActor()
		>> ES::Move(TargetMapName, Global::PLAYER_NAME, { TargetDirection }, 1.8f)
		// >> ES::PlayAnimation(ExteriorDoorName, ERenderTarget::LowerBody, "DoorClose")
		>> ES::End(true)
	);
}