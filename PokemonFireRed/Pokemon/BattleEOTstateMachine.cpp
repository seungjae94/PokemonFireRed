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
	// �ӽ� ���°� ���� ���
	if (Target->CurTempStatusId() == EPokemonStatus::None)
	{
		State = ESubstate::TestStatus;
	}
	// �ӽ� ���°� �ִ� ���
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
		//NextHp = PrevHp - (Seeded/Bound ������)
	}
}

void ABattleEOTStateMachine::ProcessTempStatusDamage()
{
	// Lerp ������

	if (Timer <= 0.0f)
	{
		// ���� ������ ó�� (Faint ���� ó�� ����)

		// ���� ��ȯ
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
	// ���°� ���� ���
	if (Target->CurStatusId() == EPokemonStatus::None)
	{
		State = ESubstate::End;
	}
	// �ӽ� ���°� �ִ� ���
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
		// ���� ������ ó�� (Faint ���� ó�� ����)

		// ���� ��ȯ
		State = ESubstate::End;
	}
}
