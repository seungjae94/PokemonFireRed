#include "BattleMoveStateMachine.h"
#include "BattleUtil.h"
#include "BattleCanvas.h"
#include "PokemonMsgBox.h"
#include "AnimatorGenerator.h"

ABattleMoveStateMachine::ABattleMoveStateMachine()
{
}

ABattleMoveStateMachine::~ABattleMoveStateMachine()
{
}

void ABattleMoveStateMachine::Start(UBattler* _Attacker, UBattler* _Defender)
{
	ABattleStateMachine::Start();

	Attacker = _Attacker;
	Defender = _Defender;

	const FPokemonMove* Move = Attacker->CurMove();

	// 기술 사용시 PP 감소
	Attacker->DecCurMovePP();

	// 정확도 체크
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
	
	// 기존 애니메이터 삭제
	if (nullptr != Animator)
	{
		Animator->Destroy();
	}
	Animator = AnimatorGenerator->Generate(_Attacker, Move->Id);
	Animator->Start();
}

void ABattleMoveStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ESubstate::None:
		break;
	case ESubstate::MoveFail1:
		ProcessMoveFail1();
		break;
	case ESubstate::MoveFail2:
		ProcessMoveFail2();
		break;
	case ESubstate::MoveAnim:
		ProcessMoveAnim();
		break;
	case ESubstate::MoveDamage:
		ProcessMoveDamage();
		break;
	case ESubstate::MoveCriticalMessage1:
		ProcessMoveCriticalMessage1();
		break;
	case ESubstate::MoveCriticalMessage2:
		ProcessMoveCriticalMessage2();
		break;
	case ESubstate::MoveEffectiveMessage1:
		ProcessMoveEffectiveMessage1();
		break;
	case ESubstate::MoveEffectiveMessage2:
		ProcessMoveEffectiveMessage2();
		break;
	case ESubstate::MoveBEStart:
		ProcessMoveBEStart();
		break;
	case ESubstate::MoveBEAnim:
		ProcessMoveBEAnim();
		break;
	case ESubstate::MoveBEMessage1:
		ProcessMoveBEMessage1();
		break;
	case ESubstate::MoveBEMessage2:
		ProcessMoveBEMessage2();
		break;
	case ESubstate::TestSE:
		ProcessTestSE();
		break;
	case ESubstate::MoveSEStart:
		ProcessMoveSEStart();
		break;
	case ESubstate::MoveSEAnim:
		ProcessMoveSEAnim();
		break;
	case ESubstate::MoveSEMessage1:
		ProcessMoveSEMessage1();
		break;
	case ESubstate::MoveSEMessage2:
		ProcessMoveSEMessage2();
		break;
	case ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleMoveStateMachine::ProcessMoveFail1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::MoveFail2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleMoveStateMachine::ProcessMoveFail2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}

void ABattleMoveStateMachine::ProcessMoveAnim()
{
	if (true == Animator->IsEnd())
	{
		DamageResult = UDamageCalculator::CalcDamage(Attacker, Defender);

		if (true == Attacker->IsPlayer())
		{
			DamageResult.Damage = UPokemonMath::Floor(DamageResult.Damage * Global::DamageBonusCoeff);
		}

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
			State = ESubstate::MoveCriticalMessage1;
			MsgBox->SetMessage(L"A critical hit!");
			MsgBox->Write();
		}
		else if (ETypeVs::NormallyEffective != DamageResult.TypeVs)
		{
			State = ESubstate::MoveEffectiveMessage1;
			MsgBox->SetMessage(DamageResult.GetTypeVsW(DefenderPokemon->GetNameW()));
			MsgBox->Write();
		}
		else
		{
			State = ESubstate::TestSE;
		}
	}
}

void ABattleMoveStateMachine::ProcessMoveCriticalMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::MoveCriticalMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleMoveStateMachine::ProcessMoveCriticalMessage2()
{
	if (Timer <= 0.0f)
	{
		const UPokemon* DefenderPokemon = Defender->CurPokemon();

		if (ETypeVs::NormallyEffective != DamageResult.TypeVs)
		{
			State = ESubstate::MoveEffectiveMessage1;
			MsgBox->SetMessage(DamageResult.GetTypeVsW(DefenderPokemon->GetNameW()));
			MsgBox->Write();
		}
		else
		{
			State = ESubstate::TestSE;
		}
	}
}

void ABattleMoveStateMachine::ProcessMoveEffectiveMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::MoveEffectiveMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleMoveStateMachine::ProcessMoveEffectiveMessage2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::TestSE;
	}
}

void ABattleMoveStateMachine::ProcessMoveBEStart()
{
	const FPokemonMove* Move = Attacker->CurMove();

	// 상태 변화인 경우
	if (Move->BEStatusId != EPokemonStatus::Normal)
	{

	}
	// 스탯 변화인 경우
	else
	{
		bool IsStatDown = Move->BEStatStageValue < 0;
		
		// 기존 애니메이터 삭제
		if (nullptr != Animator)
		{
			Animator->Destroy();
		}

		if (Move->BETarget == EMoveEffectTarget::Enemy)
		{
			Animator = AnimatorGenerator->Generate(Defender, IsStatDown);
		}
		else
		{
			Animator = AnimatorGenerator->Generate(Attacker, IsStatDown);
		}
	}

	State = ESubstate::MoveBEAnim;
	Animator->Start();
}

void ABattleMoveStateMachine::ProcessMoveBEAnim()
{
	if (true == Animator->IsEnd())
	{
		// 실제 BE 효과 적용
		State = ESubstate::MoveBEMessage1;
		UMoveEffectApplier::ApplyBE(Attacker, Defender);
		Canvas->RefreshPlayerPokemonBox();
		Canvas->RefreshEnemyPokemonBox();
		MsgBox->SetMessage(BEResult.Message);
		MsgBox->Write();
	}
}

void ABattleMoveStateMachine::ProcessMoveBEMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::MoveBEMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleMoveStateMachine::ProcessMoveBEMessage2()
{
	if (Timer <= 0.0f)
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

void ABattleMoveStateMachine::ProcessMoveSEStart()
{
	const FPokemonMove* Move = Attacker->CurMove();

	// 상태 변화인 경우
	if (Move->SEStatusId == EPokemonStatus::Normal)
	{

	}
	// 스탯 변화인 경우
	else
	{
		bool IsStatDown = Move->SEStatStageValue < 0;
		if (Move->SETarget == EMoveEffectTarget::Enemy)
		{
			Animator = AnimatorGenerator->Generate(Defender, IsStatDown);
		}
		else
		{
			Animator = AnimatorGenerator->Generate(Attacker, IsStatDown);
		}
	}

	State = ESubstate::MoveSEAnim;
	Animator->Start();
}

void ABattleMoveStateMachine::ProcessMoveSEAnim()
{
	if (true == Animator->IsEnd())
	{
		// 실제 SE 효과 적용
		State = ESubstate::MoveSEMessage1;
		UMoveEffectApplier::ApplySE(Attacker, Defender);
		Canvas->RefreshPlayerPokemonBox();
		Canvas->RefreshEnemyPokemonBox();
		MsgBox->SetMessage(SEResult.Message);
		MsgBox->Write();
	}
}

void ABattleMoveStateMachine::ProcessMoveSEMessage1()
{
	if (MsgBox->GetWriteState() == EWriteState::WriteEnd)
	{
		State = ESubstate::MoveSEMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleMoveStateMachine::ProcessMoveSEMessage2()
{
	if (Timer <= 0.0f)
	{
		State = ESubstate::End;
	}
}

void ABattleMoveStateMachine::StateChangeToMoveFail(std::wstring _FailMessage)
{
	State = ESubstate::MoveFail1;
	MsgBox->SetMessage(_FailMessage);
	MsgBox->Write();
}

void ABattleMoveStateMachine::StateChangeToMoveBE()
{
	State = ESubstate::MoveBEStart;
}

void ABattleMoveStateMachine::StateChangeToMoveSE()
{
	State = ESubstate::MoveSEStart;
}
