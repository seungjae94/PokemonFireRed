#include "BattleEOTStateMachine.h"
#include "BattleUtil.h"

ABattleEOTStateMachine::ABattleEOTStateMachine()
{
}

ABattleEOTStateMachine::~ABattleEOTStateMachine()
{
}

void ABattleEOTStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Target, UBattler* _CounterTarget)
{
	Canvas = _Canvas;
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
	if (Target->CurTempStatusId() == EPokemonStatus::Normal)
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
			// 실제 데미지 처리
			Target->CurPokemon()->SetCurHp(TargetNextHp);
			CounterTarget->CurPokemon()->SetCurHp(CounterTargetNextHp);
			if (TargetNextHp == 0)
			{
				Target->CurPokemon()->SetStatus(EPokemonStatus::Faint);
			}

			State = ESubstate::TempStatusMessage;
			Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Target) + L"'s health is\nsapped by LEECH SEED!");
			Timer = BattleMsgShowTime;
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
		// 실제 데미지 처리 (Faint 상태 처리 포함)
		Target->CurPokemon()->SetCurHp(TargetNextHp);
		Target->DecBindCount();
		if (TargetNextHp == 0)
		{
			Target->CurPokemon()->SetStatus(EPokemonStatus::Faint);
		}

		// State 전환
		State = ESubstate::TempStatusMessage;
		Canvas->SetBattleMessage(UBattleUtil::GetPokemonFullName(Target) + L" is hurt\nby BIND!");
		Timer = BattleMsgShowTime;
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
	// 타겟이 기절 상태인 경우 스킵
	if (Target->CurStatusId() == EPokemonStatus::Faint)
	{
		State = ESubstate::End;
		return;
	}

	// 상태가 없는 경우 종료
	if (Target->CurStatusId() == EPokemonStatus::Normal)
	{
		State = ESubstate::End;
	}
	// 상태가 있는 경우 진행
	else
	{
		State = ESubstate::StatusMessage;
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

		Canvas->SetBattleMessage(BattleMsg);
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
		// 실제 데미지 처리
		Target->CurPokemon()->SetCurHp(TargetNextHp);
		Target->DecBindCount();
		if (TargetNextHp == 0)
		{
			Target->CurPokemon()->SetStatus(EPokemonStatus::Faint);
		}

		// 상태 전환
		State = ESubstate::End;
	}
}
