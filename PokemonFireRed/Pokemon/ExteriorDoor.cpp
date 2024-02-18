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

	UEventCondition Cond = UEventCondition(EEventTriggerAction::Notice);
	Cond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::Move(GetWorld()->GetName(), Global::PLAYER_NAME, { FTileVector::Zero }, 7.2f) // 제자리 걷기 동작으로 문을 여는 동작을 표현
		>> ES::PlayAnimation(GetName(), "DoorOpen")
		>> ES::Move(GetWorld()->GetName(), Global::PLAYER_NAME, { TargetDirection })
		>> ES::HideActor(Global::PLAYER_NAME)
		>> ES::PlayAnimation(GetName(), "DoorClose")
		>> ES::FadeOut(0.5f)
		>> ES::Wait(0.5f)
		>> ES::ShowActor(Global::PLAYER_NAME)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::PLAYER_NAME, TargetPoint)
		>> ES::ChangeDirection(TargetMapName, Global::PLAYER_NAME, TargetDirection)
		>> ES::CameraFocus(Global::PLAYER_NAME)
		>> ES::FadeIn(0.5f)
		>> ES::Wait(0.5f)
		>> ES::End(true)
	);
}
