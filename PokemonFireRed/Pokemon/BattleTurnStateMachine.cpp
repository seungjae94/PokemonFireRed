#include "BattleTurnStateMachine.h"
#include <EnginePlatform/EngineInput.h>
#include "BattleCanvas.h"
#include "BattleEnemyActionGenerator.h"
#include "TurnOrderCalculator.h"
#include "AccuracyChecker.h"
#include "DamageCalculator.h"
#include "BattleEnums.h"
#include "Battler.h"

ABattleTurnStateMachine::ABattleTurnStateMachine()
{
}

ABattleTurnStateMachine::~ABattleTurnStateMachine()
{
}

void ABattleTurnStateMachine::Start(ABattleCanvas* _Canvas, UBattler* _Player, UBattler* _Enemy)
{
	// 입력 데이터 저장
	Canvas = _Canvas;
	Player = _Player;
	Enemy = _Enemy;

	// 적 액션 생성
	UBattleEnemyActionGenerator::Generate(Enemy);

	// 턴 순서 결정
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirst(Player, Enemy);
	IsFirstTurn = true;

	// 상태 변경
	if (true == IsPlayerFirst)
	{
		SetPlayerAsAttacker();
	}
	else
	{
		SetEnemyAsAttacker();
	}
	DispatchTurn();
}

void ABattleTurnStateMachine::Tick(float _DeltaTime)
{
	Timer -= _DeltaTime;

	switch (State)
	{
	case ABattleTurnStateMachine::ESubstate::None:
		break;
	case ABattleTurnStateMachine::ESubstate::PlayerEscapeFail:
		ProcessEscapeFail(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::Switch:
		break;
	case ABattleTurnStateMachine::ESubstate::UseItem:
		break;
	case ABattleTurnStateMachine::ESubstate::MoveFail:
		ProcessMoveFail(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveAnim:
		ProcessMoveAnim(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveDamage:
		ProcessMoveDamage(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::MoveSecondaryEffect:
		ProcessMoveSecondaryEffect(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::Faint:
		ProcessFaint(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::EndOfTurn:
		ProcessEndOfTurn(_DeltaTime);
		break;
	case ABattleTurnStateMachine::ESubstate::End:
		break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::DispatchTurn()
{
	EBattleAction AttackerAction = Attacker->CurAction();

	switch (AttackerAction)
	{
	case EBattleAction::Fight:
		DispatchFight();
		break;
	case EBattleAction::Escape:
	{
		State = ESubstate::PlayerEscapeFail;
		Canvas->SetBattleMessage(L"Can't Escape!");
	}
	break;
	case EBattleAction::Shift:
	{
		State = ESubstate::Switch;
		Canvas->SetBattleMessage(L"Debug - Attacker Changed Pokemon!");
	}
	break;
	case EBattleAction::Item:
	{
		State = ESubstate::UseItem;
		Canvas->SetBattleMessage(L"Debug - Attacker used Item!");
	}
	break;
	default:
		break;
	}
}

void ABattleTurnStateMachine::DispatchFight()
{
	bool AccuracyCheckResult = UAccuracyChecker::Check(Attacker, Defender);
	const FPokemonMove* Move = Attacker->CurMove();
	const UPokemon* AttackerPokemon = Attacker->CurPokemonReadonly();

	if (false == AccuracyCheckResult)
	{
		Canvas->SetBattleMessage(AttackerPokemon->GetNameW() + L"'s\nattack failed!");
		State = ESubstate::MoveFail;
		Timer = BattleMsgShowTime;
		return;
	}

	State = ESubstate::MoveAnim;
	Canvas->SetBattleMessage(AttackerPokemon->GetNameW() + L" used\n" + Move->GetNameW() + L"!");
	Result = UDamageCalculator::CalcDamage(Attacker, Defender);
}

void ABattleTurnStateMachine::DispatchNextPhase()
{
	if (true == IsFirstTurn)
	{
		if (Attacker == Player)
		{
			SetEnemyAsAttacker();
		}
		else
		{
			SetPlayerAsAttacker();
		}
		IsFirstTurn = false;
		DispatchTurn();
		return;
	}

	// 후공이 끝난 경우
	State = ESubstate::EndOfTurn;
}

void ABattleTurnStateMachine::DispatchSecondaryEffect()
{
	// Defender가 쓰러진 경우 처리
	if (Defender->CurPokemonReadonly()->GetCurHp() == 0)
	{
		DispatchFaint();
		return;
	}

	const FPokemonMove* Move = Attacker->CurMove();
	ESecondaryEffectTarget SETarget = Move->SETarget;

	// 부가 효과가 없는 경우 바로 다음 순서로 넘어간다.
	if (SETarget == ESecondaryEffectTarget::None)
	{
		DispatchNextPhase();
		return;
	}

	// 난수 테스트 성공시 부가 효과를 적용한다.
	int RandomNumber = UPokemonMath::RandomInt(0, 99);
	if (RandomNumber < Move->SERate)
	{
		State = ESubstate::MoveSecondaryEffect;
		SEState = ESecondaryEffectState::None;
		return;
	}

	DispatchNextPhase();
}

void ABattleTurnStateMachine::DispatchEndOfTurn()
{
	// TODO: 누가 EOT를 먼저 실행할지 결정
	State = ESubstate::EndOfTurn;
}

void ABattleTurnStateMachine::DispatchFaint()
{

}

void ABattleTurnStateMachine::ProcessEscapeFail(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('Z'))
	{
		IsFirstTurn = false;
		SetEnemyAsAttacker();
		DispatchTurn();
		return;
	}
}

void ABattleTurnStateMachine::ProcessMoveFail(float _DeltaTime)
{
	if (Timer < 0.0f)
	{
		if (true == IsFirstTurn)
		{
			IsFirstTurn = false;
			if (Attacker == Player)
			{
				SetEnemyAsAttacker();
				DispatchTurn();
			}
			else
			{
				SetPlayerAsAttacker();
				DispatchTurn();
			}
			return;
		}
		else
		{
			DispatchEndOfTurn();
			return;
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveAnim(float _DeltaTime)
{
	// 애니메이션: 차후 구현
	State = ESubstate::MoveDamage;
	Timer = DamageTime;
	MoveResultMsg = EMoveResultMsg::None;
	PrevHp = Defender->CurPokemonReadonly()->GetCurHp();
	NextHp = UPokemonMath::Max(PrevHp - Result.Damage, 0);
}

void ABattleTurnStateMachine::ProcessMoveDamage(float _DeltaTime)
{
	UPokemon* DefenderPokemon = Defender->CurPokemon();

	if (MoveResultMsg == EMoveResultMsg::None)
	{
		if (Defender == Player)
		{
			Canvas->LerpPlayerHpInfo(PrevHp, NextHp, DefenderPokemon->GetHp(), Timer / DamageTime);
		}
		else
		{
			Canvas->LerpEnemyHpInfo(PrevHp, NextHp, DefenderPokemon->GetHp(), Timer / DamageTime);
		}

		if (Timer <= 0.0f)
		{
			// 실제 데미지 처리
			DefenderPokemon->SetCurHp(NextHp);

			if (Result.IsCritical == true)
			{
				MoveResultMsg = EMoveResultMsg::Critical;
				Canvas->SetBattleMessage(L"A critical hit!");
				Timer = BattleMsgShowTime;
				return;
			}
			else if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveResultMsg::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(DefenderPokemon->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}
			
			// 데미지 종료
			DispatchSecondaryEffect();
		}
	}
	else if (MoveResultMsg == EMoveResultMsg::Critical)
	{
		if (Timer <= 0.0f)
		{
			if (Result.TypeVs != ETypeVs::NormallyEffective)
			{
				MoveResultMsg = EMoveResultMsg::TypeEffect;
				Canvas->SetBattleMessage(Result.GetTypeVsW(DefenderPokemon->GetNameW()));
				Timer = BattleMsgShowTime;
				return;
			}

			// 데미지 종료
			DispatchSecondaryEffect();
		}
	}
	else /*MoveResultMsg == EMoveResultMsg::TypeEffect*/
	{
		if (Timer <= 0.0f)
		{
			// 데미지 종료
			DispatchSecondaryEffect();
		}
	}
}

void ABattleTurnStateMachine::ProcessMoveSecondaryEffect(float _DeltaTime)
{
	if (SEState == ESecondaryEffectState::None)
	{
		// 첫 틱인 경우 스탯, 상태 변경 로직을 처리하고 메시지를 준비한다.
		const FPokemonMove* Move = Attacker->CurMove();
		ESecondaryEffectStatStage SEStatStageId = Move->SEStatStageId;
		EPokemonStatus SEStatusId = Move->SEStatusId;
		
		if (SEStatStageId != ESecondaryEffectStatStage::None)
		{
			ChangeStatStage();
		}
		else if (SEStatusId != EPokemonStatus::None)
		{
			// 일단 스탯과 상태를 동시에 변경하는 경우는 없다고 가정하고 구현한다.
			ChangeStatus();
		}

		SEState = ESecondaryEffectState::StatStageEffect;
		Timer = SEEffectShowTime;
	}
	else if (SEState == ESecondaryEffectState::StatStageEffect)
	{
		// TODO: Secondary Effect 애니메이션 재생

		if (Timer <= 0.0f)
		{
			SEState = ESecondaryEffectState::ShowSEMessage;
			Timer = BattleMsgShowTime;
			Canvas->SetBattleMessage(SEMessage);
		}
	}
	else /*SEState == ESecondaryEffectState::ShowSEMessage*/
	{
		if (Timer <= 0.0f)
		{
			DispatchNextPhase();
		}
	}
}

void ABattleTurnStateMachine::ProcessFaint(float _DeltaTime)
{
}


void ABattleTurnStateMachine::ProcessEndOfTurn(float _DeltaTime)
{
	IsPlayerFirst = UTurnOrderCalculator::IsPlayerFirstEOT(Player, Enemy);
	// Leech Seed 체크
	State = ESubstate::End;
}

void ABattleTurnStateMachine::SetPlayerAsAttacker()
{
	Attacker = Player;
	Defender = Enemy;
}

void ABattleTurnStateMachine::SetEnemyAsAttacker()
{
	Attacker = Enemy;
	Defender = Player;
}

void ABattleTurnStateMachine::ChangeStatStage()
{
	const FPokemonMove* Move = Attacker->CurMove();
	ESecondaryEffectTarget SETarget = Move->SETarget;
	ESecondaryEffectStatStage SEStatStageId = Move->SEStatStageId;

	UBattler* Target = nullptr;
	if (SETarget == ESecondaryEffectTarget::Self)
	{
		Target = Attacker;
	}
	else
	{
		Target = Defender;
	}

	SEMessage = L"";
	if (Target == Enemy)
	{
		SEMessage += L"Foe ";
	}

	UPokemon* TargetPokemon = Target->CurPokemon();

	SEMessage += TargetPokemon->GetNameW();
	SEMessage += L"'s ";

	int Value = Move->SEStatStageValue;
	std::wstring Suffix = GetStatStageMessageSuffix(Value);
	switch (SEStatStageId)
	{
	case ESecondaryEffectStatStage::Atk:
		SEMessage += L"ATTACK\n";
		Target->StatStage.AddAtk(Value);
		break;
	case ESecondaryEffectStatStage::Def:
		SEMessage += L"DEFENSE\n";
		Target->StatStage.AddDef(Value);
		break;
	case ESecondaryEffectStatStage::SpAtk:
		SEMessage += L"SP.ATK\n";
		Target->StatStage.AddSpAtk(Value);
		break;
	case ESecondaryEffectStatStage::SpDef:
		SEMessage += L"SP.DEF\n";
		Target->StatStage.AddDef(Value);
		break;
	case ESecondaryEffectStatStage::Speed:
		SEMessage += L"SPEED\n";
		Target->StatStage.AddSpeed(Value);
		break;
	case ESecondaryEffectStatStage::Accuracy:
		SEMessage += L"accuracy\n";
		Target->StatStage.AddAccuracy(Value);
		break;
	case ESecondaryEffectStatStage::Evasion:
		SEMessage += L"evasiveness\n";
		Target->StatStage.AddEvasion(Value);
		break;
	default:
		break;
	}
	SEMessage += Suffix;
}

void ABattleTurnStateMachine::ChangeStatus()
{
}

std::wstring ABattleTurnStateMachine::GetStatStageMessageSuffix(int _Value)
{
	if (_Value == 1)
	{
		return L"rose!";
	}
	else if (_Value >= 2)
	{
		return L"sharply rose!";
	}
	else if (_Value == -1)
	{
		return L"fell!";
	}
	return L"harshly fell!";
}
