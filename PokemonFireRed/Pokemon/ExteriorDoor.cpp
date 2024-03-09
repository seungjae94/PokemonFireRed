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

	UEventCondition Cond = UEventCondition(EEventTriggerAction::ArrowClick);
	Cond.RegisterCheckFunc(ToCheckFunc(CheckPlayerDirection));

	UEventManager::RegisterEvent(this, Cond,
		ES::Start(true)
		>> ES::Move(Global::Player, { FTileVector::Zero }, 7.2f) // 제자리 걷기 동작으로 문을 여는 동작을 표현
		>> ES::PlayAnimation(GetName(), "DoorOpen")
		>> ES::Move(Global::Player, { TargetDirection })
		>> ES::HideActor(Global::Player)
		>> ES::PlayAnimation(GetName(), "DoorClose")
		>> ES::FadeOut(0.75f)
		>> ES::Wait(0.75f)
		>> ES::ShowActor(Global::Player)
		>> ES::ChangeLevel(TargetMapName)
		>> ES::ChangePoint(TargetMapName, Global::Player, TargetPoint)
		>> ES::ChangeDirection(TargetMapName, Global::Player, TargetDirection)
		>> ES::CameraFocus(Global::Player)
		>> ES::FadeIn(0.75f)
		>> ES::Wait(0.75f)
		>> ES::End(true)
	);
}
