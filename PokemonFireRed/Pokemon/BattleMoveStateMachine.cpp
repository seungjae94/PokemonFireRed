#include "BattleMoveStateMachine.h"
#include "BattleUtil.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"

ABattleMoveStateMachine::ABattleMoveStateMachine()
{
}

ABattleMoveStateMachine::~ABattleMoveStateMachine()
{
}

void ABattleMoveStateMachine::Start(ABattleCanvas* _Canvas, APokemonMsgBox* _MsgBox, UBattler* _Attacker, UBattler* _Defender)
{
	Canvas = _Canvas;
	MsgBox = _MsgBox;
	Attacker = _Attacker;
	Defender = _Defender;

	const FPokemonMove* Move = Attacker->CurMove();

	bool AccuracyCheckResult = UAccuracyChecker::Check(Attacker, Defender);

	// 정확도 체크 단계에서 실패할 경우 MoveFail 상태로 보낸다.
	if (false == AccuracyCheckResult)
	{
		std::wstring BattleMsg = UBattleUtil::GetPokemonFullName(Attacker) + L"'s\nattack failed!";
		StateChangeToMoveFail(BattleMsg);
		return;
	}

	if (Move->BETarget != EMoveEffectTarget::None)
	{
		BEResult = UMoveEffectTester::TestBE(Attacker, Defender);

		// BE 사용에 실패한 경우 MoveFail 상태로 보낸다.
		if (false == BEResult.Success)
		{
			StateChangeToMoveFail(BEResult.Message);
			return;
		}
	}

	// 기술이 성공할 경우 Move 애니메이션을 재생한다.
	State = ESubstate::MoveAnim;
	MsgBox->SetMessage(UBattleUtil::GetPokemonFullName(Attacker) + L" used\n" + Move->GetNameW() + L"!");
	MsgBox->Write();
	Timer = MoveAnimationShowTime;
}

void ABattleMoveStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleMoveStateMachine::ESubstate::None:
		break;
	case ABattleMoveStateMachine::ESubstate::MoveFail:
		ProcessMoveFail();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveAnim:
		ProcessMoveAnim();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveDamage:
		ProcessMoveDamage();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveCriticalMessage:
		ProcessMoveCriticalMessage();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveEffectiveMessage:
		ProcessMoveEffectiveMessage();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveBE:
		ProcessMoveBE();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveBEMessage:
		ProcessMoveBEMessage();
		break;
	case ABattleMoveStateMachine::ESubstate::TestSE:
		ProcessTestSE();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveSE:
		ProcessMoveSE();
		break;
	case ABattleMoveStateMachine::ESubstate::MoveSEMessage:
		ProcessMoveSEMessage();
		break;
	case ABattleMoveStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleMoveStateMachine::ProcessMoveFail()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::End;
	}
}

void ABattleMoveStateMachine::ProcessMoveAnim()
{
	// Move 애니메이션 재생

	if (Timer <= 0.0f)
	{
		DamageResult = UDamageCalculator::CalcDamage(Attacker, Defender);

		if (DamageResult.Damage > 0)
		{
			State = ESubstate::MoveDamage;
			Timer = HpBarDecreaseTime;
			PrevHp = Defender->CurPokemonReadonly()->GetCurHp();
			NextHp = UPokemonMath::Max(PrevHp - DamageResult.Damage, 0);
		}
		else
		{
			StateChangeToMoveBE();
		}
	}
}

void ABattleMoveStateMachine::ProcessMoveDamage()
{
	UPokemon* DefenderPokemon = Defender->CurPokemon();

	if (true == Defender->IsPlayer())
	{
		Canvas->LerpPlayerHpInfo(PrevHp, NextHp, DefenderPokemon->GetHp(), Timer / HpBarDecreaseTime);
	}
	else
	{
		Canvas->LerpEnemyHpInfo(PrevHp, NextHp, DefenderPokemon->GetHp(), Timer / HpBarDecreaseTime);
	}

	if (Timer <= 0.0f)
	{
		// 실제 데미지 처리
		DefenderPokemon->SetCurHp(NextHp);
		if (NextHp == 0)
		{
			DefenderPokemon->SetStatus(EPokemonStatus::Faint);
		}

		// 다음 상태로 전이
		if (true == DamageResult.IsCritical)
		{
			State = ESubstate::MoveCriticalMessage;
			MsgBox->SetMessage(L"A critical hit!");
			MsgBox->Write();
		}
		else if (ETypeVs::NormallyEffective != DamageResult.TypeVs)
		{
			State = ESubstate::MoveEffectiveMessage;
			MsgBox->SetMessage(DamageResult.GetTypeVsW(DefenderPokemon->GetNameW()));
			MsgBox->Write();
		}
		else
		{
			State = ESubstate::TestSE;
		}
	}
}

void ABattleMoveStateMachine::ProcessMoveCriticalMessage()
{
	const UPokemon* DefenderPokemon = Defender->CurPokemon();

	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		if (ETypeVs::NormallyEffective != DamageResult.TypeVs)
		{
			State = ESubstate::MoveEffectiveMessage;
			MsgBox->SetMessage(DamageResult.GetTypeVsW(DefenderPokemon->GetNameW()));
			MsgBox->Write();
		}
		else
		{
			State = ESubstate::TestSE;
		}
	}

}

void ABattleMoveStateMachine::ProcessMoveEffectiveMessage()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::TestSE;
	}
}

void ABattleMoveStateMachine::ProcessMoveBE()
{
	if (Timer <= 0.0f)
	{
		// 실제 BE 효과 적용
		State = ESubstate::MoveBEMessage;
		UMoveEffectApplier::ApplyBE(Attacker, Defender);
		Canvas->RefreshPlayerPokemonBox();
		Canvas->RefreshEnemyPokemonBox();
		MsgBox->SetMessage(BEResult.Message);
		MsgBox->Write();
	}
}

void ABattleMoveStateMachine::ProcessMoveBEMessage()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::TestSE;
	}
}

void ABattleMoveStateMachine::ProcessTestSE()
{
	const FPokemonMove* Move = Attacker->CurMove();
	EMoveEffectTarget SETarget = Move->SETarget;
	SEResult = UMoveEffectTester::TestSE(Attacker, Defender);

	// SE 대상이 자기 자신이고 테스트를 성공한 경우 SE 실행
	if (SETarget == EMoveEffectTarget::Self && true == SEResult.Success)
	{
		StateChangeToMoveSE();
		return;
	}

	// Faint 상태일 경우 SE를 스킵하고 Move 종료
	const UPokemon* DefenderPokemon = Defender->CurPokemonReadonly();
	if (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint)
	{
		State = ESubstate::End;
		return;
	}
	
	// SE가 없거나 실패한 경우 Move 종료
	if (SETarget == EMoveEffectTarget::None || false == SEResult.Success)
	{
		State = ESubstate::End;
		return;
	}

	StateChangeToMoveSE();
}

void ABattleMoveStateMachine::ProcessMoveSE()
{
	if (Timer <= 0.0f)
	{
		// 실제 SE 효과 적용
		State = ESubstate::MoveSEMessage;
		UMoveEffectApplier::ApplySE(Attacker, Defender);
		Canvas->RefreshPlayerPokemonBox();
		Canvas->RefreshEnemyPokemonBox();
		MsgBox->SetMessage(SEResult.Message);
		MsgBox->Write();
	}
}

void ABattleMoveStateMachine::ProcessMoveSEMessage()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::End;
	}
}

void ABattleMoveStateMachine::StateChangeToMoveFail(std::wstring _FailMessage)
{
	State = ESubstate::MoveFail;
	MsgBox->SetMessage(_FailMessage);
	MsgBox->Write();
}

void ABattleMoveStateMachine::StateChangeToMoveBE()
{
	State = ESubstate::MoveBE;
	MoveEffectState = EMoveEffectState::MoveEffect;
	Timer = MoveEffectShowTime;
}

void ABattleMoveStateMachine::StateChangeToMoveSE()
{
	State = ESubstate::MoveSE;
	MoveEffectState = EMoveEffectState::MoveEffect;
	Timer = MoveEffectShowTime;
}
