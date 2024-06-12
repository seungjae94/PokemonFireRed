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

	if (true == IsRegistered)
	{
		UEventManager::UnregisterEvent(this, Cond);
	}

	IsRegistered = true;

	Cond = UEventCondition(EEventTriggerAction::ArrowClick);
	Cond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventStream& Stream = ES::Start(true)
		>> ES::PlayAnimation(Global::PlayerCharacter, Global::PlayerCharacter + "Idle" + TargetDirection.ToDirectionString())
		>> ES::PlaySE(RN::SEExitMap);

	if (false == TargetBgm.empty())
	{
		Stream = Stream >> ES::FadeOutBgm(0.5f);
	}

	Stream = Stream
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PlayerCharacter, TargetPoint - TargetDirection)
		>> ES::ChangeDirection(TargetMapName, Global::PlayerCharacter, TargetDirection)
		>> ES::CameraFocus(Global::PlayerCharacter)
		>> ES::HideActor(Global::PlayerCharacter)
		>> ES::ShowMapName(TargetMapNameText)
		>> ES::FadeIn(0.75f, EFadeType::HCurtain)
		>> ES::Wait(0.25f);

	if (false == TargetBgm.empty())
	{
		Stream = Stream
			>> ES::PlayBgm(TargetBgm)
			>> ES::FadeInBgm(0.75f);
	}

	Stream = Stream
		>> ES::Wait(0.5f)
		>> ES::PlaySE(RN::SEDoorOpen)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorOpen")
		>> ES::ShowActor(Global::PlayerCharacter)
		>> ES::Move(Global::PlayerCharacter, { TargetDirection }, 1.8f)
		>> ES::PlayAnimation(ExteriorDoorName, "DoorClose")
		>> ES::End(true);

	UEventManager::RegisterEvent(this, Cond, Stream);
}