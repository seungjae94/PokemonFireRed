#include "BattleEOTStateMachine.h"
#include "BattleUtil.h"
#include "PokemonMsgBox.h"
#include "BattleCanvas.h"

ABattleEOTStateMachine::ABattleEOTStateMachine()
{
}

ABattleEOTStateMachine::~ABattleEOTStateMachine()
{
}

void ABattleEOTStateMachine::Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Target, UBattler* _CounterTarget)
{
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	Target = _Target;
	CounterTarget = _CounterTarget;

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
	case ABattleEOTStateMachine::ESubstate::LeechSeed:
		ProcessLeechSeed();
		break;
	case ABattleEOTStateMachine::ESubstate::Bind:
		ProcessBind();
		break;
	case ABattleEOTStateMachine::ESubstate::TempStatusMessage1:
		ProcessTempStatusMessage1();
		break;
	case ABattleEOTStateMachine::ESubstate::TempStatusMessage2:
		ProcessTempStatusMessage2();
		break;
	case ABattleEOTStateMachine::ESubstate::TestStatus:
		ProcessTestStatus();
		break;
	case ABattleEOTStateMachine::ESubstate::StatusMessage1:
		ProcessStatusMessage1();
		break;
	case ABattleEOTStateMachine::ESubstate::StatusMessage2:
		ProcessStatusMessage2();
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
	if (Target->CurTempStatusId() == EPokemonStatus::Normal)
	{
		State = ESubstate::TestStatus;
	}
	// �ӽ� ���°� �ִ� ���
	else
	{
		if (Target->CurTempStatusId() == EPokemonStatus::TempSeeded
			&& CounterTarget->CurStatusId() == EPokemonStatus::Faint)
		{
			// EOT State Machine���� ���� �̻� Target�� ����ִٴ� �� ������ �ȴ�.
			// ������, CounterTarget�� �׾� ���� ���� �ִ�. 
			// CounterTarget�� �׾� �ִ� ��� Seeded ȿ���� ����Ǿ�� �ȵȴ�.
			State = ESubstate::TestStatus;
			return;
		}

		State = ESubstate::TempStatusAnim;
		Timer = TempStatusAnimTime;
	}
}

void ABattleEOTStateMachine::ProcessTempStatusAnim()
{
	if (Timer <= 0.0f)
	{
		if (Target->CurTempStatusId() == EPokemonStatus::TempSeeded)
		{
			State = ESubstate::LeechSeed;
			LeechSeedState = ELeechSeedState::Damage;
		}
		else if (Target->CurTempStatusId() == EPokemonStatus::TempBound)
		{
			State = ESubstate::Bind;
		}

		Timer = HpBarDecreaseTime;
	}
}

void ABattleEOTStateMachine::ProcessLeechSeed()
{
	int TargetPrevHp = Target->CurPokemonReadonly()->GetCurHp();
	int TargetMaxHp = Target->CurPokemonReadonly()->GetHp();
	int TargetNextHp = UPokemonMath::Max(TargetPrevHp - TargetMaxHp / 8, 0);
	int Damage = TargetPrevHp - TargetNextHp;
	int CounterTargetPrevHp = CounterTarget->CurPokemonReadonly()->GetCurHp();
	int CounterTargetMaxHp = CounterTarget->CurPokemonReadonly()->GetHp();
	int CounterTargetNextHp = UPokemonMath::Min(CounterTargetPrevHp + Damage, CounterTargetMaxHp);

	if (LeechSeedState == ELeechSeedState::Damage)
	{
		if (true == Target->IsPlayer())
		{
			Canvas->LerpPlayerHpInfo(TargetPrevHp, TargetNextHp, TargetMaxHp, Timer / HpBarDecreaseTime);
		}
		else
		{
			Canvas->LerpEnemyHpInfo(TargetPrevHp, TargetNextHp, TargetMaxHp, Timer / HpBarDecreaseTime);
		}

		if (Timer <= 0.0f)
		{
			LeechSeedState = ELeechSeedState::Heal;
			Timer = HpBarDecreaseTime;
		}
	}
	else
	{
		if (true == Target->IsPlayer())
		{
			Canvas->LerpEnemyHpInfo(CounterTargetPrevHp, CounterTargetNextHp, CounterTargetMaxHp, Timer / HpBarDecreaseTime);
		}
		else
		{
			Canvas->LerpPlayerHpInfo(CounterTargetPrevHp, CounterTargetNextHp, CounterTargetMaxHp, Timer / HpBarDecreaseTime);
		}

		if (Timer <= 0.0f)
		{
			// ���� ������ ó��
			Target->CurPokemon()->SetCurHp(TargetNextHp);
			CounterTarget->CurPokemon()->SetCurHp(CounterTargetNextHp);
			if (TargetNextHp == 0)
			{
				Target->CurPokemon()->SetStatus(EPokemonStatus::Faint);
			}

			State = ESubstate::TempStatusMessage1;
			MsgBox->SetMessage(UBattleUtil::GetPokemonFullName(Target) + L"'s health is\nsapped by LEECH SEED!");
			MsgBox->Write();
		}
	}
}

void ABattleEOTStateMachine::ProcessBind()
{
	int TargetPrevHp = Target->CurPokemonReadonly()->GetCurHp();
	int TargetMaxHp = Target->CurPokemonReadonly()->GetHp();
	int TargetNextHp = UPokemonMath::Max(TargetPrevHp - TargetMaxHp / 16, 0);

	if (true == Target->IsPlayer())
	{
		Canvas->LerpPlayerHpInfo(TargetPrevHp, TargetNextHp, TargetMaxHp, Timer / HpBarDecreaseTime);
	}
	else
	{
		Canvas->LerpEnemyHpInfo(TargetPrevHp, TargetNextHp, TargetMaxHp, Timer / HpBarDecreaseTime);
	}

	if (Timer <= 0.0f)
	{
		// ���� ������ ó�� (Faint ���� ó�� ����)
		Target->CurPokemon()->SetCurHp(TargetNextHp);
		Target->DecBindCount();
		if (TargetNextHp == 0)
		{
			Target->CurPokemon()->SetStatus(EPokemonStatus::Faint);
		}

		// State ��ȯ
		State = ESubstate::TempStatusMessage1;
		MsgBox->SetMessage(UBattleUtil::GetPokemonFullName(Target) + L" is hurt\nby BIND!");
		MsgBox->Write();
	}
}


void ABattleEOTStateMachine::ProcessTempStatusMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::TempStatusMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleEOTStateMachine::ProcessTempStatusMessage2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::TestStatus;
	}
}

void ABattleEOTStateMachine::ProcessTestStatus()
{
	// Ÿ���� ���� ������ ��� ��ŵ
	if (Target->CurStatusId() == EPokemonStatus::Faint)
	{
		State = ESubstate::End;
		return;
	}

	// ���°� ���� ��� ����
	if (Target->CurStatusId() == EPokemonStatus::Normal)
	{
		State = ESubstate::End;
	}
	// ���°� �ִ� ��� ����
	else
	{
		State = ESubstate::StatusMessage1;
		Timer = BattleMsgShowTime;

		std::wstring BattleMsg = UBattleUtil::GetPokemonFullName(Target) + L" is hurt\nby ";
		switch (Target->CurStatusId())
		{
		case EPokemonStatus::Burn:
			BattleMsg += L"its burn!";
			break;
		case EPokemonStatus::Poison:
			BattleMsg += L"poison!";
			break;
		default:
			break;
		}

		MsgBox->SetMessage(BattleMsg);
	}
}

void ABattleEOTStateMachine::ProcessStatusMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::StatusMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleEOTStateMachine::ProcessStatusMessage2()
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
	int TargetPrevHp = Target->CurPokemonReadonly()->GetCurHp();
	int TargetMaxHp = Target->CurPokemonReadonly()->GetHp();
	int TargetNextHp = UPokemonMath::Max(TargetPrevHp - TargetMaxHp / 8, 0);

	if (true == Target->IsPlayer())
	{
		Canvas->LerpPlayerHpInfo(TargetPrevHp, TargetNextHp, TargetMaxHp, Timer / HpBarDecreaseTime);
	}
	else
	{
		Canvas->LerpEnemyHpInfo(TargetPrevHp, TargetNextHp, TargetMaxHp, Timer / HpBarDecreaseTime);
	}

	if (Timer <= 0.0f)
	{
		// ���� ������ ó��
		Target->CurPokemon()->SetCurHp(TargetNextHp);
		Target->DecBindCount();
		if (TargetNextHp == 0)
		{
			Target->CurPokemon()->SetStatus(EPokemonStatus::Faint);
		}

		// ���� ��ȯ
		State = ESubstate::End;
	}
}
