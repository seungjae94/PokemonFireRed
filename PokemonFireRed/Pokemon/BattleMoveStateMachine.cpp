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

	// ��� ���� PP ����
	Attacker->DecCurMovePP();

	// ��Ȯ�� üũ
	bool AccuracyCheckResult = UAccuracyChecker::Check(Attacker, Defender);

	// ��Ȯ�� üũ �ܰ迡�� ������ ��� MoveFail ���·� ������.
	if (false == AccuracyCheckResult)
	{
		std::wstring BattleMsg = UBattleUtil::GetPokemonFullName(Attacker) + L"'s\nattack failed!";
		StateChangeToMoveFail(BattleMsg);
		return;
	}

	if (Move->BETarget != EMoveEffectTarget::None)
	{
		BEResult = UMoveEffectTester::TestBE(Attacker, Defender);

		// BE ��뿡 ������ ��� MoveFail ���·� ������.
		if (false == BEResult.Success)
		{
			StateChangeToMoveFail(BEResult.Message);
			return;
		}
	}

	// ��� ��뿡 �����ߴٴ� �޽��� ����Ѵ�.
	State = ESubstate::MoveUseMessage1;
	MsgBox->SetMessage(UBattleUtil::GetPokemonFullName(Attacker) + L" used\n" + Move->GetNameW() + L"!");
	MsgBox->Write();
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
	case ESubstate::MoveUseMessage1:
		ProcessMoveUseMessage1();
		break;
	case ESubstate::MoveUseMessage2:
		ProcessMoveUseMessage2();
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

void ABattleMoveStateMachine::ProcessMoveUseMessage1()
{
	if (EWriteState::WriteEnd == MsgBox->GetWriteState())
	{
		State = ESubstate::MoveUseMessage2;
		Timer = BattleMsgShowTime;
	}
}

void ABattleMoveStateMachine::ProcessMoveUseMessage2()
{
	if (Timer <= 0.0f)
	{
		// �޽��� ����� ��ģ �� Move �ִϸ��̼��� ����Ѵ�.
		State = ESubstate::MoveAnim;

		// ���� �ִϸ����Ͱ� ������ �����Ѵ�.
		if (nullptr != Animator)
		{
			Animator->Destroy();
		}

		// ������ ȿ���� ���� �ٸ� SE�� ������ ��찡 �ֱ� ������ ���� ����Ѵ�.
		DamageResult = UDamageCalculator::CalcDamage(Attacker, Defender);

		const FPokemonMove* Move = Attacker->CurMove();
		Animator = AnimatorGenerator->GenerateMoveAnimator(Attacker, Move->Id, DamageResult.TypeVs);
		Animator->Start();
	}
}

void ABattleMoveStateMachine::ProcessMoveAnim()
{
	if (true == Animator->IsEnd())
	{
		if (true == Attacker->IsPlayer())
		{
			DamageResult.Damage = UPokemonMath::Floor(DamageResult.Damage * Global::DamageBonusCoeff);
		}

		if (DamageResult.Damage > 0)
		{
			State = ESubstate::MoveDamage;
			PrevHp = Defender->CurPokemonReadonly()->GetCurHp();
			NextHp = UPokemonMath::Max(PrevHp - DamageResult.Damage, 0);

			int EffectiveDamage = PrevHp - NextHp;
			int MaxHp = Defender->CurPokemonReadonly()->GetHp();

			HpBarDecreaseTime = HpBarDecreaseBaseTime + HpBarDecreaseTimeVariance * EffectiveDamage / MaxHp;
			Timer = HpBarDecreaseTime;
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
		// ���� ������ ó��
		DefenderPokemon->SetCurHp(NextHp);
		if (NextHp == 0)
		{
			DefenderPokemon->SetStatus(EPokemonStatus::Faint);
		}

		// ���� ���·� ����
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

	// ���� �ִϸ����� ����
	if (nullptr != Animator)
	{
		Animator->Destroy();
	}

	// ���� ��ȭ�� ���
	if (Move->BEStatusId != EPokemonStatus::Normal)
	{
		//Animator = AnimatorGenerator->GenerateSkipAnimator();
	}
	// ���� ��ȭ�� ���
	else
	{
		bool IsStatDown = Move->BEStatStageValue < 0;

		if (Move->BETarget == EMoveEffectTarget::Enemy)
		{
			
			Animator = AnimatorGenerator->GenerateStatStageEffectAnimator(Defender, IsStatDown);
		}
		else
		{
			Animator = AnimatorGenerator->GenerateStatStageEffectAnimator(Attacker, IsStatDown);
		}

		if (true == IsStatDown)
		{
			USoundManager::PlaySE(RN::SEStatDown);
		}
		else
		{
			USoundManager::PlaySE(RN::SEStatUp);
		}
	}

	State = ESubstate::MoveBEAnim;
	Animator->Start();
}

void ABattleMoveStateMachine::ProcessMoveBEAnim()
{
	if (true == Animator->IsEnd())
	{
		// ���� BE ȿ�� ����
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

	// SE ����� �ڱ� �ڽ��̰� �׽�Ʈ�� ������ ��� SE ����
	if (SETarget == EMoveEffectTarget::Self && true == SEResult.Success)
	{
		StateChangeToMoveSE();
		return;
	}

	// Faint ������ ��� SE�� ��ŵ�ϰ� Move ����
	const UPokemon* DefenderPokemon = Defender->CurPokemonReadonly();
	if (DefenderPokemon->GetStatusId() == EPokemonStatus::Faint)
	{
		State = ESubstate::End;
		return;
	}
	
	// SE�� ���ų� ������ ��� Move ����
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

	// ���� �ִϸ����� ����
	if (nullptr != Animator)
	{
		Animator->Destroy();
	}

	// ���� ��ȭ�� ���
	if (Move->SEStatusId == EPokemonStatus::Normal)
	{
		//Animator = AnimatorGenerator->GenerateSkipAnimator();
	}
	// ���� ��ȭ�� ���
	else
	{
		bool IsStatDown = Move->SEStatStageValue < 0;
		if (Move->SETarget == EMoveEffectTarget::Enemy)
		{
			Animator = AnimatorGenerator->GenerateStatStageEffectAnimator(Defender, IsStatDown);
		}
		else
		{
			Animator = AnimatorGenerator->GenerateStatStageEffectAnimator(Attacker, IsStatDown);
		}
	}

	State = ESubstate::MoveSEAnim;
	Animator->Start();
}

void ABattleMoveStateMachine::ProcessMoveSEAnim()
{
	if (true == Animator->IsEnd())
	{
		// ���� SE ȿ�� ����
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
