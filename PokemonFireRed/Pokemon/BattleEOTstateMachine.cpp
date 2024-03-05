#include "BattleEOTstateMachine.h"

ABattleEOTStateMachine::ABattleEOTStateMachine()
{
}

ABattleEOTStateMachine::~ABattleEOTStateMachine()
{
}

void ABattleEOTStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Target)
{
	Canvas = _Canvas;
	Target = _Target;

	State = ESubstate::TestTempStatus;
}

void ABattleEOTStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleEOTStateMachine::ESubstate::None:
		break;
	case ABattleEOTStateMachine::ESubstate::TestTempStatus:
		ProcessTestTempStatus();
		break;
	case ABattleEOTStateMachine::ESubstate::TempStatusAnim:
		ProcessTempStatusAnim();
		break;
	case ABattleEOTStateMachine::ESubstate::TempStatusDamage:
		ProcessTempStatusDamage();
		break;
	case ABattleEOTStateMachine::ESubstate::TempStatusMessage:
		ProcessTempStatusMessage();
		break;
	case ABattleEOTStateMachine::ESubstate::TestStatus:
		ProcessTestStatus();
		break;
	case ABattleEOTStateMachine::ESubstate::StatusMessage:
		ProcessStatusMessage();
		break;
	case ABattleEOTStateMachine::ESubstate::StatusAnim:
		ProcessStatusAnim();
		break;
	case ABattleEOTStateMachine::ESubstate::StatusDamage:
		ProcessStatusDamage();
		break;
	case ABattleEOTStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleEOTStateMachine::ProcessTestTempStatus()
{
	// 임시 상태가 없는 경우
	if (Target->CurTempStatusId() == EPokemonStatus::None)
	{
		State = ESubstate::TestStatus;
	}
	// 임시 상태가 있는 경우
	else
	{
		State = ESubstate::TempStatusAnim;
		Timer = TempStatusAnimTime;
	}
}

void ABattleEOTStateMachine::ProcessTempStatusAnim()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::TempStatusDamage;
		Timer = HpBarDecreaseTime;

		//PrevHp = Target->CurPokemon()->GetCurHp();
		//NextHp = PrevHp - (Seeded/Bound 데미지)
	}
}

void ABattleEOTStateMachine::ProcessTempStatusDamage()
{
	// Lerp 데미지

	if (Timer <= 0.0f)
	{
		// 실제 데미지 처리 (Faint 상태 처리 포함)

		// 상태 전환
		State = ESubstate::TempStatusMessage;
		Timer = BattleMsgShowTime;
		Canvas->SetBattleMessage(L"Temp Status Effect!");
	}
}

void ABattleEOTStateMachine::ProcessTempStatusMessage()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::TestStatus;
	}
}

void ABattleEOTStateMachine::ProcessTestStatus()
{
	// 상태가 없는 경우
	if (Target->CurStatusId() == EPokemonStatus::None)
	{
		State = ESubstate::End;
	}
	// 임시 상태가 있는 경우
	else
	{
		State = ESubstate::StatusMessage;
		Timer = BattleMsgShowTime;
		Canvas->SetBattleMessage(L"Status Effect!");
	}
}

void ABattleEOTStateMachine::ProcessStatusMessage()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::StatusAnim;
		Timer = StatusAnimTime;
	}
}

void ABattleEOTStateMachine::ProcessStatusAnim()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::StatusDamage;
		Timer = HpBarDecreaseTime;
	}
}

void ABattleEOTStateMachine::ProcessStatusDamage()
{
	// Lerp Hp bar

	if (Timer <= 0.0f)
	{
		// 실제 데미지 처리 (Faint 상태 처리 포함)

		// 상태 전환
		State = ESubstate::End;
	}
}
