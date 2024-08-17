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

	LowerBodyRenderer->SetOrder(ERenderingOrder::BelowCharacter);
	LowerBodyRenderer->CreateAnimation("DoorOpen", ImageName, { 0, 1, 2, 3 }, 0.1f, false);
	LowerBodyRenderer->CreateAnimation("DoorClose", ImageName, { 3, 2, 1, 0 }, 0.1f, false);

	UEventStream& Stream = ES::Start(true)
		>> ES::Move(Global::PlayerCharacter, { FTileVector::Zero }, 7.2f) // 제자리 걷기 동작으로 문을 여는 동작을 표현
		>> ES::PlaySE(RN::SEDoorOpen)
		>> ES::PlayAnimation(GetName(), "DoorOpen")
		>> ES::Move(Global::PlayerCharacter, { TargetDirection })
		>> ES::HideActor(Global::PlayerCharacter)
		>> ES::PlayAnimation(GetName(), "DoorClose")
		>> ES::FadeOut(0.75f);

	if (false == TargetBgm.empty())
	{
		Stream = Stream >> ES::FadeOutBgm(0.75f);
	}

	Stream = Stream
		>> ES::Wait(0.75f)
		>> ES::ShowActor(Global::PlayerCharacter)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PlayerCharacter, TargetPoint)
		>> ES::ChangeDirection(TargetMapName, Global::PlayerCharacter, TargetDirection)
		>> ES::CameraFocus(Global::PlayerCharacter);

	if (false == TargetBgm.empty())
	{
		Stream = Stream 
			>> ES::PlayBgm(TargetBgm)
			>> ES::FadeInBgm(0.75f);
	}
	
	Stream = Stream
		>> ES::FadeIn(0.75f)
		>> ES::Wait(0.75f)
		>> ES::End(true);

	RegisterEvent(
		EEventTriggerAction::ArrowClick, 
		std::bind(&AWarp::CheckPlayerDirection, this),
		Stream);
}
